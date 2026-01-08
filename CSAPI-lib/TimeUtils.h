#pragma once

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

namespace ConnectedSystemsAPI::TimeUtils {
	inline std::chrono::system_clock::time_point parseTimeStringUTC(const std::string& timeStr) {
		std::istringstream ss(timeStr);
		std::tm tm = {};
		char discard;
		int milliseconds;

		// Parse format: YYYY-MM-DDTHH:MM:SS.mmmZ
		ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");
		ss >> discard; // read '.'
		ss >> milliseconds;
		ss >> discard; // read 'Z'

		// Convert to time_t assuming UTC
		time_t time = _mkgmtime(&tm);

		// Convert to time_point and add milliseconds
		std::chrono::system_clock::time_point tp = std::chrono::system_clock::from_time_t(time);
		tp += std::chrono::milliseconds(milliseconds);

		return tp;
	}

	inline static std::string timePointToString(const std::chrono::system_clock::time_point& tp) {
		std::tm gmt{};
		time_t time = std::chrono::system_clock::to_time_t(tp);
		gmtime_s(&gmt, &time);

		std::ostringstream oss;
		oss << std::put_time(&gmt, "%Y-%m-%dT%H:%M:%S");

		// Add milliseconds
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch()) % 1000;
		oss << '.' << std::setw(3) << std::setfill('0') << ms.count() << 'Z';

		return oss.str();
	}
}