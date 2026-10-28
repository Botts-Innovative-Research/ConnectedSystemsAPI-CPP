#include "pch.h"
#include "CppUnitTest.h"
#include "ConnectedSystemsAPI.h"

#include <chrono>
#include <optional>
#include <thread>
#include <httplib.h>

#include "DataModels/TimeExtent.h"
#include "Util/TimeUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ConnectedSystemsAPI;
using namespace ConnectedSystemsAPI::DataModels;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(TimeExtentTest)
	{
		using TimePoint = std::chrono::system_clock::time_point;

		static constexpr const char* START = "2026-01-02T03:04:05.678Z";
		static constexpr const char* END = "2026-01-03T04:05:06.789Z";

		static TimePoint parse(const char* time) { return TimeUtils::parseTimeStringUTC(time); }

	public:
		TEST_METHOD(Default_IsNow) {
			TimeExtent extent;

			Assert::IsTrue(extent.isNow());
			Assert::AreEqual("now"s, extent.toStringUTC());
		}

		TEST_METHOD(BoundedRange_IsNotNow) {
			TimeExtent extent(parse(START), parse(END));

			Assert::IsFalse(extent.isNow());
			Assert::AreEqual(std::string(START) + "/" + END, extent.toStringUTC());
		}

		TEST_METHOD(Instant_IsNotNow) {
			TimeExtent extent(parse(START), parse(START));

			Assert::IsFalse(extent.isNow());
			Assert::IsTrue(extent.isInstant());
			Assert::AreEqual(std::string(START), extent.toStringUTC());
		}

		TEST_METHOD(StartsNow_IsNotNow) {
			TimeExtent extent(std::nullopt, parse(END));

			Assert::IsFalse(extent.isNow());
			Assert::AreEqual("now/"s + END, extent.toStringUTC());
		}

		TEST_METHOD(EndsNow_IsNotNow) {
			TimeExtent extent(parse(START), std::nullopt);

			Assert::IsFalse(extent.isNow());
			Assert::AreEqual(std::string(START) + "/now", extent.toStringUTC());
		}

		TEST_METHOD(UnboundedStart_IsNotNow) {
			TimeExtent extent((TimePoint::min)(), parse(END));

			Assert::IsFalse(extent.isNow());
			Assert::AreEqual("../"s + END, extent.toStringUTC());
		}

		TEST_METHOD(FromJson_UnboundedStart_IsNotNow) {
			auto extent = nlohmann::json::parse(R"(["..", "2026-01-03T04:05:06.789Z"])").get<TimeExtent>();

			Assert::IsTrue(extent.isUnboundedStart());
			Assert::IsFalse(extent.startsNow());
			Assert::IsTrue(extent.getEnd() == parse(END));
		}

		TEST_METHOD(FromJson_UnboundedEnd_IsNotNow) {
			auto extent = nlohmann::json::parse(R"(["2026-01-02T03:04:05.678Z", ".."])").get<TimeExtent>();

			Assert::IsTrue(extent.getStart() == parse(START));
			Assert::IsTrue(extent.isUnboundedEnd());
			Assert::IsFalse(extent.endsNow());
		}

		TEST_METHOD(FromJson_NullEnd_IsUnbounded) {
			auto extent = nlohmann::json::parse(R"(["2026-01-02T03:04:05.678Z", null])").get<TimeExtent>();

			Assert::IsTrue(extent.isUnboundedEnd());
		}

		TEST_METHOD(FromJson_NowEnd_EndsNow) {
			// The form an OpenSensorHub node uses for validTime, here with 7 fractional digits
			auto extent = nlohmann::json::parse(R"(["2026-01-02T03:04:05.1234567Z", "now"])").get<TimeExtent>();

			Assert::IsTrue(extent.hasStart());
			Assert::IsTrue(extent.endsNow());
			Assert::IsFalse(extent.isUnboundedEnd());
		}

		TEST_METHOD(FromJson_StringRange_ParsesUnboundedStart) {
			auto extent = nlohmann::json("../2026-01-03T04:05:06.789Z").get<TimeExtent>();

			Assert::IsTrue(extent.isUnboundedStart());
			Assert::AreEqual("../"s + END, extent.toStringUTC());
		}

		TEST_METHOD(ToJson_UnboundedEnds_WriteDoubleDots) {
			TimeExtent extent((TimePoint::min)(), (TimePoint::max)());

			nlohmann::ordered_json j;
			to_json(j, extent);
			Assert::AreEqual(R"(["..",".."])"s, j.dump());
		}

		TEST_METHOD(Json_RoundTrip_AllBoundKinds) {
			for (const auto* input : {
				R"(["2026-01-02T03:04:05.678Z","2026-01-03T04:05:06.789Z"])",
				R"(["..","2026-01-03T04:05:06.789Z"])",
				R"(["2026-01-02T03:04:05.678Z",".."])",
				R"(["2026-01-02T03:04:05.678Z","now"])",
				R"(["now","now"])" }) {
				auto extent = nlohmann::json::parse(input).get<TimeExtent>();

				nlohmann::ordered_json j;
				to_json(j, extent);
				Assert::AreEqual(std::string(input), j.dump());
			}
		}

		TEST_METHOD(GetSystemById_WithTimeExtent_SendsDatetimeRange) {
			httplib::Server server;
			std::string received;
			server.Get("/api/systems/sys1", [&](const httplib::Request& req, httplib::Response& res) {
				received = req.get_param_value("datetime");
				res.set_content(R"({"type":"Feature","id":"sys1","properties":{"featureType":"http://www.w3.org/ns/sosa/Sensor","uid":"urn:test:sys1","name":"Test"}})", "application/json");
			});

			int port = server.bind_to_any_port("127.0.0.1");
			std::thread serverThread([&] { server.listen_after_bind(); });
			server.wait_until_ready();

			ConSysAPI api("http://127.0.0.1:" + std::to_string(port) + "/api", "admin", "password");
			auto response = api.getSystemsAPI().getSystemById("sys1", TimeExtent(parse(START), parse(END)));

			server.stop();
			serverThread.join();

			Assert::AreEqual(200, response.getResponseCode());
			Assert::AreEqual(std::string(START) + "/" + END, received);
		}
	};
}
