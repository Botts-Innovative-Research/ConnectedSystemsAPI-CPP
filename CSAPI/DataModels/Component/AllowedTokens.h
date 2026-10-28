#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <utility>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "../../Util/JsonUtils.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class AllowedTokens;
	void to_json(nlohmann::ordered_json& j, const AllowedTokens& v);

	/// <summary>
	/// Constraint restricting the possible values of a Category, CategoryRange, or Text component
	/// to a list of tokens and/or a regular expression pattern.
	/// </summary>
	class AllowedTokens {
	private:
		std::optional<std::vector<std::string>> m_values;
		std::optional<std::string> m_pattern;

	public:
		AllowedTokens() = default;

		nlohmann::ordered_json toJson() const {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// List of allowed token values.
		/// </summary>
		const std::optional<std::vector<std::string>>& getValues() const noexcept { return m_values; }
		void setValues(std::optional<std::vector<std::string>> values) { m_values = std::move(values); }
		void clearValues() noexcept { m_values.reset(); }

		/// <summary>
		/// Regular expression that values must match.
		/// </summary>
		const std::optional<std::string>& getPattern() const noexcept { return m_pattern; }
		void setPattern(std::optional<std::string> pattern) { m_pattern = std::move(pattern); }
		void clearPattern() noexcept { m_pattern.reset(); }

		friend void from_json(const nlohmann::json& j, AllowedTokens& v);
		friend void to_json(nlohmann::ordered_json& j, const AllowedTokens& v);

		friend bool operator==(const AllowedTokens& a, const AllowedTokens& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const AllowedTokens& a, const AllowedTokens& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const AllowedTokens& v) {
			return os << v.toJson().dump(2);
		}
	};

	inline void from_json(const nlohmann::json& j, AllowedTokens& v) {
		v.m_values = ConnectedSystemsAPI::JsonUtils::tryParseStringArray(j, "values");
		v.m_pattern = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "pattern");
	}

	inline void to_json(nlohmann::ordered_json& j, const AllowedTokens& v) {
		j = nlohmann::ordered_json::object();
		if (v.m_values) j["values"] = v.m_values.value();
		if (v.m_pattern) j["pattern"] = v.m_pattern.value();
	}
}
