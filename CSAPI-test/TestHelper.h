#pragma once

#include <iostream>
#include <string>
#include "pch.h"
#include "ConnectedSystemsAPI.h"
#include "RegistryInit.h"
#include "DataModels/DataStream.h"
#include "DataModels/DataStreamBuilder.h"
#include "DataModels/System.h"
#include "DataModels/SystemBuilder.h"
#include "DataModels/ObservationSchema.h"
#include "DataModels/ObservationSchemaBuilder.h"
#include "DataModels/Properties.h"
#include "DataModels/PropertiesBuilder.h"
#include "DataModels/Component/BooleanBuilder.h"
#include "DataModels/Component/DataRecordBuilder.h"

namespace CSAPItest {
	class TestHelper {
	public:
		static constexpr const char* TEST_SYSTEM_UID = "test-system-001";
		ConnectedSystemsAPI::ConSysAPI csapi{ "localhost:8282/sensorhub/api", "admin", "admin" };

		ConnectedSystemsAPI::APIResponse<void> createTestSystem() {
			auto system = ConnectedSystemsAPI::DataModels::SystemBuilder()
				.setProperties(ConnectedSystemsAPI::DataModels::PropertiesBuilder()
					.setFeatureType("osa:Sensor")
					.setUid(TEST_SYSTEM_UID)
					.setName("Test System 001")
					.setDescription("This is a test system created by CSAPI-test")
					.setAssetType("Equipment")
					.build())
				.build();
			return csapi.getSystemsAPI().createSystem(system);
		}

		ConnectedSystemsAPI::APIResponse<void> createTestSubsystem() {
			auto parentSystemId = getTestSystemId();
			auto subsystem = ConnectedSystemsAPI::DataModels::SystemBuilder()
				.setProperties(ConnectedSystemsAPI::DataModels::PropertiesBuilder()
					.setFeatureType("osa:Sensor")
					.setUid("test-subsystem-001")
					.setName("Test Subsystem 001")
					.setDescription("This is a test subsystem created by CSAPI-test")
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
				if (item.getProperties().getUid() == "test-subsystem-001") {
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

		ConnectedSystemsAPI::DataModels::DataStream createDataStream() {
			auto booleanField = ConnectedSystemsAPI::DataModels::Component::BooleanBuilder()
				.withType("Boolean")
				.withName("booleanField")
				.withDescription("This is a test boolean field")
				.build();

			auto resultSchemaPtr = std::make_unique<ConnectedSystemsAPI::DataModels::Component::DataRecord>(
				ConnectedSystemsAPI::DataModels::Component::DataRecordBuilder()
				.withType("DataRecord")
				.addField(std::make_unique<ConnectedSystemsAPI::DataModels::Component::Boolean>(booleanField))
				.build()
			);

			auto observationSchemaPtr = std::make_unique<ConnectedSystemsAPI::DataModels::ObservationSchema>(
				ConnectedSystemsAPI::DataModels::ObservationSchemaBuilder()
				.withObservationFormat("application/om+json")
				.withResultSchema(std::move(resultSchemaPtr))
				.build()
			);

			auto dataStream = ConnectedSystemsAPI::DataModels::DataStreamBuilder()
				.withName("Test DataStream 001")
				.withOutputName("test_output_001")
				.withDescription("This is a test data stream created by CSAPI-test")
				.withSchema(std::move(observationSchemaPtr))
				.build();

			return dataStream;
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
				std::cout << "ystem: " << item.getProperties().getName() << std::endl;
				auto subsystemsResponse = csapi.getSystemsAPI().getSubsystems(item.getId());
				auto subsystems = subsystemsResponse.getItems();
				for (const auto& subsystem : subsystems) {
					std::cout << "  Subsystem: " << subsystem.getProperties().getName() << std::endl;
				}
			}
		}
	};
}