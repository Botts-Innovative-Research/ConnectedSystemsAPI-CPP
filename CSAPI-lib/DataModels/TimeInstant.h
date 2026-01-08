#pragma once

#include <chrono>
#include <string>
#include <nlohmann/json.hpp>

#include "TimeUtils.h"

namespace ConnectedSystemsAPI::DataModels {
	class TimeInstant {
	private:
		std::chrono::system_clock::time_point timePoint;

	public:
		TimeInstant() = default;
		explicit TimeInstant(const std::chrono::system_clock::time_point& tp)
			: timePoint(tp) {
		}

		/// <summary>
		/// The time point represented by this TimeInstant.
		/// </summary>
		const std::chrono::system_clock::time_point& getTimePoint() const { return timePoint; }

		friend void from_json(const nlohmann::json& j, TimeInstant& t);
		friend void to_json(nlohmann::ordered_json& j, const TimeInstant& t);
	};

	inline void from_json(const nlohmann::json& j, TimeInstant& t) {
		std::string timeStr = j.get<std::string>();
		t.timePoint = TimeUtils::parseTimeStringUTC(timeStr);
	}

	inline void to_json(nlohmann::ordered_json& j, const TimeInstant& t) {
		std::string timeStr = TimeUtils::timePointToString(t.getTimePoint());
		j = timeStr;
	}

	inline std::ostream& operator<<(std::ostream& os, const TimeInstant& t) {
		nlohmann::ordered_json j;
		ConnectedSystemsAPI::DataModels::to_json(j, t);
		os << j.dump();
		return os;
	}

	inline bool operator==(const TimeInstant& a, const TimeInstant& b) {
		return a.getTimePoint() == b.getTimePoint();
	}
	inline bool operator!=(const TimeInstant& a, const TimeInstant& b) {
		return !(a == b);
	}
}