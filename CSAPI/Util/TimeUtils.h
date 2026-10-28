#pragma once

#include <chrono>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace ConnectedSystemsAPI::TimeUtils {
	namespace Detail {
		using Clock = std::chrono::system_clock;

		/// <summary>
		/// Number of days since 1970-01-01 for a proleptic Gregorian date.
		/// See https://howardhinnant.github.io/date_algorithms.html#days_from_civil
		/// </summary>
		constexpr int64_t daysFromCivil(int64_t y, unsigned m, unsigned d) noexcept {
			y -= m <= 2;
			const int64_t era = (y >= 0 ? y : y - 399) / 400;
			const unsigned yoe = static_cast<unsigned>(y - era * 400);
			const unsigned doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;
			const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
			return era * 146097 + static_cast<int64_t>(doe) - 719468;
		}

		/// <summary>
		/// Proleptic Gregorian date for a number of days since 1970-01-01.
		/// See https://howardhinnant.github.io/date_algorithms.html#civil_from_days
		/// </summary>
		constexpr void civilFromDays(int64_t z, int64_t& y, unsigned& m, unsigned& d) noexcept {
			z += 719468;
			const int64_t era = (z >= 0 ? z : z - 146096) / 146097;
			const unsigned doe = static_cast<unsigned>(z - era * 146097);
			const unsigned yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
			const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
			const unsigned mp = (5 * doy + 2) / 153;
			d = doy - (153 * mp + 2) / 5 + 1;
			m = mp < 10 ? mp + 3 : mp - 9;
			y = static_cast<int64_t>(yoe) + era * 400 + (m <= 2);
		}

		/// <summary>
		/// Reads exactly `count` digits starting at `pos`, advancing `pos`.
		/// </summary>
		inline int readDigits(const std::string& s, size_t& pos, size_t count) {
			int value = 0;
			for (size_t i = 0; i < count; ++i, ++pos) {
				if (pos >= s.size() || s[pos] < '0' || s[pos] > '9')
					throw std::invalid_argument("Invalid RFC 3339 time: \"" + s + "\"");
				value = value * 10 + (s[pos] - '0');
			}
			return value;
		}

		inline void expect(const std::string& s, size_t& pos, const char* allowed) {
			if (pos >= s.size() || std::string(allowed).find(s[pos]) == std::string::npos)
				throw std::invalid_argument("Invalid RFC 3339 time: \"" + s + "\"");
			++pos;
		}

		inline std::string pad(int64_t value, int width) {
			std::string digits = std::to_string(value < 0 ? -value : value);
			if (static_cast<int>(digits.size()) < width)
				digits.insert(0, width - digits.size(), '0');
			return value < 0 ? "-" + digits : digits;
		}
	}

	/// <summary>
	/// Parses an RFC 3339 date-time, e.g., "2026-01-02T03:04:05Z", "2026-01-02T03:04:05.123456Z", or "2026-01-02T03:04:05+02:00".
	/// Fractional seconds may have any number of digits and are kept to the precision of system_clock.
	/// </summary>
	/// <exception cref="std::invalid_argument">Thrown if the string is not a valid RFC 3339 date-time.</exception>
	inline std::chrono::system_clock::time_point parseTimeStringUTC(const std::string& timeStr) {
		using namespace std::chrono;
		using Detail::readDigits;
		using Detail::expect;

		size_t pos = 0;
		const int year = readDigits(timeStr, pos, 4);
		expect(timeStr, pos, "-");
		const int month = readDigits(timeStr, pos, 2);
		expect(timeStr, pos, "-");
		const int day = readDigits(timeStr, pos, 2);
		expect(timeStr, pos, "Tt ");
		const int hour = readDigits(timeStr, pos, 2);
		expect(timeStr, pos, ":");
		const int minute = readDigits(timeStr, pos, 2);
		expect(timeStr, pos, ":");
		const int second = readDigits(timeStr, pos, 2);

		if (month < 1 || month > 12 || day < 1 || day > 31 || hour > 23 || minute > 59 || second > 60)
			throw std::invalid_argument("Invalid RFC 3339 time: \"" + timeStr + "\"");

		// Fractional seconds, any number of digits, truncated to nanoseconds
		nanoseconds fraction{ 0 };
		if (pos < timeStr.size() && timeStr[pos] == '.') {
			++pos;
			const size_t fractionStart = pos;
			int64_t nanos = 0;
			int digits = 0;
			while (pos < timeStr.size() && timeStr[pos] >= '0' && timeStr[pos] <= '9') {
				if (digits < 9) {
					nanos = nanos * 10 + (timeStr[pos] - '0');
					++digits;
				}
				++pos;
			}
			if (pos == fractionStart)
				throw std::invalid_argument("Invalid RFC 3339 time: \"" + timeStr + "\"");
			for (; digits < 9; ++digits) nanos *= 10;
			fraction = nanoseconds(nanos);
		}

		// Time zone: "Z" or a numeric offset
		minutes offset{ 0 };
		if (pos < timeStr.size() && (timeStr[pos] == 'Z' || timeStr[pos] == 'z')) {
			++pos;
		}
		else if (pos < timeStr.size() && (timeStr[pos] == '+' || timeStr[pos] == '-')) {
			const int sign = timeStr[pos++] == '-' ? -1 : 1;
			const int offsetHours = readDigits(timeStr, pos, 2);
			if (pos < timeStr.size() && timeStr[pos] == ':') ++pos;
			const int offsetMinutes = readDigits(timeStr, pos, 2);
			offset = minutes(sign * (offsetHours * 60 + offsetMinutes));
		}
		else {
			throw std::invalid_argument("Invalid RFC 3339 time, missing time zone: \"" + timeStr + "\"");
		}

		if (pos != timeStr.size())
			throw std::invalid_argument("Invalid RFC 3339 time: \"" + timeStr + "\"");

		const auto wholeSeconds = hours(24 * Detail::daysFromCivil(year, month, day))
			+ hours(hour) + minutes(minute) + seconds(second) - offset;
		return system_clock::time_point(duration_cast<system_clock::duration>(wholeSeconds)
			+ duration_cast<system_clock::duration>(fraction));
	}

	/// <summary>
	/// Formats a time point as an RFC 3339 UTC date-time, e.g., "2026-01-02T03:04:05.123Z".
	/// At least milliseconds are written; finer precision is written when present.
	/// </summary>
	inline std::string timePointToString(const std::chrono::system_clock::time_point& tp) {
		using namespace std::chrono;
		using Detail::pad;

		// Split into whole days and the time of day, flooring so times before 1970 work too
		const auto sinceEpoch = tp.time_since_epoch();
		auto dayCount = duration_cast<hours>(sinceEpoch).count() / 24;
		auto timeOfDay = sinceEpoch - duration_cast<system_clock::duration>(hours(24 * dayCount));
		if (timeOfDay < system_clock::duration::zero()) {
			timeOfDay += hours(24);
			--dayCount;
		}

		int64_t year;
		unsigned month, day;
		Detail::civilFromDays(dayCount, year, month, day);

		const auto h = duration_cast<hours>(timeOfDay);
		const auto m = duration_cast<minutes>(timeOfDay - h);
		const auto s = duration_cast<seconds>(timeOfDay - h - m);
		const auto nanos = duration_cast<nanoseconds>(timeOfDay - h - m - s).count();

		// Nine fractional digits, trimmed of trailing zeros down to a minimum of three
		std::string fraction = pad(nanos, 9);
		while (fraction.size() > 3 && fraction.back() == '0')
			fraction.pop_back();

		return pad(year, 4) + "-" + pad(month, 2) + "-" + pad(day, 2)
			+ "T" + pad(h.count(), 2) + ":" + pad(m.count(), 2) + ":" + pad(s.count(), 2)
			+ "." + fraction + "Z";
	}
}
