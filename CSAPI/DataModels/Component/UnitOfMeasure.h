#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Util/JsonUtils.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class UnitOfMeasure;
	void to_json(nlohmann::ordered_json& j, const UnitOfMeasure& v);

	class UnitOfMeasure {
	private:
		std::optional<std::string> m_label;
		std::optional<std::string> m_symbol;
		std::optional<std::string> m_code;
		std::optional<std::string> m_href;

	public:
		UnitOfMeasure() = default;
		UnitOfMeasure(const UnitOfMeasure&) = default;
		UnitOfMeasure(UnitOfMeasure&&) noexcept = default;
		UnitOfMeasure& operator=(const UnitOfMeasure&) = default;
		UnitOfMeasure& operator=(UnitOfMeasure&&) noexcept = default;
		~UnitOfMeasure() = default;

		void validate() const {
			if (!m_code && !m_href)
				throw std::invalid_argument("UnitOfMeasure: either code or href is required");
		}

		nlohmann::ordered_json toJson() const {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// Human-readable label for the unit.
		/// </summary>
		std::optional<std::string> getLabel() const { return m_label; }
		void setLabel(const std::optional<std::string>& label) { m_label = label; }
		void setLabel(std::string&& label) { m_label = std::move(label); }
		bool hasLabel() const { return m_label.has_value(); }
		void clearLabel() { m_label.reset(); }

		/// <summary>
		/// The preferred unit symbol to use when presenting the data in a UI (uses the UCUM code instead if no symbol is provided).
		/// </summary>
		std::optional<std::string> getSymbol() const { return m_symbol; }
		void setSymbol(const std::optional<std::string>& symbol) { m_symbol = symbol; }
		void setSymbol(std::string&& symbol) { m_symbol = std::move(symbol); }
		bool hasSymbol() const { return m_symbol.has_value(); }
		void clearSymbol() { m_symbol.reset(); }

		/// <summary>
		/// UCUM code for the unit of measure.
		/// </summary>
		std::optional<std::string> getCode() const { return m_code; }
		void setCode(const std::optional<std::string>& code) { m_code = code; }
		void setCode(std::string&& code) { m_code = std::move(code); }
		bool hasCode() const { return m_code.has_value(); }
		void clearCode() { m_code.reset(); }

		/// <summary>
		/// Link to a definition of the unit of measure.
		/// </summary>
		std::optional<std::string> getHref() const { return m_href; }
		void setHref(const std::optional<std::string>& href) { m_href = href; }
		void setHref(std::string&& href) { m_href = std::move(href); }
		bool hasHref() const { return m_href.has_value(); }
		void clearHref() { m_href.reset(); }

		friend void from_json(const nlohmann::json& j, UnitOfMeasure& v);
		friend void to_json(nlohmann::ordered_json& j, const UnitOfMeasure& v);

		friend std::ostream& operator<<(std::ostream& os, const UnitOfMeasure& v) {
			return os << v.toJson().dump(2);
		}
	};

	inline void from_json(const nlohmann::json& j, UnitOfMeasure& v) {
		v.m_label = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "label");
		v.m_symbol = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "symbol");
		v.m_code = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "code");
		v.m_href = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "href");
	}

	inline void to_json(nlohmann::ordered_json& j, const UnitOfMeasure& v) {
		j = nlohmann::ordered_json::object();

		if (v.m_label) j["label"] = v.m_label;
		if (v.m_symbol) j["symbol"] = v.m_symbol;
		if (v.m_code) j["code"] = v.m_code;
		if (v.m_href) j["href"] = v.m_href;
	}
}