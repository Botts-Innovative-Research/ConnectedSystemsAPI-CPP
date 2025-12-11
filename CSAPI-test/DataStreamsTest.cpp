#include <string>

#include "pch.h"
#include "CppUnitTest.h"
#include "ConnectedSystemsAPI.h"
#include "TestHelper.h"
#include "DataModels/DataStream.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(DataStreamsTest)
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

		TEST_METHOD(GetDataStreams) {
			auto response = testHelper.csapi.getDataStreamsAPI().getDataStreams();
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(GetDataStreamById) {
			auto response = testHelper.csapi.getDataStreamsAPI().getDataStreams();
			std::string id = response.getItems().at(0).getId().value_or("");
			auto response2 = testHelper.csapi.getDataStreamsAPI().getDataStreamById(id);
			Assert::IsTrue(response2.isSuccessful());
		}

		TEST_METHOD(GetObservationSchema) {
			auto response = testHelper.csapi.getDataStreamsAPI().getDataStreams();
			std::string id = response.getItems().at(0).getId().value_or("");

			auto response2 = testHelper.csapi.getDataStreamsAPI().getObservationSchema(id);
			Assert::IsTrue(response2.isSuccessful());
		}

		TEST_METHOD(CreateDataStream) {
			testHelper.createTestSystem();
			std::string systemId = testHelper.getTestSystemId();
			auto dataStream = testHelper.createDataStream();
			auto response = testHelper.csapi.getDataStreamsAPI().createDataStream(systemId, dataStream);
			Assert::IsTrue(response.isSuccessful());
		}

		TEST_METHOD(DeleteDataStream) {
			testHelper.createTestSystem();
			std::string systemId = testHelper.getTestSystemId();
			auto dataStream = testHelper.createDataStream();
			auto dsId = dataStream.getId().value_or("");
			testHelper.csapi.getDataStreamsAPI().deleteDataStream(dsId, true);
		}
	};
}