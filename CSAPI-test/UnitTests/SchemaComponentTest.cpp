#include "pch.h"
#include "CppUnitTest.h"

#include <iostream>
#include <sstream>
#include <string>
#include <variant>
#include <nlohmann/json.hpp>

#include "RegistryInit.h"
#include "DataModels/Component/UnparsedComponent.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ConnectedSystemsAPI::DataModels::Component;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(SchemaComponentTest)
	{
		/// <summary>
		/// Parses a component and asserts that serializing it reproduces the input (ignoring key order).
		/// </summary>
		static std::unique_ptr<DataComponent> assertRoundTrip(const char* input) {
			auto j = nlohmann::json::parse(input);
			auto component = DataComponentRegistry::createDataComponent(j);
			auto output = nlohmann::json(component->toJson());
			if (output != j) Logger::WriteMessage(("Expected " + j.dump() + "\nGot      " + output.dump() + "\n").c_str());
			Assert::IsTrue(output == j, L"Component did not round-trip");
			return component;
		}

	public:
		TEST_METHOD(ReferenceFrame_IsReadAndWritten) {
			auto component = assertRoundTrip(R"({
				"type": "Time", "name": "time", "definition": "http://www.opengis.net/def/property/OGC/0/SamplingTime",
				"referenceFrame": "http://www.opengis.net/def/trs/BIPM/0/UTC",
				"uom": { "href": "http://www.opengis.net/def/uom/ISO-8601/0/Gregorian" }
			})");
			auto time = dynamic_cast<Time*>(component.get());
			Assert::AreEqual("http://www.opengis.net/def/trs/BIPM/0/UTC"s, time->getReferenceFrame().value());
		}

		TEST_METHOD(Quality_IsReadAndWritten) {
			auto component = assertRoundTrip(R"({
				"type": "Quantity", "name": "lat", "uom": { "code": "deg" },
				"quality": [
					{ "type": "Quantity", "name": "accuracy", "uom": { "code": "m" }, "value": 5.0 },
					{ "type": "Text", "name": "method", "value": "GPS" }
				]
			})");
			const auto& quality = dynamic_cast<Quantity*>(component.get())->getQuality();
			Assert::AreEqual(size_t(2), quality.size());
			Assert::AreEqual("Quantity"s, quality[0]->getType());
			Assert::AreEqual(5.0, dynamic_cast<Quantity*>(quality[0].get())->getValue().value());
		}

		TEST_METHOD(Quality_CopyIsDeep) {
			auto j = nlohmann::json::parse(R"({ "type": "Count", "name": "n", "quality": [ { "type": "Text", "name": "note", "value": "a" } ] })");
			auto original = j.get<Count>();
			Count copy = original;

			Assert::AreEqual(size_t(1), copy.getQuality().size());
			Assert::IsTrue(copy.getQuality()[0].get() != original.getQuality()[0].get());
			Assert::IsTrue(copy == original);
		}

		TEST_METHOD(AllowedTokens_ValuesAndPattern_RoundTrip) {
			auto component = assertRoundTrip(R"({
				"type": "Category", "name": "mode",
				"constraint": { "values": [ "AUTO", "MANUAL" ], "pattern": "[A-Z]+" }
			})");
			const auto& constraint = dynamic_cast<Category*>(component.get())->getConstraint();
			Assert::IsTrue(constraint.has_value());
			Assert::AreEqual(size_t(2), constraint->getValues()->size());
			Assert::AreEqual("[A-Z]+"s, constraint->getPattern().value());
		}

		TEST_METHOD(AllowedTokens_Empty_IsKept) {
			auto component = assertRoundTrip(R"({ "type": "Text", "name": "t", "constraint": {} })");
			Assert::IsTrue(dynamic_cast<Text*>(component.get())->hasConstraint());
		}

		TEST_METHOD(AllowedValues_Intervals_RoundTrip) {
			auto component = assertRoundTrip(R"({
				"type": "Quantity", "name": "lon", "uom": { "code": "deg" },
				"constraint": { "intervals": [ [ -180.0, 180.0 ] ], "significantFigures": 9 }
			})");
			const auto& constraint = dynamic_cast<Quantity*>(component.get())->getConstraint();
			Assert::AreEqual(-180.0, constraint->getIntervals()->at(0).first);
			Assert::AreEqual(180.0, constraint->getIntervals()->at(0).second);
			Assert::AreEqual(9, constraint->getSignificantFigures().value());
		}

		TEST_METHOD(AllowedValues_CountValues_RoundTrip) {
			auto component = assertRoundTrip(R"({ "type": "Count", "name": "c", "constraint": { "values": [ 0.0, 1.0, 2.0 ] } })");
			Assert::AreEqual(size_t(3), dynamic_cast<Count*>(component.get())->getConstraint()->getValues()->size());
		}

		TEST_METHOD(AllowedValues_RangeComponents_RoundTrip) {
			assertRoundTrip(R"({ "type": "CountRange", "name": "c", "constraint": { "intervals": [ [ 0.0, 10.0 ] ] } })");
			assertRoundTrip(R"({ "type": "QuantityRange", "name": "q", "uom": { "code": "m" }, "constraint": { "values": [ 1.5 ] } })");
			assertRoundTrip(R"({ "type": "CategoryRange", "name": "r", "constraint": { "values": [ "A", "B" ] } })");
		}

		TEST_METHOD(AllowedTimes_StringsAndNumbers_RoundTrip) {
			auto component = assertRoundTrip(R"({
				"type": "Time", "name": "t", "uom": { "href": "http://www.opengis.net/def/uom/ISO-8601/0/Gregorian" },
				"constraint": {
					"values": [ "2026-01-02T03:04:05Z", 1767323045.0 ],
					"intervals": [ [ "2026-01-01T00:00:00Z", "2026-12-31T23:59:59Z" ] ]
				}
			})");
			const auto& constraint = dynamic_cast<Time*>(component.get())->getConstraint();
			Assert::IsTrue(std::holds_alternative<std::string>(constraint->getValues()->at(0)));
			Assert::IsTrue(std::holds_alternative<double>(constraint->getValues()->at(1)));
			Assert::AreEqual("2026-01-01T00:00:00Z"s, std::get<std::string>(constraint->getIntervals()->at(0).first));
		}

		TEST_METHOD(AllowedTimes_TimeRange_RoundTrip) {
			assertRoundTrip(R"({ "type": "TimeRange", "name": "t", "uom": { "code": "s" }, "constraint": { "intervals": [ [ 0.0, 60.0 ] ] } })");
		}

		TEST_METHOD(DataRecord_UnparseableFields_AreKeptWithoutOutput) {
			const char* input = R"({
				"type": "DataRecord", "name": "rec",
				"fields": [
					{ "type": "Text", "name": "ok" },
					{ "type": "Matrix", "name": "unknownType", "elementCount": { "value": 3 } },
					{ "name": "missingType" },
					{ "type": "Quantity", "name": "missingUom" }
				]
			})";

			std::ostringstream captured;
			auto oldBuf = std::cerr.rdbuf(captured.rdbuf());
			auto component = assertRoundTrip(input);
			std::cerr.rdbuf(oldBuf);
			Assert::AreEqual(""s, captured.str());

			const auto& fields = dynamic_cast<DataRecord*>(component.get())->getFields();
			Assert::AreEqual(size_t(4), fields.size());
			Assert::IsNull(dynamic_cast<UnparsedComponent*>(fields[0].get()));
			for (size_t i = 1; i < 4; ++i) {
				auto unparsed = dynamic_cast<UnparsedComponent*>(fields[i].get());
				Assert::IsNotNull(unparsed);
				Assert::IsFalse(unparsed->getError().empty());
			}
			Assert::AreEqual("unknownType"s, fields[1]->getName().value());
			Assert::AreEqual("Matrix"s, fields[1]->getType());
			Assert::IsTrue(dynamic_cast<UnparsedComponent*>(fields[1].get())->getError().find("Unknown component type: Matrix") != std::string::npos);
		}

		TEST_METHOD(Vector_UnparseableCoordinate_IsKept) {
			auto component = assertRoundTrip(R"({
				"type": "Vector", "name": "v", "referenceFrame": "urn:frame",
				"coordinates": [ { "type": "Quantity", "name": "x", "uom": { "code": "m" } }, { "type": "Unknown", "name": "y" } ]
			})");
			const auto& coordinates = dynamic_cast<Vector*>(component.get())->getCoordinates();
			Assert::AreEqual(size_t(2), coordinates.size());
			Assert::IsNotNull(dynamic_cast<UnparsedComponent*>(coordinates[1].get()));
		}

		TEST_METHOD(DataArray_UnparseableElementType_IsKept) {
			auto component = assertRoundTrip(R"({
				"type": "DataArray", "name": "a", "elementCount": { "href": "#n" }, "elementType": { "type": "Unknown", "name": "e" }
			})");
			Assert::IsNotNull(dynamic_cast<const UnparsedComponent*>(dynamic_cast<DataArray*>(component.get())->getElementType()));
		}

		TEST_METHOD(ServerStyleSchema_RoundTrips) {
			// Field shapes taken from an OpenSensorHub node's Meshtastic schemas
			assertRoundTrip(R"({
				"type": "DataRecord", "name": "MeshtasticPositionV3", "definition": "urn:osh:def:meshtastic:position:3.0.0",
				"fields": [
					{ "type": "Text", "name": "schemaVersion", "definition": "urn:osh:def:meshtastic#schemaVersion", "constraint": { "values": [ "3.0.0" ] } },
					{ "type": "Time", "name": "receiptTime", "definition": "http://www.opengis.net/def/property/OGC/0/SamplingTime",
					  "referenceFrame": "http://www.opengis.net/def/trs/BIPM/0/UTC", "uom": { "href": "http://www.opengis.net/def/uom/ISO-8601/0/Gregorian" } },
					{ "type": "Category", "name": "timeQuality", "definition": "urn:osh:def:meshtastic#timeQuality",
					  "constraint": { "values": [ "POSITION_TIMESTAMP", "POSITION_TIME", "PACKET_RX_TIME", "RECEIPT_TIME" ] } },
					{ "type": "Count", "name": "channelIndex", "definition": "urn:osh:def:meshtastic#channelIndex",
					  "constraint": { "values": [ 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 ] } },
					{ "type": "Quantity", "name": "longitude", "definition": "http://sensorml.com/ont/swe/property/Longitude",
					  "uom": { "code": "deg" }, "axisID": "Lon", "constraint": { "intervals": [ [ -180.0, 180.0 ] ] } },
					{ "type": "Count", "name": "packetMetadataCount", "id": "packetMetadataCount" },
					{ "type": "DataArray", "name": "packetMetadata", "elementCount": { "href": "#packetMetadataCount" },
					  "elementType": { "type": "DataRecord", "name": "entry", "fields": [ { "type": "Text", "name": "fieldName" } ] } }
				]
			})");
		}
	};
}
