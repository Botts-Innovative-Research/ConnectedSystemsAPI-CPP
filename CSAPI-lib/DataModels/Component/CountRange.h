#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "SimpleComponent.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class CountRange;
			void to_json(nlohmann::ordered_json& j, const CountRange& q);

			class CountRange : public SimpleComponent {
			private:
				std::optional<std::vector<int>> value;

			public:
				CountRange() = default;

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
				std::optional<std::vector<int>> getValue() const { return value; }
				/// <summary>Inline value(s) for the component.
				/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)</summary>
				void setValue(const std::optional<std::vector<int>>& value) { this->value = value; }
			};

			// Register with the DataComponentRegistry
			struct CountRangeRegistrar {
				CountRangeRegistrar() {
					ConnectedSystemsAPI::DataModels::Component::DataComponentRegistry::registerType(
						"CountRange", [](const nlohmann::json& j) {
						return std::make_unique<ConnectedSystemsAPI::DataModels::Component::CountRange>(j.get<ConnectedSystemsAPI::DataModels::Component::CountRange>());
					}
					);
				}
			};
			static CountRangeRegistrar countRangeRegistrar;

			inline void from_json(const nlohmann::json& j, CountRange& v) {
				from_json(j, static_cast<SimpleComponent&>(v));

				if (j.contains("value") && j["value"].is_array())
					v.setValue(j["value"].get<std::vector<int>>());
				else
					v.setValue(std::nullopt);
			}

			inline void to_json(nlohmann::ordered_json& j, const CountRange& v) {
				to_json(j, static_cast<const SimpleComponent&>(v));

				if (v.getValue())
					j["value"] = v.getValue().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const CountRange& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}