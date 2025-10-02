#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "SimpleComponent.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class CategoryRange;
			void to_json(nlohmann::ordered_json& j, const CategoryRange& q);

			class CategoryRange : public SimpleComponent {
			private:
				std::optional<std::vector<std::string>> value;
				std::optional<std::string> codeSpace;

			public:
				CategoryRange() = default;

				void validate() const {
					SimpleComponent::validate();
				}

				nlohmann::ordered_json toJson() const override {
					nlohmann::ordered_json j;
					to_json(j, *this);
					return j;
				}

				/// <summary>Inline value(s) for the component.
				/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)</summary>
				std::optional<std::vector<std::string>> getValue() const { return value; }
				/// <summary>Inline value(s) for the component.
				/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)</summary>
				void setValue(const std::optional<std::vector<std::string>>& value) { this->value = value; }
				/// <summary>Name of the dictionary where the possible values for this component are listed and defined.</summary>
				std::optional<std::string> getCodeSpace() const { return codeSpace; }
				/// <summary>Name of the dictionary where the possible values for this component are listed and defined.</summary>
				void setCodeSpace(const std::optional<std::string>& codeSpace) { this->codeSpace = codeSpace; }
			};

			// Register with the DataComponentRegistry
			struct CategoryRangeRegistrar {
				CategoryRangeRegistrar() {
					ConnectedSystemsAPI::DataModels::Component::DataComponentRegistry::registerType(
						"CategoryRange", [](const nlohmann::json& j) {
						return std::make_unique<ConnectedSystemsAPI::DataModels::Component::CategoryRange>(j.get<ConnectedSystemsAPI::DataModels::Component::CategoryRange>());
					}
					);
				}
			};
			static CategoryRangeRegistrar categoryRangeRegistrar;

			inline void from_json(const nlohmann::json& j, CategoryRange& v) {
				from_json(j, static_cast<SimpleComponent&>(v));

				if (j.contains("value") && j["value"].is_array())
					v.setValue(j["value"].get<std::vector<std::string>>());
				else
					v.setValue(std::nullopt);

				if (j.contains("codeSpace") && j["codeSpace"].is_string())
					v.setCodeSpace(j["codeSpace"].get<std::string>());
				else
					v.setCodeSpace(std::nullopt);
			}

			inline void to_json(nlohmann::ordered_json& j, const CategoryRange& v) {
				to_json(j, static_cast<const SimpleComponent&>(v));

				if (v.getValue())
					j["value"] = v.getValue().value();

				if (v.getCodeSpace())
					j["codeSpace"] = v.getCodeSpace().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const CategoryRange& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}