#include <string>

#include "pch.h"
#include "CppUnitTest.h"
#include "ConnectedSystemsAPI.h"
#include "../TestHelper.h"
#include "DataModels/System.h"
#include "DataModels/SystemBuilder.h"
#include "DataModels/Properties.h"
#include "DataModels/PropertiesBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(SystemsTest)
	{
		TestHelper testHelper;

		TEST_METHOD_INITIALIZE(ClassInitialize) {
			testHelper = TestHelper();
		}

		TEST_METHOD_CLEANUP(MethodCleanup) {
			testHelper.cleanupTestSystem();
		}

		TEST_METHOD(GetSystems) {
			testHelper.createTestSystem();
			auto response = testHelper.csapi.getSystemsAPI().getSystems();
			auto const& items = response.getItems();
			Assert::IsFalse(items.empty());
		}

		TEST_METHOD(GetSubsystems) {
			testHelper.createTestSystem();
			testHelper.createTestSubsystem();
			auto response = testHelper.csapi.getSystemsAPI().getSubsystems(testHelper.getTestSystemId());
			Assert::IsTrue(response.isSuccessful());
			Assert::IsFalse(response.getItems().empty());
		}

		TEST_METHOD(GetSystemById) {
			testHelper.createTestSystem();
			auto response = testHelper.csapi.getSystemsAPI().getSystemById(testHelper.getTestSystemId());
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(CreateSystem) {
			auto systemId = testHelper.createTestSystem();
			Assert::IsFalse(systemId.empty());
		}

		TEST_METHOD(CreateSubsystem) {
			testHelper.createTestSystem();
			auto subsystemId = testHelper.createTestSubsystem();
			Assert::IsFalse(subsystemId.empty());
		}

		TEST_METHOD(UpdateSystem) {
			testHelper.createTestSystem();

			auto system = ConnectedSystemsAPI::DataModels::SystemBuilder()
				.withProperties(ConnectedSystemsAPI::DataModels::PropertiesBuilder()
					.withFeatureType("sosa:Sensor"s)
					.withUid("test-system-001"s)
					.withName("Test System 001 - Updated"s)
					.withDescription("This is a test system created by CSAPI-test - Updated"s)
					.withAssetType("Equipment"s)
					.build())
				.build();

			std::string systemId = testHelper.getTestSystemId();

			auto response = testHelper.csapi.getSystemsAPI().updateSystem(systemId, system);
			Assert::IsTrue(response.isSuccessful());

			auto updatedSystem = testHelper.csapi.getSystemsAPI().getSystemById(systemId);
			Assert::IsTrue(updatedSystem.isSuccessful());
			Assert::AreEqual("Test System 001 - Updated"s, updatedSystem.getItems().at(0).getProperties().getName());
			Assert::AreEqual("This is a test system created by CSAPI-test - Updated"s, updatedSystem.getItems().at(0).getProperties().getDescription());
		}

		TEST_METHOD(DeleteSystem) {
			testHelper.createTestSystem();

			auto response = testHelper.csapi.getSystemsAPI().deleteSystem(testHelper.getTestSystemId());
			Assert::IsTrue(response.isSuccessful());
		}

	};
}