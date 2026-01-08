#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <nlohmann/json.hpp>
#include "SimpleComponent.h"
#include "UnitOfMeasure.h"
#include "Util/JsonUtils.h"

namespace ConnectedSystemsAPI::DataModels::Component {
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
		TimeRange(const TimeRange&) = default;
		TimeRange(TimeRange&&) noexcept = default;
		TimeRange& operator=(const TimeRange&) = default;
		TimeRange& operator=(TimeRange&&) noexcept = default;
		~TimeRange() override = default;

		void validate() const {
			SimpleComponent::validate();
			if (unitOfMeasure)
				unitOfMeasure->validate();
		}

		nlohmann::ordered_json toJson() const override {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// Inline value(s) for the component.
		/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)
		/// </summary>
		std::optional<std::vector<double>> getValue() const noexcept { return value; }
		void setValue(const std::optional<std::vector<double>>& value) { this->value = value; }
		void setValue(std::optional<std::vector<double>>&& value)noexcept { this->value = std::move(value); }
		void setValue(const std::vector<double>& value) { this->value = value; }
		void setValue(std::vector<double>&& value)noexcept { this->value = std::move(value); }
		bool hasValue() const noexcept { return value.has_value(); }
		void clearValue() noexcept { value.reset(); }

		/// <summary>
		/// Specifies the origin of the temporal reference frame as an ISO8601 date
		/// (used to specify time after an epoch that is to say in a custom frame)
		/// </summary>
		std::optional<std::string> getReferenceTime() const noexcept { return referenceTime; }
		void setReferenceTime(const std::optional<std::string>& referenceTime) { this->referenceTime = referenceTime; }
		void setReferenceTime(const std::string& referenceTime) { this->referenceTime = referenceTime; }
		bool hasReferenceTime() const noexcept { return referenceTime.has_value(); }
		void clearReferenceTime() noexcept { referenceTime.reset(); }

		/// <summary>
		/// Temporal frame of reference whose origin is located by the value of this component.
		/// </summary>
		std::optional<std::string> getLocalFrame() const noexcept { return localFrame; }
		void setLocalFrame(const std::optional<std::string>& localFrame) { this->localFrame = localFrame; }
		void setLocalFrame(const std::string& localFrame) { this->localFrame = localFrame; }
		bool hasLocalFrame() const noexcept { return localFrame.has_value(); }
		void clearLocalFrame() noexcept { localFrame.reset(); }

		/// <summary>
		/// Temporal unit of measure used to express the value of this data component.
		/// </summary>
		std::optional<UnitOfMeasure> getUnitOfMeasure() const noexcept { return unitOfMeasure; }
		void setUnitOfMeasure(const std::optional<UnitOfMeasure>& unitOfMeasure) { this->unitOfMeasure = unitOfMeasure; }
		void setUnitOfMeasure(UnitOfMeasure&& unitOfMeasure) noexcept { this->unitOfMeasure = std::move(unitOfMeasure); }
		bool hasUnitOfMeasure() const noexcept { return unitOfMeasure.has_value(); }
		void clearUnitOfMeasure() noexcept { unitOfMeasure.reset(); }
	};

	inline DataComponent::Registrar<TimeRange> registerTimeRange{ "TimeRange" };
	inline bool operator==(const TimeRange& a, const TimeRange& b) { return a.toJson() == b.toJson(); }
	inline bool operator!=(const TimeRange& a, const TimeRange& b) { return !(a == b); }

	inline void from_json(const nlohmann::json& j, TimeRange& v) {
		from_json(j, static_cast<SimpleComponent&>(v));

		v.setValue(ConnectedSystemsAPI::JsonUtils::tryParseDoubleArray(j, "value"));
		v.setReferenceTime(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "referenceTime"));
		v.setLocalFrame(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "localFrame"));

		if (j.contains("uom") && j["uom"].is_object())
			v.setUnitOfMeasure(j["uom"].get<UnitOfMeasure>());
		else
			v.setUnitOfMeasure(std::nullopt);
	}

	inline void to_json(nlohmann::ordered_json& j, const TimeRange& v) {
		to_json(j, static_cast<const SimpleComponent&>(v));

		if (v.getValue()) j["value"] = v.getValue().value();
		if (v.getReferenceTime()) j["referenceTime"] = v.getReferenceTime().value();
		if (v.getLocalFrame()) j["localFrame"] = v.getLocalFrame().value();
		if (v.getUnitOfMeasure()) j["uom"] = v.getUnitOfMeasure().value();
	}

	inline std::ostream& operator<<(std::ostream& os, const TimeRange& v) {
		nlohmann::ordered_json j;
		to_json(j, v);
		return os << j.dump(2);
	}
}