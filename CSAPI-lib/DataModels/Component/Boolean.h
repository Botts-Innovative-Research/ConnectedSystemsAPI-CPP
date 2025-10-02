#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "ScalarComponent.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class Boolean;
			void to_json(nlohmann::ordered_json& j, const Boolean& q);

			class Boolean : public ScalarComponent {
			private:
				std::optional<bool> value;

			public:
				Boolean() = default;

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
				std::optional<int> getValue() const { return value; }
				/// <summary>Inline value(s) for the component.
				/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)</summary>
				void setValue(const std::optional<int>& value) { this->value = value; }
			};

			// Register with the DataComponentRegistry
			struct BooleanRegistrar {
				BooleanRegistrar() {
					ConnectedSystemsAPI::DataModels::Component::DataComponentRegistry::registerType(
						"Boolean", [](const nlohmann::json& j) {
						return std::make_unique<ConnectedSystemsAPI::DataModels::Component::Boolean>(j.get<ConnectedSystemsAPI::DataModels::Component::Boolean>());
					}
					);
				}
			};
			static BooleanRegistrar booleanRegistrar;

			inline void from_json(const nlohmann::json& j, Boolean& v) {
				from_json(j, static_cast<ScalarComponent&>(v));
				if (j.contains("value") && j["value"].is_boolean())
					v.setValue(j["value"].get<bool>());
				else
					v.setValue(std::nullopt);
			}

			inline void to_json(nlohmann::ordered_json& j, const Boolean& v) {
				to_json(j, static_cast<const ScalarComponent&>(v));
				if (v.getValue())
					j["value"] = v.getValue().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const Boolean& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}