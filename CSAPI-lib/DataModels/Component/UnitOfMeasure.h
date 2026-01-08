#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <nlohmann/json.hpp>
#include "Util/JsonUtils.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class UnitOfMeasure {
	private:
		std::optional<std::string> label;
		std::optional<std::string> symbol;
		std::optional<std::string> code;
		std::optional<std::string> href;

	public:
		UnitOfMeasure() = default;
		UnitOfMeasure(const UnitOfMeasure&) = default;
		UnitOfMeasure(UnitOfMeasure&&) noexcept = default;
		UnitOfMeasure& operator=(const UnitOfMeasure&) = default;
		UnitOfMeasure& operator=(UnitOfMeasure&&) noexcept = default;
		~UnitOfMeasure() = default;

		void validate() const {
			if (!code && !href)
				throw std::invalid_argument("UnitOfMeasure: either code or href is required");
		}

		/// <summary>
		/// Human-readable label for the unit.
		/// </summary>
		const std::optional<std::string> getLabel() const { return label; }
		void setLabel(const std::optional<std::string>& label) { this->label = label; }
		void setLabel(std::string&& label) { this->label = std::move(label); }
		bool hasLabel() const { return label.has_value(); }
		void clearLabel() { label.reset(); }

		/// <summary>
		/// The preferred unit symbol to use when presenting the data in a UI (uses the UCUM code instead if no symbol is provided).
		/// </summary>
		const std::optional<std::string> getSymbol() const { return symbol; }
		void setSymbol(const std::optional<std::string>& symbol) { this->symbol = symbol; }
		void setSymbol(std::string&& symbol) { this->symbol = std::move(symbol); }
		bool hasSymbol() const { return symbol.has_value(); }
		void clearSymbol() { symbol.reset(); }

		/// <summary>
		/// UCUM code for the unit of measure.
		/// </summary>
		const std::optional<std::string> getCode() const { return code; }
		void setCode(const std::optional<std::string>& code) { this->code = code; }
		void setCode(std::string&& code) { this->code = std::move(code); }
		bool hasCode() const { return code.has_value(); }
		void clearCode() { code.reset(); }

		/// <summary>
		/// Link to a definition of the unit of measure.
		/// </summary>
		const std::optional<std::string> getHref() const { return href; }
		void setHref(const std::optional<std::string>& href) { this->href = href; }
		void setHref(std::string&& href) { this->href = std::move(href); }
		bool hasHref() const { return href.has_value(); }
		void clearHref() { href.reset(); }
	};

	inline void from_json(const nlohmann::json& j, UnitOfMeasure& v) {
		v = UnitOfMeasure();
		v.setLabel(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "label"));
		v.setSymbol(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "symbol"));
		v.setCode(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "code"));
		v.setHref(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "href"));
	}

	inline void to_json(nlohmann::ordered_json& j, const UnitOfMeasure& v) {
		j = nlohmann::ordered_json::object();
		if (v.getLabel()) j["label"] = v.getLabel();
		if (v.getSymbol()) j["symbol"] = v.getSymbol();
		if (v.getCode()) j["code"] = v.getCode();
		if (v.getHref()) j["href"] = v.getHref();
	}

	inline std::ostream& operator<<(std::ostream& os, const UnitOfMeasure& v) {
		nlohmann::ordered_json j;
		ConnectedSystemsAPI::DataModels::Component::to_json(j, v);
		os << j.dump(2);
	}
}