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
	private:
		TestHelper testHelper;
	public:
		TEST_METHOD_INITIALIZE(ClassInitialize) {
			testHelper = TestHelper();
		}

		TEST_METHOD_CLEANUP(MethodCleanup) {
			testHelper.cleanupTestSystem();
		}

		TEST_METHOD(GetObservations) {
			auto response = testHelper.csapi.getObservationsAPI().getObservations();
			Assert::IsTrue(response.isSuccessful());

			std::cout << "Observations Response: " << response.getResponseBody() << std::endl;

			for (const auto& obs : response.getItems()) {
				std::cout << "Observation ID: " << obs.toJson().dump(2) << std::endl;
			}
		}

		TEST_METHOD(GetObservationsOfDataStream) {
			auto dataStreamsResponse = testHelper.csapi.getDataStreamsAPI().getDataStreams();
			std::string dataStreamId = dataStreamsResponse.getItems().at(0).getId().value_or("");
			auto response = testHelper.csapi.getObservationsAPI().getObservationsOfDataStream(dataStreamId);
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(CreateObservation) {
			testHelper.createTestSystem();
			std::string systemId = testHelper.getTestSystemId();
			auto dataStream = testHelper.createDataStream();
			auto dataStreamCreateResponse = testHelper.csapi.getDataStreamsAPI().createDataStream(systemId, dataStream);
			auto dataStreamGetResponse = testHelper.csapi.getDataStreamsAPI().getDataStreams();

			// First create a datastream
			std::string dataStreamId;
			for (const auto& ds : dataStreamGetResponse.getItems()) {
				if (ds.getName().value_or("") == dataStream.getName().value_or("")) {
					dataStreamId = ds.getId().value_or("");
					break;
				}
			}
			assert(!dataStreamId.empty());

			//Get the schema to know what kind of observation to create
			auto schemaResponse = testHelper.csapi.getDataStreamsAPI().getObservationSchema(dataStreamId);
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

			auto observationCreateResponse = testHelper.csapi.getObservationsAPI().createObservation(dataStreamId, observation);
			std::cout << "Create Observation Response Code: " << observationCreateResponse.getResponseCode() << std::endl;
			Assert::IsTrue(observationCreateResponse.isSuccessful());

			// Get the observations to verify
			auto observationsResponse = testHelper.csapi.getObservationsAPI().getObservationsOfDataStream(dataStreamId);
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