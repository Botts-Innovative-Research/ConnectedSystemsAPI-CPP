#include "pch.h"
#include "CppUnitTest.h"

#include <nlohmann/json.hpp>

#include "DataModels/Observation.h"
#include "DataModels/Data/DataBlockMixed.h"
#include "DataModels/Data/DataValue.h"
#include "DataModels/Component/DataRecord.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ConnectedSystemsAPI::DataModels;

namespace CSAPItest {
	TEST_CLASS(DataBlockMixedTest)
	{
	public:
		TEST_METHOD(FromJson_NestedObject_ParsesAsDataBlock) {
			auto j = nlohmann::json::parse(R"({ "location": { "lat": 34.5, "lon": -86.5, "alt": 193.0 } })");
			auto block = j.get<Data::DataBlockMixed>();

			auto location = block.getField("location");
			Assert::IsNotNull(location);
			Assert::IsTrue(location->isDataBlock());

			auto lat = location->getDataBlock()->getField("lat");
			Assert::IsNotNull(lat);
			Assert::AreEqual(34.5, std::get<double>(lat->value));
		}

		TEST_METHOD(FromJson_Array_ParsesElementsRecursively) {
			auto j = nlohmann::json::parse(R"({ "points": [ { "x": 1 }, { "x": 2 } ], "flags": [ true, false ] })");
			auto block = j.get<Data::DataBlockMixed>();

			auto points = block.getField("points");
			Assert::IsNotNull(points);
			Assert::IsTrue(points->isArray());
			Assert::AreEqual(size_t(2), points->getArray()->size());
			auto x = points->getArray()->at(1).getDataBlock()->getField("x");
			Assert::AreEqual(int64_t(2), std::get<int64_t>(x->value));

			auto flags = block.getField("flags");
			Assert::IsTrue(flags->isArray());
			Assert::IsFalse(std::get<bool>(flags->getArray()->at(1).value));
		}

		TEST_METHOD(ToJson_NestedValues_RoundTrip) {
			auto input = nlohmann::ordered_json::parse(R"({ "location": { "lat": 34.5, "lon": -86.5 }, "points": [ 1, [ 2, 3 ], { "y": "z" } ] })");
			auto block = input.get<Data::DataBlockMixed>();

			nlohmann::ordered_json output;
			to_json(output, block);
			Assert::AreEqual(input.dump(), output.dump());
		}

		TEST_METHOD(Observation_NestedResult_RoundTrip) {
			auto j = nlohmann::json::parse(R"({
				"id": "obs1",
				"datastream@id": "ds1",
				"phenomenonTime": "2026-10-28T14:10:56.184Z",
				"resultTime": "2026-10-28T14:10:56.184Z",
				"result": { "location": { "alt": 193.0, "lat": 34.75, "lon": -86.56 } }
			})");
			auto observation = j.get<Observation>();

			Assert::AreEqual(j["result"].dump(), nlohmann::json(observation.toJson()["result"]).dump());
		}

		TEST_METHOD(Copy_NestedDataBlock_IsDeep) {
			Data::DataBlockMixed inner;
			inner.setField("lat", Data::DataValue(1.0));
			Data::DataValue original(inner);

			Data::DataValue copy = original;
			copy.getDataBlock()->setField("lat", Data::DataValue(2.0));

			auto originalLat = original.getDataBlock()->getField("lat");
			Assert::AreEqual(1.0, std::get<double>(originalLat->value));
		}

		TEST_METHOD(DataRecord_CreateDataBlock_NestsVectorCoordinates) {
			auto schema = nlohmann::json::parse(R"({
				"type": "DataRecord",
				"fields": [ {
					"type": "Vector",
					"name": "location",
					"referenceFrame": "http://www.opengis.net/def/crs/EPSG/0/4979",
					"coordinates": [
						{ "type": "Quantity", "name": "lat", "uom": { "code": "deg" } },
						{ "type": "Quantity", "name": "lon", "uom": { "code": "deg" } }
					]
				} ]
			})");
			auto record = schema.get<Component::DataRecord>();
			auto block = record.createDataBlock();

			auto location = block.getField("location");
			Assert::IsNotNull(location);
			Assert::IsTrue(location->isDataBlock());
			Assert::IsTrue(location->getDataBlock()->hasField("lat"));
			Assert::IsTrue(location->getDataBlock()->hasField("lon"));
		}
	};
}
