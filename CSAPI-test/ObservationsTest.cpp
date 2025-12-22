#include <iostream>
#include <string>

#include "pch.h"
#include "CppUnitTest.h"
#include "ConnectedSystemsAPI.h"
#include "TestHelper.h"
#include "DataModels/DataStream.h"
#include "DataModels/ObservationBuilder.h"
#include "DataModels/ObservationSchema.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(ObservationsTest)
	{
		TestHelper testHelper;

		ConnectedSystemsAPI::DataModels::Data::DataBlockMixed createTestDataBlock(const std::string& dataStreamId) {
			//Get the schema to know what kind of observation to create
			auto schemaResponse = testHelper.csapi.getDataStreamsAPI().getObservationSchema(dataStreamId);
			auto schema = schemaResponse.getItems().at(0).getResultSchema();
			auto schemaDataRecord = dynamic_cast<const ConnectedSystemsAPI::DataModels::Component::DataRecord*>(schema);
			// Create a data block according to the schema
			ConnectedSystemsAPI::DataModels::Data::DataBlockMixed dataBlock = schemaDataRecord->createDataBlock();
			return dataBlock;
		}

		ConnectedSystemsAPI::DataModels::Observation pushTestObservation(const std::string& dataStreamId, const ConnectedSystemsAPI::DataModels::Data::DataBlockMixed& dataBlock) {
			auto now = ConnectedSystemsAPI::DataModels::TimeInstant(std::chrono::system_clock::now());
			auto observation = ConnectedSystemsAPI::DataModels::ObservationBuilder()
				.withResultTime(now)
				.withResult(dataBlock)
				.build();
			testHelper.csapi.getObservationsAPI().createObservation(dataStreamId, observation);
			return getTestObservation(dataStreamId);
		}

		ConnectedSystemsAPI::DataModels::Observation getTestObservation(const std::string& dataStreamId) {
			auto observationsResponse = testHelper.csapi.getObservationsAPI().getObservationsOfDataStream(dataStreamId);
			Assert::IsTrue(observationsResponse.isSuccessful());
			Assert::IsFalse(observationsResponse.getItems().empty());
			return observationsResponse.getItems().at(0);
		}

		TEST_METHOD_INITIALIZE(ClassInitialize) {
			testHelper = TestHelper();
		}

		TEST_METHOD_CLEANUP(MethodCleanup) {
			testHelper.cleanupTestSystem();
		}

		TEST_METHOD(GetObservations) {
			auto response = testHelper.csapi.getObservationsAPI().getObservations();
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(GetObservationsOfDataStream) {
			auto dataStreamsResponse = testHelper.csapi.getDataStreamsAPI().getDataStreams();
			std::string dataStreamId = dataStreamsResponse.getItems().at(0).getId().value_or("");
			auto response = testHelper.csapi.getObservationsAPI().getObservationsOfDataStream(dataStreamId);
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(CreateObservation) {
			auto systemId = testHelper.createTestSystem();
			auto dataStreamId = testHelper.createTestDataStream(systemId);
			auto dataBlock = createTestDataBlock(dataStreamId);

			dataBlock.setField("booleanField", ConnectedSystemsAPI::DataModels::Data::DataValue(true));
			auto observation = pushTestObservation(dataStreamId, dataBlock);

			//Verify the data stream id matches
			Assert::AreEqual(dataStreamId, observation.getDataStreamId().value_or(""));
			//Verify the result matches
			const auto* resultValue = observation.getResult().getField("booleanField");
			Assert::IsNotNull(resultValue);
			Assert::IsTrue(std::holds_alternative<bool>(resultValue->value));
		}

		TEST_METHOD(CreateObservation2) {
			// Same as above, but instead of creating a new datastream, use an existing one
			auto dataStreamsResponse = testHelper.csapi.getDataStreamsAPI().getDataStreams();
			std::string dataStreamId = dataStreamsResponse.getItems().at(0).getId().value_or("");
			assert(!dataStreamId.empty());

			//Get the schema to know what kind of observation to create
			auto schemaResponse = testHelper.csapi.getDataStreamsAPI().getObservationSchema(dataStreamId);
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
			auto observationCreateResponse = testHelper.csapi.getObservationsAPI().createObservation(dataStreamId, observation);
			Assert::IsTrue(observationCreateResponse.isSuccessful());

			// Get the observations to verify
			auto observationsResponse = testHelper.csapi.getObservationsAPI().getObservationsOfDataStream(dataStreamId);
			Assert::IsTrue(observationsResponse.isSuccessful());
			Assert::IsFalse(observationsResponse.getItems().empty());
			Assert::AreEqual(dataStreamId, observationsResponse.getItems().at(0).getDataStreamId().value_or(""));

			//Verify the result matches
			const auto* resultValue = observationsResponse.getItems().at(0).getResult().getField("data");
			Assert::IsNotNull(resultValue);
			Assert::IsTrue(std::holds_alternative<std::string>(resultValue->value));
		}

		TEST_METHOD(GetObservationById) {
			auto systemId = testHelper.createTestSystem();
			auto dataStreamId = testHelper.createTestDataStream(systemId);
			auto dataBlock = createTestDataBlock(dataStreamId);
			auto observation = pushTestObservation(dataStreamId, dataBlock);
			auto observationId = observation.getId().value_or("");

			auto getResponse = testHelper.csapi.getObservationsAPI().getObservationById(observationId);
			Assert::IsTrue(getResponse.isSuccessful());
			Assert::AreEqual(observationId, getResponse.getItems().at(0).getId().value_or(""));
		}

		TEST_METHOD(DeleteObservation) {
			auto systemId = testHelper.createTestSystem();
			auto dataStreamId = testHelper.createTestDataStream(systemId);
			auto dataBlock = createTestDataBlock(dataStreamId);
			auto observation = pushTestObservation(dataStreamId, dataBlock);
			auto observationId = observation.getId().value_or("");

			auto deleteResponse = testHelper.csapi.getObservationsAPI().deleteObservation(observationId);
			Assert::IsTrue(deleteResponse.isSuccessful());

			auto getResponse = testHelper.csapi.getObservationsAPI().getObservationById(observationId);
			Assert::IsFalse(getResponse.isSuccessful());
		}
	};
}