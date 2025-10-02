#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "ScalarComponent.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class Category;
			void to_json(nlohmann::ordered_json& j, const Category& q);

			class Category : public ScalarComponent {
			private:
				std::optional<std::string> value;
				std::optional<std::string> codeSpace;

			public:
				Category() = default;

				void validate() const {
					ScalarComponent::validate();
				}

				nlohmann::ordered_json toJson() const override {
					nlohmann::ordered_json j;
					to_json(j, *this);
					return j;
				}

				/// <summary>Inline value(s) for the component.
				/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)</summary>
				std::optional<std::string> getValue() const { return value; }
				/// <summary>Inline value(s) for the component.
				/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)</summary>
				void setValue(const std::optional<std::string>& value) { this->value = value; }
				/// <summary>Name of the dictionary where the possible values for this component are listed and defined.</summary>
				std::optional<std::string> getCodeSpace() const { return codeSpace; }
				/// <summary>Name of the dictionary where the possible values for this component are listed and defined.</summary>
				void setCodeSpace(const std::optional<std::string>& codeSpace) { this->codeSpace = codeSpace; }
			};

			// Register with the DataComponentRegistry
			struct CategoryRegistrar {
				CategoryRegistrar() {
					ConnectedSystemsAPI::DataModels::Component::DataComponentRegistry::registerType(
						"Category", [](const nlohmann::json& j) {
						return std::make_unique<ConnectedSystemsAPI::DataModels::Component::Category>(j.get<ConnectedSystemsAPI::DataModels::Component::Category>());
					}
					);
				}
			};
			static CategoryRegistrar categoryRegistrar;

			inline void from_json(const nlohmann::json& j, Category& v) {
				from_json(j, static_cast<ScalarComponent&>(v));

				if (j.contains("value") && j["value"].is_string())
					v.setValue(j["value"].get<std::string>());
				else
					v.setValue(std::nullopt);

				if (j.contains("codeSpace") && j["codeSpace"].is_string())
					v.setCodeSpace(j["codeSpace"].get<std::string>());
				else
					v.setCodeSpace(std::nullopt);
			}

			inline void to_json(nlohmann::ordered_json& j, const Category& v) {
				to_json(j, static_cast<const ScalarComponent&>(v));

				if (v.getValue())
					j["value"] = v.getValue().value();

				if (v.getCodeSpace())
					j["codeSpace"] = v.getCodeSpace().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const Category& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}