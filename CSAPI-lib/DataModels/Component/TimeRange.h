#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "SimpleComponent.h"
#include "UnitOfMeasure.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class TimeRange;
			void to_json(nlohmann::ordered_json& j, const TimeRange& q);

			class TimeRange : public SimpleComponent {
			private:
				std::optional<std::vector<double>> value;
				std::optional<std::string> referenceTime;
				std::optional<std::string> localFrame;
				std::optional<UnitOfMeasure> unitOfMeasure;

			public:
				TimeRange() = default;

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
				/// <summary>Specifies the origin of the temporal reference frame as an ISO8601 date
				/// (used to specify time after an epoch that is to say in a custom frame)</summary>
				std::optional<std::string> getReferenceTime() const { return referenceTime; }
				/// <summary>Specifies the origin of the temporal reference frame as an ISO8601 date
				/// (used to specify time after an epoch that is to say in a custom frame)</summary>
				void setReferenceTime(const std::optional<std::string>& referenceTime) { this->referenceTime = referenceTime; }
				/// <summary>Temporal frame of reference whose origin is located by the value of this component.</summary>
				std::optional<std::string> getLocalFrame() const { return localFrame; }
				/// <summary>Temporal frame of reference whose origin is located by the value of this component.</summary>
				void setLocalFrame(const std::optional<std::string>& localFrame) { this->localFrame = localFrame; }
				/// <summary>Temporal unit of measure used to express the value of this data component.</summary>
				std::optional<UnitOfMeasure> getUnitOfMeasure() const { return unitOfMeasure; }
				/// <summary>Temporal unit of measure used to express the value of this data component.</summary>
				void setUnitOfMeasure(const std::optional<UnitOfMeasure>& unitOfMeasure) { this->unitOfMeasure = unitOfMeasure; }
			};

			// Register with the DataComponentRegistry
			struct TimeRangeRegistrar {
				TimeRangeRegistrar() {
					ConnectedSystemsAPI::DataModels::Component::DataComponentRegistry::registerType(
						"TimeRange", [](const nlohmann::json& j) {
						return std::make_unique<ConnectedSystemsAPI::DataModels::Component::TimeRange>(j.get<ConnectedSystemsAPI::DataModels::Component::TimeRange>());
					}
					);
				}
			};
			static TimeRangeRegistrar timeRangeRegistrar;

			inline void from_json(const nlohmann::json& j, TimeRange& v) {
				from_json(j, static_cast<SimpleComponent&>(v));

				if (j.contains("value") && j["value"].is_array())
					v.setValue(j["value"].get<std::vector<double>>());
				else
					v.setValue(std::nullopt);

				if (j.contains("referenceTime") && j["referenceTime"].is_string())
					v.setReferenceTime(j["referenceTime"].get<std::string>());
				else
					v.setReferenceTime(std::nullopt);

				if (j.contains("localFrame") && j["localFrame"].is_string())
					v.setLocalFrame(j["localFrame"].get<std::string>());
				else
					v.setLocalFrame(std::nullopt);

				if (j.contains("uom") && j["uom"].is_object())
					v.setUnitOfMeasure(j["uom"].get<UnitOfMeasure>());
				else
					v.setUnitOfMeasure(std::nullopt);
			}

			inline void to_json(nlohmann::ordered_json& j, const TimeRange& v) {
				to_json(j, static_cast<const SimpleComponent&>(v));
				if (v.getValue())
					j["value"] = v.getValue().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const TimeRange& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}