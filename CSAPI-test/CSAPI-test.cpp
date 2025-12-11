#include <iostream>
#include <string>
#include "pch.h"
#include "CppUnitTest.h"
#include "ConnectedSystemsAPI.h"
#include "RegistryInit.h"
#include "DataModels/DataStream.h"
#include "DataModels/DataStreamBuilder.h"
#include "DataModels/System.h"
#include "DataModels/SystemBuilder.h"
#include "DataModels/ObservationBuilder.h"
#include "DataModels/ObservationSchema.h"
#include "DataModels/ObservationSchemaBuilder.h"
#include "DataModels/Properties.h"
#include "DataModels/PropertiesBuilder.h"
#include "DataModels/Component/BooleanBuilder.h"
#include "DataModels/Component/DataComponent.h"
#include "DataModels/Component/DataRecordBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(CSAPItest)
	{
	private:
		static constexpr const char* TEST_SYSTEM_UID = "test-system-001";
		ConnectedSystemsAPI::ConSysAPI csapi{ "localhost:8282/sensorhub/api"s, "admin"s, "admin"s };

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

		ConnectedSystemsAPI::DataModels::DataStream createDataStream() {
			auto booleanField = ConnectedSystemsAPI::DataModels::Component::BooleanBuilder()
				.withType("Boolean"s)
				.withName("booleanField"s)
				.withDescription("This is a test boolean field"s)
				.build();

			auto resultSchemaPtr = std::make_unique<ConnectedSystemsAPI::DataModels::Component::DataRecord>(
				ConnectedSystemsAPI::DataModels::Component::DataRecordBuilder()
				.withType("DataRecord")
				.addField(std::make_unique<ConnectedSystemsAPI::DataModels::Component::Boolean>(booleanField))
				.build()
			);

			auto observationSchemaPtr = std::make_unique<ConnectedSystemsAPI::DataModels::ObservationSchema>(
				ConnectedSystemsAPI::DataModels::ObservationSchemaBuilder()
				.withObservationFormat("application/om+json"s)
				.withResultSchema(std::move(resultSchemaPtr))
				.build()
			);

			auto dataStream = ConnectedSystemsAPI::DataModels::DataStreamBuilder()
				.withName("Test DataStream 001"s)
				.withOutputName("test_output_001"s)
				.withDescription("This is a test data stream created by CSAPI-test"s)
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
				std::cout << "System: " << item.getProperties().getName() << std::endl;
				auto subsystemsResponse = csapi.getSystemsAPI().getSubsystems(item.getId());
				auto subsystems = subsystemsResponse.getItems();
				for (const auto& subsystem : subsystems) {
					std::cout << "  Subsystem: " << subsystem.getProperties().getName() << std::endl;
				}
			}
		}

	public:
		/// <summary>
		/// Remove any existing test system after each test method.
		/// </summary>
		TEST_METHOD_CLEANUP(MethodCleanup) {
			cleanupTestSystem();
		}

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

		TEST_METHOD(GetDataStreams) {
			auto response = csapi.getDataStreamsAPI().getDataStreams();
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(GetDataStreamById) {
			auto response = csapi.getDataStreamsAPI().getDataStreams();
			std::string id = response.getItems().at(0).getId().value_or("");
			auto response2 = csapi.getDataStreamsAPI().getDataStreamById(id);
			Assert::IsTrue(response2.isSuccessful());
		}

		TEST_METHOD(GetObservationSchema) {
			auto response = csapi.getDataStreamsAPI().getDataStreams();
			std::string id = response.getItems().at(0).getId().value_or("");

			auto response2 = csapi.getDataStreamsAPI().getObservationSchema(id);
			Assert::IsTrue(response2.isSuccessful());
		}

		TEST_METHOD(CreateDataStream) {
			createTestSystem();
			std::string systemId = getTestSystemId();
			auto dataStream = createDataStream();
			auto response = csapi.getDataStreamsAPI().createDataStream(systemId, dataStream);
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(DeleteDataStream) {
			createTestSystem();
			std::string systemId = getTestSystemId();
			auto dataStream = createDataStream();
			auto dsId = dataStream.getId().value_or("");
			csapi.getDataStreamsAPI().deleteDataStream(dsId, true);
		}

		TEST_METHOD(GetObservations) {
			auto response = csapi.getObservationsAPI().getObservations();
			Assert::IsTrue(response.isSuccessful());

			std::cout << "Observations Response: " << response.getResponseBody() << std::endl;

			for (const auto& obs : response.getItems()) {
				std::cout << "Observation ID: " << obs.toJson().dump(2) << std::endl;
			}
		}

		TEST_METHOD(GetObservationsOfDataStream) {
			auto dataStreamsResponse = csapi.getDataStreamsAPI().getDataStreams();
			std::string dataStreamId = dataStreamsResponse.getItems().at(0).getId().value_or("");
			auto response = csapi.getObservationsAPI().getObservationsOfDataStream(dataStreamId);
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(CreateObservation) {
			createTestSystem();
			std::string systemId = getTestSystemId();
			auto dataStream = createDataStream();
			auto dataStreamCreateResponse = csapi.getDataStreamsAPI().createDataStream(systemId, dataStream);
			auto dataStreamGetResponse = csapi.getDataStreamsAPI().getDataStreams();

			// First create a datastreams
			std::string dataStreamId;
			for (const auto& ds : dataStreamGetResponse.getItems()) {
				if (ds.getName().value_or("") == dataStream.getName().value_or("")) {
					dataStreamId = ds.getId().value_or("");
					break;
				}
			}
			assert(!dataStreamId.empty());

			//Get the schema to know what kind of observation to create
			auto schemaResponse = csapi.getDataStreamsAPI().getObservationSchema(dataStreamId);
			Assert::IsTrue(schemaResponse.isSuccessful());
			auto schema = schemaResponse.getItems().at(0).getResultSchema();
			auto schemaDataRecord = dynamic_cast<const ConnectedSystemsAPI::DataModels::Component::DataRecord*>(schema);
			Assert::IsNotNull(schemaDataRecord);
			// Create a data block according to the schema
			auto dataBlock = schemaDataRecord->createDataBlock();
			dataBlock.setField("booleanField", ConnectedSystemsAPI::DataModels::Data::DataValue(true));

			auto now = ConnectedSystemsAPI::DataModels::TimeInstant(std::chrono::system_clock::now());

			// Then create an observation for that datastream
			auto observation = ConnectedSystemsAPI::DataModels::ObservationBuilder()
				.withResultTime(now)
				.withResult(dataBlock)
				.build();
			// Print out the observation JSON
			std::cout << "Observation to Create: " << observation.toJson().dump(2) << std::endl;

			auto observationCreateResponse = csapi.getObservationsAPI().createObservation(dataStreamId, observation);
			std::cout << "Create Observation Response Code: " << observationCreateResponse.getResponseCode() << std::endl;
			Assert::IsTrue(observationCreateResponse.isSuccessful());

			// Get the observations to verify
			auto observationsResponse = csapi.getObservationsAPI().getObservationsOfDataStream(dataStreamId);
			Assert::IsTrue(observationsResponse.isSuccessful());
			Assert::IsTrue(observationsResponse.getItems().size() > 0);
			//Verify the data stream id matches
			Assert::AreEqual(dataStreamId, observationsResponse.getItems().at(0).getDataStreamId().value_or(""));
			//Verify the result matches
			const auto* resultValue = observationsResponse.getItems().at(0).getResult().getField("booleanField");
			Assert::IsNotNull(resultValue);
			Assert::IsTrue(std::holds_alternative<bool>(resultValue->value));
		}

		TEST_METHOD(CreateObservation2) {
			// Same as above, but instead of creating a new datastream, use an existing one
			auto dataStreamsResponse = csapi.getDataStreamsAPI().getDataStreams();
			std::string dataStreamId = dataStreamsResponse.getItems().at(0).getId().value_or("");
			assert(!dataStreamId.empty());

			//Get the schema to know what kind of observation to create
			auto schemaResponse = csapi.getDataStreamsAPI().getObservationSchema(dataStreamId);
			Assert::IsTrue(schemaResponse.isSuccessful());
			auto schema = schemaResponse.getItems().at(0).getResultSchema();
			auto schemaDataRecord = dynamic_cast<const ConnectedSystemsAPI::DataModels::Component::DataRecord*>(schema);
			Assert::IsNotNull(schemaDataRecord);

			// Create a data block according to the schema
			auto dataBlock = schemaDataRecord->createDataBlock();
			if (dataBlock.hasField("data")) {
				// This field exists in the default datastream created by the OSH node example project
				dataBlock.setField("data", ConnectedSystemsAPI::DataModels::Data::DataValue("Test string data"));
			}
			else {
				Assert::Fail(L"Schema does not have 'data' field");
			}

			auto now = ConnectedSystemsAPI::DataModels::TimeInstant(std::chrono::system_clock::now());
			// Then create an observation for that datastream
			auto observation = ConnectedSystemsAPI::DataModels::ObservationBuilder()
				.withResultTime(now)
				.withResult(dataBlock)
				.build();
			auto observationCreateResponse = csapi.getObservationsAPI().createObservation(dataStreamId, observation);
			Assert::IsTrue(observationCreateResponse.isSuccessful());

			// Get the observations to verify
			auto observationsResponse = csapi.getObservationsAPI().getObservationsOfDataStream(dataStreamId);
			Assert::IsTrue(observationsResponse.isSuccessful());
			Assert::IsTrue(observationsResponse.getItems().size() > 0);
			Assert::AreEqual(dataStreamId, observationsResponse.getItems().at(0).getDataStreamId().value_or(""));

			//Verify the result matches
			const auto* resultValue = observationsResponse.getItems().at(0).getResult().getField("data");
			Assert::IsNotNull(resultValue);
			std::cout << "Result value type index: " << resultValue->value.index() << std::endl;
			Assert::IsTrue(std::holds_alternative<std::string>(resultValue->value));
			std::cout << "Result value: " << std::get<std::string>(resultValue->value) << std::endl;
		}
	};
}