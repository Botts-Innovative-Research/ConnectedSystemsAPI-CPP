#include <iostream>
#include <string>
#include "pch.h"
#include "CppUnitTest.h"
#include "DataModels/DataStream.h"
#include "DataModels/System.h"
#include "DataModels/SystemBuilder.h"
#include "DataModels/Properties.h"
#include "DataModels/PropertiesBuilder.h"
#include "ConnectedSystemsAPI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(CSAPItest)
	{
	private:
		static constexpr const char* TEST_SYSTEM_UID = "test-system-001";
		ConnectedSystemsAPI::ConSysAPI csapi{ "localhost:8181/sensorhub/api"s, "admin"s, "admin"s };

		ConnectedSystemsAPI::APIResponse<void> createTestSystem() {
			auto system = ConnectedSystemsAPI::DataModels::SystemBuilder()
				.setProperties(ConnectedSystemsAPI::DataModels::PropertiesBuilder()
					.setFeatureType("sosa:Sensor"s)
					.setUid(TEST_SYSTEM_UID)
					.setName("Test System 001"s)
					.setDescription("This is a test system created by CSAPI-test"s)
					.setAssetType("Equipment"s)
					.build())
				.build();
			return csapi.getSystemsAPI().createSystem(system);
		}

		ConnectedSystemsAPI::APIResponse<void> createTestSubsystem() {
			auto parentSystemId = getTestSystemId();
			auto subsystem = ConnectedSystemsAPI::DataModels::SystemBuilder()
				.setProperties(ConnectedSystemsAPI::DataModels::PropertiesBuilder()
					.setFeatureType("sosa:Sensor"s)
					.setUid("test-subsystem-001"s)
					.setName("Test Subsystem 001"s)
					.setDescription("This is a test subsystem created by CSAPI-test"s)
					.build())
				.build();
			return csapi.getSystemsAPI().createSubsystem(parentSystemId, subsystem);
		}

		std::string getTestSystemId() {
			auto response = csapi.getSystemsAPI().getSystems();
			for (const auto& item : response.getItems()) {
				if (item.getProperties().getUid() == TEST_SYSTEM_UID) {
					return item.getId();
				}
			}
			return "";
		}

		std::string getTestSubsystemId() {
			auto response = csapi.getSystemsAPI().getSubsystems(getTestSystemId());
			for (const auto& item : response.getItems()) {
				if (item.getProperties().getUid() == "test-subsystem-001"s) {
					return item.getId();
				}
			}
			return "";
		}

		void cleanupTestSystem() {
			// First delete the subsystem
			std::string subsystemId = getTestSubsystemId();
			if (!subsystemId.empty()) {
				csapi.getSystemsAPI().deleteSystem(subsystemId, true);
			}

			std::string systemId = getTestSystemId();
			if (!systemId.empty()) {
				csapi.getSystemsAPI().deleteSystem(systemId, true);
			}

			// Verify deletion
			systemId = getTestSystemId();
			if (!systemId.empty()) {
				std::cout << "Failed to clean up test system." << std::endl;
			}
		}

		//Debug stuff
		template<typename T>
		void printResponse(const ConnectedSystemsAPI::APIResponse<T>& response) {
			std::cout << "Response Code: " << response.getResponseCode() << std::endl;
			std::cout << "Response Message: " << response.getResponseMessage() << std::endl;
			std::cout << "Headers: " << std::endl;
			for (const auto& header : response.getHeaders()) {
				std::cout << "  " << header.first << ": ";
				for (const auto& value : header.second) {
					std::cout << value << " ";
				}
				std::cout << std::endl;
			}
			std::cout << "Response Body: " << response.getResponseBody() << std::endl;
		}

		void printSystems() {
			auto response = csapi.getSystemsAPI().getSystems();
			auto items = response.getItems();
			for (const auto& item : items) {
				std::cout << "System: " << item.getProperties().getName() << std::endl;
				auto subsystemsResponse = csapi.getSystemsAPI().getSubsystems(item.getId());
				auto subsystems = subsystemsResponse.getItems();
				for (const auto& subsystem : subsystems) {
					std::cout << "  Subsystem: " << subsystem.getProperties().getName() << std::endl;
				}
			}
		}

	public:
		TEST_METHOD(GetSystems) {
			createTestSystem();
			auto response = csapi.getSystemsAPI().getSystems();
			auto items = response.getItems();
			Assert::IsTrue(items.size() > 0);
		}

		TEST_METHOD(GetSubsystems) {
			createTestSystem();
			createTestSubsystem();
			auto response = csapi.getSystemsAPI().getSubsystems(getTestSystemId());
			Assert::IsTrue(response.isSuccessful());
			Assert::IsTrue(response.getItems().size() > 0);
		}

		TEST_METHOD(GetSystemById) {
			createTestSystem();
			auto response = csapi.getSystemsAPI().getSystemById(getTestSystemId());
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(CreateSystem) {
			auto response = createTestSystem();
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(CreateSubsystem) {
			createTestSystem();
			auto response = createTestSubsystem();
			printSystems();
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(UpdateSystem) {
			createTestSystem();

			auto system = ConnectedSystemsAPI::DataModels::SystemBuilder()
				.setProperties(ConnectedSystemsAPI::DataModels::PropertiesBuilder()
					.setFeatureType("sosa:Sensor"s)
					.setUid("test-system-001"s)
					.setName("Test System 001 - Updated"s)
					.setDescription("This is a test system created by CSAPI-test - Updated"s)
					.setAssetType("Equipment"s)
					.build())
				.build();

			std::string systemId = getTestSystemId();

			auto response = csapi.getSystemsAPI().updateSystem(systemId, system);
			Assert::IsTrue(response.isSuccessful());

			auto updatedSystem = csapi.getSystemsAPI().getSystemById(systemId);
			Assert::IsTrue(updatedSystem.isSuccessful());
			Assert::AreEqual("Test System 001 - Updated"s, updatedSystem.getItems().at(0).getProperties().getName());
			Assert::AreEqual("This is a test system created by CSAPI-test - Updated"s, updatedSystem.getItems().at(0).getProperties().getDescription());
		}

		TEST_METHOD(DeleteSystem) {
			createTestSystem();

			auto response = csapi.getSystemsAPI().deleteSystem(getTestSystemId());
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD_CLEANUP(MethodCleanup) {
			cleanupTestSystem();
		}

		TEST_METHOD(GetDataStreams) {
			auto response = csapi.getDataStreamsAPI().getSystems();
			std::cout << "Response: " << response.getResponseBody() << std::endl;
			Assert::IsTrue(response.isSuccessful());
			std::cout << "DataStream: " << response.getItems().at(0) << std::endl;
		}
	};
}
