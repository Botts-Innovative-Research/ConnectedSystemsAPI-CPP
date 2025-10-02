#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "ScalarComponent.h"
#include "SimpleComponent.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class QuantityRange;
			void to_json(nlohmann::ordered_json& j, const QuantityRange& q);

			class QuantityRange : public SimpleComponent {
			private:
				std::optional<std::vector<double>> value;
				UnitOfMeasure unitOfMeasure;

			public:
				QuantityRange() = default;

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
				std::optional<std::vector<double>> getValue() const { return value; }
				/// <summary>Inline value(s) for the component.
				/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)</summary>
				void setValue(const std::optional<std::vector<double>>& value) { this->value = value; }
				/// <summary>Unit of measure used to express the value of this data component.</summary>
				const UnitOfMeasure& getUnitOfMeasure() const { return unitOfMeasure; }
				/// <summary>Unit of measure used to express the value of this data component.</summary>
				void setUnitOfMeasure(const UnitOfMeasure& unitOfMeasure) { this->unitOfMeasure = unitOfMeasure; }
			};

			// Register with the DataComponentRegistry
			struct QuantityRangeRegistrar {
				QuantityRangeRegistrar() {
					ConnectedSystemsAPI::DataModels::Component::DataComponentRegistry::registerType(
						"QuantityRange", [](const nlohmann::json& j) {
						return std::make_unique<ConnectedSystemsAPI::DataModels::Component::QuantityRange>(j.get<ConnectedSystemsAPI::DataModels::Component::QuantityRange>());
					}
					);
				}
			};
			static QuantityRangeRegistrar quantityRangeRegistrar;

			inline void from_json(const nlohmann::json& j, QuantityRange& v) {
				from_json(j, static_cast<SimpleComponent&>(v));

				if (j.contains("value") && j["value"].is_array())
					v.setValue(j["value"].get<std::vector<double>>());
				else
					v.setValue(std::nullopt);

				v.setUnitOfMeasure(j.at("uom").get<UnitOfMeasure>());
			}

			inline void to_json(nlohmann::ordered_json& j, const QuantityRange& v) {
				to_json(j, static_cast<const SimpleComponent&>(v));

				if (v.getValue())
					j["value"] = v.getValue().value();

				j["uom"] = v.getUnitOfMeasure();
			}

			inline std::ostream& operator<<(std::ostream& os, const QuantityRange& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}