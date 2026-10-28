#include "pch.h"
#include "CppUnitTest.h"

#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

#include "DataModels/Component/DataArray.h"
#include "DataModels/Component/DataRecord.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ConnectedSystemsAPI::DataModels::Component;

namespace CSAPItest {
	TEST_CLASS(DataArrayTest)
	{
		static nlohmann::json linkedArrayJson() {
			return nlohmann::json::parse(R"({
				"type": "DataArray",
				"name": "packetMetadata",
				"elementCount": { "href": "#packetMetadataCount" },
				"elementType": { "type": "Text", "name": "entry" }
			})");
		}

		static nlohmann::json inlineArrayJson() {
			return nlohmann::json::parse(R"({
				"type": "DataArray",
				"name": "row",
				"elementCount": { "type": "Count", "name": "elementCount", "value": 1280 },
				"elementType": { "type": "Count", "name": "pixel" }
			})");
		}

	public:
		TEST_METHOD(FromJson_ElementCountLink_ParsesHref) {
			auto array = linkedArrayJson().get<DataArray>();

			Assert::IsTrue(array.hasElementCountLink());
			Assert::AreEqual(std::string("#packetMetadataCount"), array.getElementCountLink().value());
			Assert::IsNotNull(array.getElementType());
		}

		TEST_METHOD(FromJson_InlineElementCount_ParsesCount) {
			auto array = inlineArrayJson().get<DataArray>();

			Assert::IsFalse(array.hasElementCountLink());
			Assert::AreEqual(1280, array.getElementCount().getValue().value());
		}

		TEST_METHOD(ToJson_ElementCountLink_WritesHrefOnly) {
			auto array = linkedArrayJson().get<DataArray>();

			auto expected = nlohmann::json::parse(R"({ "href": "#packetMetadataCount" })");
			Assert::AreEqual(expected.dump(), nlohmann::json(array.toJson()["elementCount"]).dump());
		}

		TEST_METHOD(ToJson_InlineElementCount_WritesCount) {
			auto array = inlineArrayJson().get<DataArray>();

			auto elementCount = array.toJson()["elementCount"];
			Assert::AreEqual(std::string("Count"), elementCount["type"].get<std::string>());
			Assert::AreEqual(1280, elementCount["value"].get<int>());
		}

		TEST_METHOD(Copy_ElementCountLink_IsPreserved) {
			auto array = linkedArrayJson().get<DataArray>();
			DataArray copy = array;

			Assert::IsTrue(copy.hasElementCountLink());
			Assert::IsTrue(array == copy);
		}

		TEST_METHOD(SetElementCount_ClearsLink) {
			auto array = linkedArrayJson().get<DataArray>();
			Count count;
			count.setType("Count");
			count.setValue(3);
			array.setElementCount(count);

			Assert::IsFalse(array.hasElementCountLink());
			Assert::AreEqual(3, array.getElementCount().getValue().value());
		}

		TEST_METHOD(SetElementCountLink_ClearsInlineCount) {
			auto array = inlineArrayJson().get<DataArray>();
			array.setElementCountLink("#rowCount");

			Assert::IsTrue(array.hasElementCountLink());
			Assert::IsFalse(array.getElementCount().hasValue());
		}

		TEST_METHOD(Validate_ElementCountLink_Passes) {
			auto array = linkedArrayJson().get<DataArray>();
			array.validate();
		}

		TEST_METHOD(DataRecord_WithLinkedDataArray_KeepsFieldWithoutWarnings) {
			auto j = nlohmann::json::parse(R"({
				"type": "DataRecord",
				"fields": [
					{ "type": "Count", "name": "packetMetadataCount", "id": "packetMetadataCount" },
					{
						"type": "DataArray",
						"name": "packetMetadata",
						"elementCount": { "href": "#packetMetadataCount" },
						"elementType": { "type": "Text", "name": "entry" }
					}
				]
			})");

			std::ostringstream captured;
			auto oldBuf = std::cerr.rdbuf(captured.rdbuf());
			auto record = j.get<DataRecord>();
			std::cerr.rdbuf(oldBuf);

			Assert::AreEqual(std::string(), captured.str());
			Assert::AreEqual(size_t(2), record.getFields().size());
			auto array = dynamic_cast<const DataArray*>(record.getFields()[1].get());
			Assert::IsNotNull(array);
			Assert::AreEqual(std::string("#packetMetadataCount"), array->getElementCountLink().value());
		}
	};
}
