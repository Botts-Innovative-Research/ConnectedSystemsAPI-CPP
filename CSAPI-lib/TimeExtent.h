#pragma once

#include <chrono>
#include <string>
#include <nlohmann/json.hpp>
#include "TimeUtils.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class TimeExtent {
		private:
			using TimePoint = std::chrono::system_clock::time_point;

			std::optional<TimePoint> start;
			std::optional<TimePoint> end;

		public:
			static constexpr const char* SPECIAL_VALUE_NOW = "now";
			static constexpr const char* SPECIAL_VALUE_UNBOUNDED = "..";

			TimeExtent() = default;
			TimeExtent(const std::optional<TimePoint>& start, const std::optional<TimePoint>& end)
				: start(start), end(end) {
			}

			const std::optional<TimePoint>& getStart() const { return start; }
			const std::optional<TimePoint>& getEnd() const { return end; }
		};

		inline void from_json(const nlohmann::json& j, TimeExtent& t) {
			if (!j.is_array() || j.size() != 2) {
				throw std::invalid_argument("TimeExtent JSON must be an array of two elements.");
			}

			std::optional<std::chrono::system_clock::time_point> start;
			std::string startStr = j.at(0).get<std::string>();
			if (startStr == TimeExtent::SPECIAL_VALUE_NOW) {
				start = std::nullopt; // 'now' is represented as nullopt
			}
			else if (startStr != TimeExtent::SPECIAL_VALUE_UNBOUNDED) {
				start = TimeUtils::parseTimeStringUTC(startStr);
			}

			std::optional<std::chrono::system_clock::time_point> end;
			std::string endStr = j.at(1).get<std::string>();
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
			return os << j.dump(4);
		}
	}
}