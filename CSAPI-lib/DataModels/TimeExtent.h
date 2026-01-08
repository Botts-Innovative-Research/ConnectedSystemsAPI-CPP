#pragma once

#include <chrono>
#include <string>
#include <optional>
#include <nlohmann/json.hpp>

#include "TimeUtils.h"

namespace ConnectedSystemsAPI::DataModels {
	/// <summary>
	/// Represents a time or time range.
	/// Also supports special cases of time instants at 'now',
	/// time periods beginning or ending at 'now', and open-ended time periods.
	/// The time points are represented as UTC time points.
	/// </summary>
	class TimeExtent {
	private:
		using TimePoint = std::chrono::system_clock::time_point;

		std::optional<TimePoint> start; // nullopt means 'now', min() means unbounded past
		std::optional<TimePoint> end;   // nullopt means 'now', max() means unbounded future

	public:
		static constexpr const char* SPECIAL_VALUE_NOW = "now";
		static constexpr const char* SPECIAL_VALUE_UNBOUNDED = "..";

		TimeExtent() = default;
		TimeExtent(const std::optional<TimePoint>& start, const std::optional<TimePoint>& end)
			: start(start), end(end) {
		}

		/// <returns>The start time point. Nullopt represents 'now', min() represents unbounded past.</returns>
		const std::optional<TimePoint>& getStart() const { return start; }
		/// <returns>The end time point. Nullopt represents 'now', max() represents unbounded future.</returns>
		const std::optional<TimePoint>& getEnd() const { return end; }

		/// <returns>True if the start time is a defined value (not 'now' and not unbounded).</returns>
		bool hasStart() const { return start.has_value() && start.value() != (TimePoint::min)(); }
		/// <returns>True if the end time is a defined value (not 'now' and not unbounded).</returns>
		bool hasEnd() const { return end.has_value() && end.value() != (TimePoint::max)(); }
		/// <returns>True if the range is unbounded at both ends, representing all times.</returns>
		bool isUnbounded() const { return !hasStart() && !hasEnd(); }
		/// <returns>True if the start is unbounded, representing all times before the end.</returns>
		bool isUnboundedStart() const { return start.has_value() && !hasStart(); }
		/// <returns>True if the end is unbounded, representing all times after the start.</returns>
		bool isUnboundedEnd() const { return end.has_value() && !hasEnd(); }
		/// <returns>True if the time interval represents a single instant (start equals end).</returns>
		bool isInstant() const { return hasStart() && hasEnd() && start.value() == end.value(); }
		/// <returns>True if the time interval starts at 'now'.</returns>
		bool startsNow() const { return !start.has_value(); }
		/// <returns>True if the time interval ends at 'now'.</returns>
		bool endsNow() const { return !end.has_value(); }
		/// <returns>True if the time interval represents 'now' (both start and end are 'now').</returns>
		bool isNow() const { return (!startsNow() && !endsNow()); }

		/// <summary>
		/// Returns a string representation of the time extent in the format "start/end", or "instant" if the time extent represents a single instant.
		/// </summary>
		/// <param name="allowSpecialValues">If true, 'now' and unbounded values are represented by their special strings ("now" or "..").
		///                                  If false, all times are represented as exact time points, with 'now' replaced by the current system time,
		///                                  and unbounded values replaced by min() or max().</param>
		/// <returns></returns>
		std::string toStringUTC(bool allowSpecialValues = true) const {
			std::string startStr;
			std::string endStr;
			std::string nowStr = allowSpecialValues ? SPECIAL_VALUE_NOW : TimeUtils::timePointToString(std::chrono::system_clock::now());

			if (isNow())
				return nowStr;
			if (isInstant())
				return TimeUtils::timePointToString(start.value());

			if (startsNow())
				startStr = nowStr;
			else if (isUnboundedStart())
				startStr = allowSpecialValues ? SPECIAL_VALUE_UNBOUNDED : TimeUtils::timePointToString((TimePoint::min)());
			else
				startStr = TimeUtils::timePointToString(start.value());

			if (endsNow())
				endStr = nowStr;
			else if (isUnboundedEnd())
				endStr = allowSpecialValues ? SPECIAL_VALUE_UNBOUNDED : TimeUtils::timePointToString((TimePoint::max)());
			else
				endStr = TimeUtils::timePointToString(end.value());

			return startStr + "/" + endStr;
		}

		friend void from_json(const nlohmann::json& j, TimeExtent& t);
		friend void to_json(nlohmann::ordered_json& j, const TimeExtent& t);
		friend bool operator==(const TimeExtent& a, const TimeExtent& b);
		friend bool operator!=(const TimeExtent& a, const TimeExtent& b);
	};

	inline void from_json(const nlohmann::json& j, TimeExtent& t) {
		std::string startStr;
		std::string endStr;

		if (j.is_array()) {
			if (j.size() != 2) {
				throw std::invalid_argument("TimeExtent JSON must be an array of two elements.");
			}

			startStr = j.at(0).get<std::string>();
			endStr = j.at(1).get<std::string>();
		}
		else if (j.is_string()) {
			std::string timeStr = j.get<std::string>();
			size_t delimPos = timeStr.find('/');
			if (delimPos == std::string::npos) {
				// Single instant
				startStr = timeStr;
				endStr = timeStr;
			}
			else {
				// Time range
				startStr = timeStr.substr(0, delimPos);
				endStr = timeStr.substr(delimPos + 1);
			}
		}
		else {
			throw std::invalid_argument("TimeExtent JSON must be either an array of two elements or a string.");
		}

		std::optional<std::chrono::system_clock::time_point> start;
		if (startStr == TimeExtent::SPECIAL_VALUE_NOW) {
			start = std::nullopt; // 'now' is represented as nullopt
		}
		else if (startStr != TimeExtent::SPECIAL_VALUE_UNBOUNDED) {
			start = TimeUtils::parseTimeStringUTC(startStr);
		}

		std::optional<std::chrono::system_clock::time_point> end;
		if (endStr == TimeExtent::SPECIAL_VALUE_NOW) {
			end = std::nullopt; // 'now' is represented as nullopt
		}
		else if (endStr != TimeExtent::SPECIAL_VALUE_UNBOUNDED) {
			end = TimeUtils::parseTimeStringUTC(endStr);
		}

		t = TimeExtent(start, end);
	}

	inline void to_json(nlohmann::ordered_json& j, const TimeExtent& t) {
		std::string startStr = t.getStart() ? TimeUtils::timePointToString(t.getStart().value()) : TimeExtent::SPECIAL_VALUE_NOW;
		std::string endStr = t.getEnd() ? TimeUtils::timePointToString(t.getEnd().value()) : TimeExtent::SPECIAL_VALUE_NOW;
		j = nlohmann::json::array({ startStr, endStr });
	}

	inline std::ostream& operator<<(std::ostream& os, const TimeExtent& t) {
		nlohmann::ordered_json j;
		ConnectedSystemsAPI::DataModels::to_json(j, t);
		return os << j.dump(2);
	}

	inline bool operator== (const TimeExtent& a, const TimeExtent& b) {
		return a.getStart() == b.getStart() && a.getEnd() == b.getEnd();
	}

	inline bool operator!= (const TimeExtent& a, const TimeExtent& b) {
		return !(a == b);
	}
}