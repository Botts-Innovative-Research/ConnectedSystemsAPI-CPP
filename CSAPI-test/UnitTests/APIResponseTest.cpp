#include "pch.h"
#include "CppUnitTest.h"

#include <string>

#include "APIResponse.h"
#include "DataModels/Observation.h"
#include "DataModels/System.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ConnectedSystemsAPI;
using namespace ConnectedSystemsAPI::DataModels;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(APIResponseTest)
	{
		static constexpr const char* SYSTEM_1 = R"({"type":"Feature","id":"sys1","properties":{"featureType":"http://www.w3.org/ns/sosa/Sensor","uid":"urn:test:sys1","name":"One"}})";
		static constexpr const char* SYSTEM_2 = R"({"type":"Feature","id":"sys2","properties":{"featureType":"http://www.w3.org/ns/sosa/Sensor","uid":"urn:test:sys2","name":"Two"}})";
		// Missing the required "properties"
		static constexpr const char* BAD_SYSTEM = R"({"type":"Feature","id":"bad"})";

		static APIResponse<System> systems(int code, const std::string& body) {
			return APIResponse<System>(code, "", body, {});
		}

	public:
		TEST_METHOD(List_AllValid_IsSuccessful) {
			auto response = systems(200, "{\"items\":["s + SYSTEM_1 + "," + SYSTEM_2 + "]}");

			Assert::IsTrue(response.isSuccessful());
			Assert::IsFalse(response.hasParseErrors());
			Assert::AreEqual(size_t(2), response.getItems().size());
		}

		TEST_METHOD(List_Empty_IsSuccessful) {
			auto response = systems(200, R"({"items":[]})");

			Assert::IsTrue(response.isSuccessful());
			Assert::IsTrue(response.getItems().empty());
		}

		TEST_METHOD(List_BadItem_IsSkippedAndReported) {
			auto response = systems(200, "{\"items\":["s + SYSTEM_1 + "," + BAD_SYSTEM + "," + SYSTEM_2 + "]}");

			Assert::IsTrue(response.isSuccessful());
			Assert::AreEqual(size_t(2), response.getItems().size());
			Assert::AreEqual("sys2"s, response.getItems()[1].getId());
			Assert::AreEqual(size_t(1), response.getParseErrors().size());
			Assert::IsTrue(response.getParseErrors()[0].find("items[1]: ") == 0);
		}

		TEST_METHOD(Single_Valid_IsSuccessful) {
			auto response = systems(200, SYSTEM_1);

			Assert::IsTrue(response.isSuccessful());
			Assert::AreEqual("sys1"s, response.getItem().getId());
		}

		TEST_METHOD(Single_BadItem_IsNotSuccessful) {
			auto response = systems(200, BAD_SYSTEM);

			Assert::IsFalse(response.isSuccessful());
			Assert::IsTrue(response.getItems().empty());
			Assert::AreEqual(size_t(1), response.getParseErrors().size());
		}

		TEST_METHOD(Single_BadTimestamp_IsNotSuccessful) {
			auto response = APIResponse<Observation>(200, "", R"({"id":"obs1","phenomenonTime":"yesterday","result":{}})", {});

			Assert::IsFalse(response.isSuccessful());
			Assert::IsTrue(response.getParseErrors()[0].find("yesterday") != std::string::npos);
		}

		TEST_METHOD(InvalidJson_IsNotSuccessful) {
			auto response = systems(200, R"({"items": [)");

			Assert::IsFalse(response.isSuccessful());
			Assert::IsTrue(response.getParseErrors()[0].find("Invalid JSON: ") == 0);
		}

		TEST_METHOD(Redirect_EmptyBody_IsNotSuccessful) {
			// An OpenSensorHub node redirects an unknown ID to its web UI
			auto response = systems(302, "");

			Assert::IsFalse(response.isSuccessful());
			Assert::IsTrue(response.hasParseErrors());
		}

		TEST_METHOD(HttpError_JsonBody_IsNotParsed) {
			auto response = systems(404, R"({"status":404,"message":"Not found"})");

			Assert::IsFalse(response.isSuccessful());
			Assert::IsFalse(response.hasParseErrors());
			Assert::AreEqual(404, response.getResponseCode());
		}

		TEST_METHOD(Void_UsesHttpStatusOnly) {
			Assert::IsTrue(APIResponse<void>(201, "", "", {}).isSuccessful());
			Assert::IsFalse(APIResponse<void>(500, "", "", {}).isSuccessful());
		}
	};
}
