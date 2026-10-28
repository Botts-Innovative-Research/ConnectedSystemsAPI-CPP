#include "pch.h"
#include "CppUnitTest.h"

#include <chrono>
#include <stdexcept>
#include <string>

#include "Util/TimeUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ConnectedSystemsAPI;
using namespace std::chrono;
using namespace std::string_literals;

namespace CSAPItest {
	TEST_CLASS(TimeUtilsTest)
	{
		// 2026-01-02T03:04:05Z
		static constexpr int64_t EPOCH_SECONDS = 1767323045;

		static int64_t toMicroseconds(const std::string& time) {
			return duration_cast<microseconds>(TimeUtils::parseTimeStringUTC(time).time_since_epoch()).count();
		}

		static std::string roundTrip(const std::string& time) {
			return TimeUtils::timePointToString(TimeUtils::parseTimeStringUTC(time));
		}

		static void assertInvalid(const std::string& time) {
			Assert::ExpectException<std::invalid_argument>([&] { TimeUtils::parseTimeStringUTC(time); });
		}

	public:
		TEST_METHOD(Parse_Milliseconds) {
			Assert::AreEqual(EPOCH_SECONDS * 1000000 + 678000, toMicroseconds("2026-01-02T03:04:05.678Z"));
		}

		TEST_METHOD(Parse_NoFractionalSeconds) {
			Assert::AreEqual(EPOCH_SECONDS * 1000000, toMicroseconds("2026-01-02T03:04:05Z"));
		}

		TEST_METHOD(Parse_OneFractionalDigit_IsTenths) {
			Assert::AreEqual(EPOCH_SECONDS * 1000000 + 900000, toMicroseconds("2026-01-02T03:04:05.9Z"));
		}

		TEST_METHOD(Parse_TwoFractionalDigits_IsHundredths) {
			Assert::AreEqual(EPOCH_SECONDS * 1000000 + 120000, toMicroseconds("2026-01-02T03:04:05.12Z"));
		}

		TEST_METHOD(Parse_Microseconds) {
			Assert::AreEqual(EPOCH_SECONDS * 1000000 + 123456, toMicroseconds("2026-01-02T03:04:05.123456Z"));
		}

		TEST_METHOD(Parse_NineFractionalDigits_KeepsMicroseconds) {
			Assert::AreEqual(EPOCH_SECONDS * 1000000 + 123456, toMicroseconds("2026-01-02T03:04:05.123456789Z"));
		}

		TEST_METHOD(Parse_PositiveOffset_ConvertsToUtc) {
			// 03:04:05+02:00 is 01:04:05Z
			Assert::AreEqual(int64_t(1767315845) * 1000000, toMicroseconds("2026-01-02T03:04:05+02:00"));
		}

		TEST_METHOD(Parse_NegativeOffsetWithoutColon_ConvertsToUtc) {
			// 03:04:05-0530 is 08:34:05Z
			Assert::AreEqual(int64_t(1767342845) * 1000000, toMicroseconds("2026-01-02T03:04:05-0530"));
		}

		TEST_METHOD(Parse_LowercaseSeparators) {
			Assert::AreEqual(EPOCH_SECONDS * 1000000, toMicroseconds("2026-01-02t03:04:05z"));
		}

		TEST_METHOD(Parse_LeapDay) {
			Assert::AreEqual(int64_t(1709208000) * 1000000, toMicroseconds("2024-02-29T12:00:00Z"));
		}

		TEST_METHOD(Parse_BeforeEpoch) {
			Assert::AreEqual(int64_t(-1) * 1000000 + 500000, toMicroseconds("1969-12-31T23:59:59.5Z"));
		}

		TEST_METHOD(Parse_Invalid_Throws) {
			assertInvalid("");
			assertInvalid("garbage");
			assertInvalid("2026-01-02");
			assertInvalid("2026-01-02T03:04:05");
			assertInvalid("2026-01-02T03:04:05.Z");
			assertInvalid("2026-13-02T03:04:05Z");
			assertInvalid("2026-01-02T24:04:05Z");
			assertInvalid("2026-01-02T03:04:05Zjunk");
			assertInvalid("2026-01-02T03:04:05+02");
		}

		TEST_METHOD(Format_Milliseconds) {
			auto tp = system_clock::time_point(duration_cast<system_clock::duration>(seconds(EPOCH_SECONDS) + milliseconds(678)));
			Assert::AreEqual("2026-01-02T03:04:05.678Z"s, TimeUtils::timePointToString(tp));
		}

		TEST_METHOD(Format_WholeSeconds_WritesThreeDigits) {
			auto tp = system_clock::time_point(duration_cast<system_clock::duration>(seconds(EPOCH_SECONDS)));
			Assert::AreEqual("2026-01-02T03:04:05.000Z"s, TimeUtils::timePointToString(tp));
		}

		TEST_METHOD(Format_Epoch) {
			Assert::AreEqual("1970-01-01T00:00:00.000Z"s, TimeUtils::timePointToString(system_clock::time_point()));
		}

		TEST_METHOD(Format_BeforeEpoch) {
			auto tp = system_clock::time_point(duration_cast<system_clock::duration>(milliseconds(-500)));
			Assert::AreEqual("1969-12-31T23:59:59.500Z"s, TimeUtils::timePointToString(tp));
		}

		TEST_METHOD(Format_ClockLimits_DoNotThrow) {
			Assert::IsFalse(TimeUtils::timePointToString((system_clock::time_point::min)()).empty());
			Assert::IsFalse(TimeUtils::timePointToString((system_clock::time_point::max)()).empty());
		}

		TEST_METHOD(RoundTrip_ServerFormats) {
			// Formats returned by an OpenSensorHub node
			Assert::AreEqual("2026-01-02T03:04:05.678Z"s, roundTrip("2026-01-02T03:04:05.678Z"));
			Assert::AreEqual("2026-01-02T03:04:05.000Z"s, roundTrip("2026-01-02T03:04:05Z"));
			Assert::AreEqual("2026-01-02T03:04:05.900Z"s, roundTrip("2026-01-02T03:04:05.9Z"));
			Assert::AreEqual("2026-01-02T03:04:05.120Z"s, roundTrip("2026-01-02T03:04:05.12Z"));
			Assert::AreEqual("2026-01-02T03:04:05.1234567Z"s, roundTrip("2026-01-02T03:04:05.1234567Z"));
			Assert::AreEqual("2026-01-02T01:04:05.000Z"s, roundTrip("2026-01-02T03:04:05+02:00"));
		}
	};
}
