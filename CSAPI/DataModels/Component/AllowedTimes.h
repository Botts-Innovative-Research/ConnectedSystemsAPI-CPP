#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <utility>
#include <variant>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "../../Util/JsonUtils.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class AllowedTimes;
	void to_json(nlohmann::ordered_json& j, const AllowedTimes& v);

	/// <summary>
	/// Constraint restricting the possible values of a Time or TimeRange component
	/// to a list of time instants and/or intervals, optionally with a number of significant figures.
	/// </summary>
	class AllowedTimes {
	public:
		/// <summary>
		/// A time position: an ISO 8601 date-time string, or a number in the component's unit of measure.
		/// </summary>
		using TimePosition = std::variant<std::string, double>;
		/// <summary>
		/// An inclusive [start, end] interval.
		/// </summary>
		using Interval = std::pair<TimePosition, TimePosition>;

	private:
		std::optional<std::vector<TimePosition>> m_values;
		std::optional<std::vector<Interval>> m_intervals;
		std::optional<int> m_significantFigures;

	public:
		AllowedTimes() = default;

		nlohmann::ordered_json toJson() const {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// List of allowed time instants.
		/// </summary>
		const std::optional<std::vector<TimePosition>>& getValues() const noexcept { return m_values; }
		void setValues(std::optional<std::vector<TimePosition>> values) { m_values = std::move(values); }
		void clearValues() noexcept { m_values.reset(); }

		/// <summary>
		/// List of allowed [start, end] time intervals.
		/// </summary>
		const std::optional<std::vector<Interval>>& getIntervals() const noexcept { return m_intervals; }
		void setIntervals(std::optional<std::vector<Interval>> intervals) { m_intervals = std::move(intervals); }
		void clearIntervals() noexcept { m_intervals.reset(); }

		/// <summary>
		/// Maximum number of significant figures.
		/// </summary>
		const std::optional<int>& getSignificantFigures() const noexcept { return m_significantFigures; }
		void setSignificantFigures(std::optional<int> significantFigures) noexcept { m_significantFigures = significantFigures; }
		void clearSignificantFigures() noexcept { m_significantFigures.reset(); }

		friend void from_json(const nlohmann::json& j, AllowedTimes& v);
		friend void to_json(nlohmann::ordered_json& j, const AllowedTimes& v);

		friend bool operator==(const AllowedTimes& a, const AllowedTimes& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const AllowedTimes& a, const AllowedTimes& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const AllowedTimes& v) {
			return os << v.toJson().dump(2);
		}

	private:
		static std::optional<TimePosition> parseTimePosition(const nlohmann::json& j) {
			if (j.is_string()) return TimePosition(j.get<std::string>());
			if (j.is_number()) return TimePosition(j.get<double>());
			return std::nullopt;
		}

		static nlohmann::ordered_json timePositionToJson(const TimePosition& t) {
			if (std::holds_alternative<std::string>(t)) return std::get<std::string>(t);
			return std::get<double>(t);
		}
	};

	inline void from_json(const nlohmann::json& j, AllowedTimes& v) {
		v.m_values.reset();
		if (j.contains("values") && j["values"].is_array()) {
			std::vector<AllowedTimes::TimePosition> values;
			for (const auto& value : j["values"]) {
				if (auto t = AllowedTimes::parseTimePosition(value)) values.push_back(std::move(*t));
			}
			v.m_values = std::move(values);
		}

		v.m_intervals.reset();
		if (j.contains("intervals") && j["intervals"].is_array()) {
			std::vector<AllowedTimes::Interval> intervals;
			for (const auto& interval : j["intervals"]) {
				if (!interval.is_array() || interval.size() != 2) continue;
				auto start = AllowedTimes::parseTimePosition(interval[0]);
				auto end = AllowedTimes::parseTimePosition(interval[1]);
				if (start && end) intervals.emplace_back(std::move(*start), std::move(*end));
			}
			v.m_intervals = std::move(intervals);
		}

		v.m_significantFigures = ConnectedSystemsAPI::JsonUtils::tryParseInteger(j, "significantFigures");
	}

	inline void to_json(nlohmann::ordered_json& j, const AllowedTimes& v) {
		j = nlohmann::ordered_json::object();
		if (v.m_values) {
			j["values"] = nlohmann::ordered_json::array();
			for (const auto& t : v.m_values.value())
				j["values"].push_back(AllowedTimes::timePositionToJson(t));
		}
		if (v.m_intervals) {
			j["intervals"] = nlohmann::ordered_json::array();
			for (const auto& [start, end] : v.m_intervals.value())
				j["intervals"].push_back({ AllowedTimes::timePositionToJson(start), AllowedTimes::timePositionToJson(end) });
		}
		if (v.m_significantFigures) j["significantFigures"] = v.m_significantFigures.value();
	}
}
