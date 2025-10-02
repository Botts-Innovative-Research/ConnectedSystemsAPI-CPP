#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class UnitOfMeasure {
		private:
			std::optional<std::string> label;
			std::optional<std::string> symbol;
			std::optional<std::string> code;
			std::optional<std::string> href;

		public:
			UnitOfMeasure() = default;

			void validate() const {
				if (!code && !href)
					throw std::invalid_argument("UnitOfMeasure: either code or href is required");
			}

			/// <summary>Human-readable label for the unit.</summary>
			const std::optional<std::string> getLabel() const { return label; }
			/// <summary>Human-readable label for the unit.</summary>
			void setLabel(const std::optional<std::string>& label) { this->label = label; }
			/// <summary>The preferred unit symbol to use when presenting the data in a UI (uses the UCUM code instead if no symbol is provided).</summary>
			const std::optional<std::string> getSymbol() const { return symbol; }
			/// <summary>The preferred unit symbol to use when presenting the data in a UI (uses the UCUM code instead if no symbol is provided).</summary>
			void setSymbol(const std::optional<std::string>& symbol) { this->symbol = symbol; }
			/// <summary>UCUM code for the unit of measure.</summary>
			const std::optional<std::string> getCode() const { return code; }
			/// <summary>UCUM code for the unit of measure.</summary>
			void setCode(const std::optional<std::string>& code) { this->code = code; }
			/// <summary>Link to a definition of the unit of measure.</summary>
			const std::optional<std::string> getHref() const { return href; }
			/// <summary>Link to a definition of the unit of measure.</summary>
			void setHref(const std::optional<std::string>& href) { this->href = href; }
		};

		inline void from_json(const nlohmann::json& j, UnitOfMeasure& u) {
			u = UnitOfMeasure();
			if (j.contains("label") && j["label"].is_string())
				u.setLabel(j["label"].get<std::string>());
			else
				u.setLabel(std::nullopt);

			if (j.contains("symbol") && j["symbol"].is_string())
				u.setSymbol(j["symbol"].get<std::string>());
			else
				u.setSymbol(std::nullopt);

			if (j.contains("code") && j["code"].is_string())
				u.setCode(j["code"].get<std::string>());
			else
				u.setCode(std::nullopt);

			if (j.contains("href") && j["href"].is_string())
				u.setHref(j["href"].get<std::string>());
			else
				u.setHref(std::nullopt);
		}

		inline void to_json(nlohmann::ordered_json& j, const UnitOfMeasure& u) {
			j = nlohmann::ordered_json::object();
			if (u.getLabel())
				j["label"] = u.getLabel();
			if (u.getSymbol())
				j["symbol"] = u.getSymbol();
			if (u.getCode())
				j["code"] = u.getCode();
			if (u.getHref())
				j["href"] = u.getHref();
		}

		inline std::ostream& operator<<(std::ostream& os, const UnitOfMeasure& u) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, u);
			os << j.dump(2);
		}
	}
}