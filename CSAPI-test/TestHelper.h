#pragma once

#include <iostream>
#include <string>
#include "pch.h"
#include "ConnectedSystemsAPI.h"
#include "RegistryInit.h"
#include "DataModels/DataStream.h"
#include "DataModels/DataStreamBuilder.h"
#include "DataModels/ControlStream.h"
#include "DataModels/ControlStreamBuilder.h"
#include "DataModels/CommandSchema.h"
#include "DataModels/CommandSchemaBuilder.h"
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

		std::string createTestSystem() {
			auto system = ConnectedSystemsAPI::DataModels::SystemBuilder()
				.withProperties(ConnectedSystemsAPI::DataModels::PropertiesBuilder()
					.withFeatureType("osa:Sensor")
					.withUid(TEST_SYSTEM_UID)
					.withName("Test System 001")
					.withDescription("This is a test system created by CSAPI-test")
					.withAssetType("Equipment")
					.build())
				.build();
			csapi.getSystemsAPI().createSystem(system);
			return getTestSystemId();
		}

		std::string createTestSubsystem() {
			auto parentSystemId = getTestSystemId();
			auto subsystem = ConnectedSystemsAPI::DataModels::SystemBuilder()
				.withProperties(ConnectedSystemsAPI::DataModels::PropertiesBuilder()
					.withFeatureType("osa:Sensor")
					.withUid("test-subsystem-001")
					.withName("Test Subsystem 001")
					.withDescription("This is a test subsystem created by CSAPI-test")
					.build())
				.build();
			csapi.getSystemsAPI().createSubsystem(parentSystemId, subsystem);
			return getTestSubsystemId();
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
			std::string systemId = getTestSystemId();
			if (systemId.empty()) return "";

			auto response = csapi.getSystemsAPI().getSubsystems(systemId);
			for (const auto& item : response.getItems()) {
				if (item.getProperties().getUid() == "test-subsystem-001") {
					return item.getId();
				}
			}
			return "";
		}

		void cleanupTestSystem() {
			std::string systemId = getTestSystemId();
			std::string subsystemId = getTestSubsystemId();
			if (!systemId.empty()) {
				if (!subsystemId.empty())
					csapi.getSystemsAPI().deleteSystem(subsystemId, true);
				csapi.getSystemsAPI().deleteSystem(systemId, true);
			}

			// Verify deletion
			systemId = getTestSystemId();
			if (!systemId.empty()) {
				std::cout << "Failed to clean up test system." << std::endl;
			}
		}

		std::string createTestDataStream(const std::string& systemId) {
			auto dataStream = createTestDataStreamObject();
			auto dataStreamCreateResponse = csapi.getDataStreamsAPI().createDataStream(systemId, dataStream);
			return getTestDataStreamIdByName(dataStream.getName().value_or(""));
		}

		ConnectedSystemsAPI::DataModels::DataStream createTestDataStreamObject() {
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

		std::string getTestDataStreamIdByName(const std::string& name) {
			auto response = csapi.getDataStreamsAPI().getDataStreams();
			for (const auto& item : response.getItems()) {
				if (item.getName().value_or("") == name) {
					return item.getId().value_or("");
				}
			}
			return "";
		}

		std::string createTestControlStream(const std::string& systemId) {
			auto controlStream = createTestControlStreamObject();
			auto controlStreamCreateResponse = csapi.getControlStreamsAPI().createControlStream(systemId, controlStream);
			return getTestControlStreamIdByName(controlStream.getName().value_or(""));
		}

		ConnectedSystemsAPI::DataModels::ControlStream createTestControlStreamObject() {
			auto booleanParamField = ConnectedSystemsAPI::DataModels::Component::BooleanBuilder()
				.withType("Boolean")
				.withName("booleanParam")
				.withDescription("This is a test boolean parameter field")
				.build();

			auto parametersSchemaPtr = std::make_unique<ConnectedSystemsAPI::DataModels::Component::DataRecord>(
				ConnectedSystemsAPI::DataModels::Component::DataRecordBuilder()
				.withType("DataRecord")
				.addField(std::make_unique<ConnectedSystemsAPI::DataModels::Component::Boolean>(booleanParamField))
				.build()
			);

			auto booleanResultField = ConnectedSystemsAPI::DataModels::Component::BooleanBuilder()
				.withType("Boolean")
				.withName("booleanResult")
				.withDescription("This is a test boolean result field")
				.build();

			auto resultSchemaPtr = std::make_unique<ConnectedSystemsAPI::DataModels::Component::DataRecord>(
				ConnectedSystemsAPI::DataModels::Component::DataRecordBuilder()
				.withType("DataRecord")
				.addField(std::make_unique<ConnectedSystemsAPI::DataModels::Component::Boolean>(booleanResultField))
				.build()
			);

			auto commandSchemaPtr = std::make_unique<ConnectedSystemsAPI::DataModels::CommandSchema>(
				ConnectedSystemsAPI::DataModels::CommandSchemaBuilder()
				.withCommandFormat("application/swe+json")
				.withParametersSchema(std::move(parametersSchemaPtr))
				.withResultSchema(std::move(resultSchemaPtr))
				.build()
			);

			auto controlStream = ConnectedSystemsAPI::DataModels::ControlStreamBuilder()
				.withName("Test ControlStream 001")
				.withInputName("test_input_001")
				.withDescription("This is a test control stream created by CSAPI-test")
				.withSchema(std::move(commandSchemaPtr))
				.build();

			return controlStream;
		}

		std::string getTestControlStreamIdByName(const std::string& name) {
			auto response = csapi.getControlStreamsAPI().getControlStreams();
			for (const auto& item : response.getItems()) {
				if (item.getName().value_or("") == name) {
					return item.getId().value_or("");
				}
			}
			return "";
		}
	};
}