#include <string>

#include "pch.h"
#include "CppUnitTest.h"
#include "ConnectedSystemsAPI.h"
#include "../TestHelper.h"
#include "DataModels/ControlStream.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(ControlStreamsTest)
	{
		TestHelper testHelper;

		TEST_METHOD_INITIALIZE(ClassInitialize) {
			testHelper = TestHelper();
		}

		TEST_METHOD_CLEANUP(MethodCleanup) {
			testHelper.cleanupTestSystem();
		}

		TEST_METHOD(GetControlStreams) {
			auto response = testHelper.csapi.getControlStreamsAPI().getControlStreams();
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(GetControlStreamsOfSystem) {
			auto systemsResponse = testHelper.csapi.getSystemsAPI().getSystems();
			Assert::IsTrue(systemsResponse.isSuccessful());
			Assert::IsFalse(systemsResponse.getItems().empty());
			auto systemId = systemsResponse.getItems().front().getId();
			auto response = testHelper.csapi.getControlStreamsAPI().getControlStreamsOfSystem(systemId);
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(GetControlStreamsById) {
			auto controlStreamsResponse = testHelper.csapi.getControlStreamsAPI().getControlStreams();
			Assert::IsTrue(controlStreamsResponse.isSuccessful());
			Assert::IsFalse(controlStreamsResponse.getItems().empty());
			auto controlStreamId = controlStreamsResponse.getItems().front().getId().value_or("");
			auto response = testHelper.csapi.getControlStreamsAPI().getControlStreamById(controlStreamId);
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(GetControlStreamSchema) {
			auto controlStreamsResponse = testHelper.csapi.getControlStreamsAPI().getControlStreams();
			Assert::IsTrue(controlStreamsResponse.isSuccessful());
			Assert::IsFalse(controlStreamsResponse.getItems().empty());
			auto controlStreamId = controlStreamsResponse.getItems().front().getId().value_or("");
			auto response = testHelper.csapi.getControlStreamsAPI().getControlStreamSchema(controlStreamId);
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(CreateControlStream) {
			auto systemId = testHelper.createTestSystem();
			auto controlStream = testHelper.createTestControlStreamObject();
			std::cout << "Creating Control Stream: " << controlStream << std::endl;
			auto response = testHelper.csapi.getControlStreamsAPI().createControlStream(systemId, controlStream);
			std::cout << "Created Control Stream Response: " << response.getResponseMessage() << std::endl;
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(DeleteControlStream) {
			auto systemId = testHelper.createTestSystem();
			auto controlStreamId = testHelper.createTestControlStream(systemId);
			testHelper.csapi.getControlStreamsAPI().deleteControlStream(controlStreamId, true);
		}
	};
}