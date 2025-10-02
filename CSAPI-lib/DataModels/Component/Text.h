#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "ScalarComponent.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class Text;
			void to_json(nlohmann::ordered_json& j, const Text& q);

			class Text : public ScalarComponent {
			private:
				std::optional<std::string> value;

			public:
				Text() = default;

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
			};

			// Register with the DataComponentRegistry
			struct TextRegistrar {
				TextRegistrar() {
					ConnectedSystemsAPI::DataModels::Component::DataComponentRegistry::registerType(
						"Text", [](const nlohmann::json& j) {
						return std::make_unique<ConnectedSystemsAPI::DataModels::Component::Text>(j.get<ConnectedSystemsAPI::DataModels::Component::Text>());
					}
					);
				}
			};
			static TextRegistrar textRegistrar;

			inline void from_json(const nlohmann::json& j, Text& v) {
				from_json(j, static_cast<ScalarComponent&>(v));
				if (j.contains("value") && j["value"].is_string())
					v.setValue(j["value"].get<std::string>());
				else
					v.setValue(std::nullopt);
			}

			inline void to_json(nlohmann::ordered_json& j, const Text& v) {
				to_json(j, static_cast<const ScalarComponent&>(v));
				if (v.getValue())
					j["value"] = v.getValue().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const Text& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}