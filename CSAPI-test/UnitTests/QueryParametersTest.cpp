#include "pch.h"
#include "CppUnitTest.h"
#include "ConnectedSystemsAPI.h"

#include <optional>
#include <string>
#include <thread>
#include <vector>
#include <httplib.h>

#include "DataModels/TimeExtent.h"
#include "Query/DataStreamsQuery.h"
#include "Query/ObservationsOfDataStreamQuery.h"
#include "Query/SystemsQuery.h"
#include "Util/TimeUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ConnectedSystemsAPI;
using namespace ConnectedSystemsAPI::DataModels;
using namespace ConnectedSystemsAPI::Query;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(QueryParametersTest)
	{
		using TimePoint = std::chrono::system_clock::time_point;

		static constexpr const char* START = "2026-01-02T03:04:05.678Z";
		static constexpr const char* END = "2026-01-03T04:05:06.789Z";

		static TimePoint parse(const char* time) { return TimeUtils::parseTimeStringUTC(time); }

	public:
		TEST_METHOD(Empty_IsEmptyString) {
			Assert::AreEqual(""s, DataStreamsQuery().toString());
		}

		TEST_METHOD(TimeExtent_Range_IsStartSlashEnd) {
			auto query = DataStreamsQuery().setPhenomenonTime(TimeExtent(parse(START), parse(END)));
			Assert::AreEqual("?phenomenonTime="s + START + "/" + END, query.toString());
		}

		TEST_METHOD(TimeExtent_Instant_IsSingleTime) {
			auto query = DataStreamsQuery().setPhenomenonTime(TimeExtent(parse(START), parse(START)));
			Assert::AreEqual("?phenomenonTime="s + START, query.toString());
		}

		TEST_METHOD(TimeExtent_Now_IsNow) {
			auto query = SystemsQuery().setDatetime(TimeExtent());
			Assert::AreEqual("?datetime=now"s, query.toString());
		}

		TEST_METHOD(TimeExtent_UnboundedStart_IsDoubleDots) {
			auto query = DataStreamsQuery().setResultTime(TimeExtent((TimePoint::min)(), parse(END)));
			Assert::AreEqual("?resultTime=../"s + END, query.toString());
		}

		TEST_METHOD(Value_Space_IsEncoded) {
			Assert::AreEqual("?q=weather%20station"s, DataStreamsQuery().setQ("weather station").toString());
		}

		TEST_METHOD(Value_ReservedCharacters_AreEncoded) {
			Assert::AreEqual("?q=a%26b%3Dc%2Bd%23e%3Ff%25"s, DataStreamsQuery().setQ("a&b=c+d#e?f%").toString());
		}

		TEST_METHOD(Value_NonAscii_IsEncodedAsUtf8) {
			Assert::AreEqual("?q=caf%C3%A9"s, DataStreamsQuery().setQ("caf\xC3\xA9").toString());
		}

		TEST_METHOD(Value_CommaSeparatedList_KeepsCommas) {
			Assert::AreEqual("?q=gps,imu"s, DataStreamsQuery().setQ("gps,imu").toString());
		}

		TEST_METHOD(Value_UriWithColonsAndSlashes_IsReadable) {
			auto query = DataStreamsQuery().setObservedProperty("http://sensorml.com/ont/swe/property/Location");
			Assert::AreEqual("?observedProperty=http://sensorml.com/ont/swe/property/Location"s, query.toString());
		}

		TEST_METHOD(Value_Vector_IsJoinedWithCommas) {
			std::vector<std::string> systems = { "sys1", "sys 2" };
			Assert::AreEqual("?system=sys1,sys%202"s, DataStreamsQuery().setSystem(systems).toString());
		}

		TEST_METHOD(MultipleParameters_AreJoinedWithAmpersand) {
			auto query = DataStreamsQuery().setLimit(10).setQ("gps");
			Assert::AreEqual("?limit=10&q=gps"s, query.toString());
		}

		TEST_METHOD(Request_ServerDecodesOriginalValues) {
			httplib::Server server;
			std::string receivedFoi, receivedTime, receivedLimit;
			server.Get("/api/datastreams/ds1/observations", [&](const httplib::Request& req, httplib::Response& res) {
				receivedFoi = req.get_param_value("foi");
				receivedTime = req.get_param_value("phenomenonTime");
				receivedLimit = req.get_param_value("limit");
				res.set_content(R"({"items":[]})", "application/json");
			});

			int port = server.bind_to_any_port("127.0.0.1");
			std::thread serverThread([&] { server.listen_after_bind(); });
			server.wait_until_ready();

			ConSysAPI api("http://127.0.0.1:" + std::to_string(port) + "/api", "admin", "password");
			auto query = ObservationsOfDataStreamQuery()
				.setFoi("a&b=c +#d")
				.setPhenomenonTime(TimeExtent(parse(START), (TimePoint::max)()))
				.setLimit(5);
			auto response = api.getObservationsAPI().getObservationsOfDataStream("ds1", query);

			server.stop();
			serverThread.join();

			Assert::AreEqual(200, response.getResponseCode());
			Assert::AreEqual("a&b=c +#d"s, receivedFoi);
			Assert::AreEqual(std::string(START) + "/..", receivedTime);
			Assert::AreEqual("5"s, receivedLimit);
		}
	};
}
