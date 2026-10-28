#pragma once

#include <optional>
#include <ostream>
#include <utility>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "../../Util/JsonUtils.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class AllowedValues;
	void to_json(nlohmann::ordered_json& j, const AllowedValues& v);

	/// <summary>
	/// Constraint restricting the possible values of a Count, CountRange, Quantity, or QuantityRange component
	/// to a list of values and/or intervals, optionally with a number of significant figures.
	/// </summary>
	class AllowedValues {
	public:
		/// <summary>
		/// An inclusive [min, max] interval.
		/// </summary>
		using Interval = std::pair<double, double>;

	private:
		std::optional<std::vector<double>> m_values;
		std::optional<std::vector<Interval>> m_intervals;
		std::optional<int> m_significantFigures;

	public:
		AllowedValues() = default;

		nlohmann::ordered_json toJson() const {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// List of allowed values.
		/// </summary>
		const std::optional<std::vector<double>>& getValues() const noexcept { return m_values; }
		void setValues(std::optional<std::vector<double>> values) { m_values = std::move(values); }
		void clearValues() noexcept { m_values.reset(); }

		/// <summary>
		/// List of allowed [min, max] intervals.
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

		friend void from_json(const nlohmann::json& j, AllowedValues& v);
		friend void to_json(nlohmann::ordered_json& j, const AllowedValues& v);

		friend bool operator==(const AllowedValues& a, const AllowedValues& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const AllowedValues& a, const AllowedValues& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const AllowedValues& v) {
			return os << v.toJson().dump(2);
		}
	};

	inline void from_json(const nlohmann::json& j, AllowedValues& v) {
		v.m_values = ConnectedSystemsAPI::JsonUtils::tryParseDoubleArray(j, "values");

		v.m_intervals.reset();
		if (j.contains("intervals") && j["intervals"].is_array()) {
			std::vector<AllowedValues::Interval> intervals;
			for (const auto& interval : j["intervals"]) {
				if (interval.is_array() && interval.size() == 2 && interval[0].is_number() && interval[1].is_number())
					intervals.emplace_back(interval[0].get<double>(), interval[1].get<double>());
			}
			v.m_intervals = std::move(intervals);
		}

		v.m_significantFigures = ConnectedSystemsAPI::JsonUtils::tryParseInteger(j, "significantFigures");
	}

	inline void to_json(nlohmann::ordered_json& j, const AllowedValues& v) {
		j = nlohmann::ordered_json::object();
		if (v.m_values) j["values"] = v.m_values.value();
		if (v.m_intervals) {
			j["intervals"] = nlohmann::ordered_json::array();
			for (const auto& [min, max] : v.m_intervals.value())
				j["intervals"].push_back({ min, max });
		}
		if (v.m_significantFigures) j["significantFigures"] = v.m_significantFigures.value();
	}
}
