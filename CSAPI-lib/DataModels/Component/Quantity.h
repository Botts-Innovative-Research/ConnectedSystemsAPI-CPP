#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "ScalarComponent.h"
#include "UnitOfMeasure.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class Quantity;
			void to_json(nlohmann::ordered_json& j, const Quantity& q);

			class Quantity : public ScalarComponent {
			private:
				std::optional<double> value;
				UnitOfMeasure unitOfMeasure;

			public:
				Quantity() = default;

				void validate() const {
					ScalarComponent::validate();
					unitOfMeasure.validate();
				}

				nlohmann::ordered_json toJson() const override {
					nlohmann::ordered_json j;
					to_json(j, *this);
					return j;
				}

				/// <summary>Inline value(s) for the component.
				/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)</summary>
				std::optional<double> getValue() const { return value; }
				/// <summary>Inline value(s) for the component.
				/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)</summary>
				void setValue(const std::optional<double>& value) { this->value = value; }
				/// <summary>Unit of measure used to express the value of this data component.</summary>
				const UnitOfMeasure& getUnitOfMeasure() const { return unitOfMeasure; }
				/// <summary>Unit of measure used to express the value of this data component.</summary>
				void setUnitOfMeasure(const UnitOfMeasure& unitOfMeasure) { this->unitOfMeasure = unitOfMeasure; }
			};

			// Register with the DataComponentRegistry
			struct QuantityRegistrar {
				QuantityRegistrar() {
					ConnectedSystemsAPI::DataModels::Component::DataComponentRegistry::registerType(
						"Quantity", [](const nlohmann::json& j) {
						return std::make_unique<ConnectedSystemsAPI::DataModels::Component::Quantity>(j.get<ConnectedSystemsAPI::DataModels::Component::Quantity>());
					}
					);
				}
			};
			static QuantityRegistrar quantityRegistrar;

			inline void from_json(const nlohmann::json& j, Quantity& v) {
				from_json(j, static_cast<ScalarComponent&>(v));

				if (j.contains("value") && j["value"].is_number())
					v.setValue(j.at("value").get<double>());
				else
					v.setValue(std::nullopt);
				v.setUnitOfMeasure(j.at("uom").get<UnitOfMeasure>());
			}

			inline void to_json(nlohmann::ordered_json& j, const Quantity& v) {
				to_json(j, static_cast<const ScalarComponent&>(v));
				if (v.getValue())
					j["value"] = v.getValue().value();
				j["uom"] = v.getUnitOfMeasure();
			}

			inline std::ostream& operator<<(std::ostream& os, const Quantity& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}