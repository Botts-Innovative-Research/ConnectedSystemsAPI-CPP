#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "ScalarComponent.h"
#include "UnitOfMeasure.h"
#include "Util/JsonUtils.h"
#include "DataComponent.h"
#include "SimpleComponent.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class Time;
	void to_json(nlohmann::ordered_json& j, const Time& q);

	class Time : public ScalarComponent {
	private:
		std::optional<double> m_value;
		std::optional<std::string> m_referenceTime;
		std::optional<std::string> m_localFrame;
		std::optional<UnitOfMeasure> m_unitOfMeasure;

	public:
		Time() = default;
		Time(const Time&) = default;
		Time(Time&&) noexcept = default;
		Time& operator=(const Time&) = default;
		Time& operator=(Time&&) noexcept = default;
		~Time() override = default;

		void validate() const override {
			ScalarComponent::validate();
			if (m_unitOfMeasure)
				m_unitOfMeasure->validate();
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
		std::optional<double> getValue() const noexcept { return m_value; }
		void setValue(const std::optional<double>& value) { m_value = value; }
		void setValue(double value) { m_value = value; }
		bool hasValue() const noexcept { return m_value.has_value(); }
		void clearValue() noexcept { m_value.reset(); }

		/// <summary>
		/// Specifies the origin of the temporal reference frame as an ISO8601 date
		/// (used to specify time after an epoch that is to say in a custom frame)
		/// </summary>
		std::optional<std::string> getReferenceTime() const noexcept { return m_referenceTime; }
		void setReferenceTime(const std::optional<std::string>& referenceTime) { m_referenceTime = referenceTime; }
		void setReferenceTime(const std::string& referenceTime) { m_referenceTime = referenceTime; }
		bool hasReferenceTime() const noexcept { return m_referenceTime.has_value(); }
		void clearReferenceTime() noexcept { m_referenceTime.reset(); }

		/// <summary>
		/// Temporal frame of reference whose origin is located by the value of this component.
		/// </summary>
		std::optional<std::string> getLocalFrame() const noexcept { return m_localFrame; }
		void setLocalFrame(const std::optional<std::string>& localFrame) { m_localFrame = localFrame; }
		void setLocalFrame(const std::string& localFrame) { m_localFrame = localFrame; }
		bool hasLocalFrame() const noexcept { return m_localFrame.has_value(); }
		void clearLocalFrame() noexcept { m_localFrame.reset(); }

		/// <summary>
		/// Temporal unit of measure used to express the value of this data component.
		/// </summary>
		std::optional<UnitOfMeasure> getUnitOfMeasure() const noexcept { return m_unitOfMeasure; }
		void setUnitOfMeasure(const std::optional<UnitOfMeasure>& unitOfMeasure) { m_unitOfMeasure = unitOfMeasure; }
		void setUnitOfMeasure(UnitOfMeasure&& unitOfMeasure) noexcept { m_unitOfMeasure = std::move(unitOfMeasure); }
		bool hasUnitOfMeasure() const noexcept { return m_unitOfMeasure.has_value(); }
		void clearUnitOfMeasure() noexcept { m_unitOfMeasure.reset(); }

		friend void from_json(const nlohmann::json& j, Time& v);
		friend void to_json(nlohmann::ordered_json& j, const Time& v);

		friend bool operator==(const Time& a, const Time& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const Time& a, const Time& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const Time& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			return os << j.dump(2);
		}
	};

	const inline DataComponent::Registrar<Time> registerTime{ "Time" };

	inline void from_json(const nlohmann::json& j, Time& v) {
		from_json(j, static_cast<ScalarComponent&>(v));

		v.m_value = ConnectedSystemsAPI::JsonUtils::tryParseDouble(j, "value");
		v.m_referenceTime = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "referenceTime");
		v.m_localFrame = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "localFrame");

		if (j.contains("uom") && j["uom"].is_object())
			v.m_unitOfMeasure = j["uom"].get<UnitOfMeasure>();
		else
			v.m_unitOfMeasure = std::nullopt;
	}

	inline void to_json(nlohmann::ordered_json& j, const Time& v) {
		to_json(j, static_cast<const ScalarComponent&>(v));

		if (v.m_value) j["value"] = v.m_value.value();
		if (v.m_referenceTime) j["referenceTime"] = v.m_referenceTime.value();
		if (v.m_localFrame) j["localFrame"] = v.m_localFrame.value();
		if (v.m_unitOfMeasure) j["uom"] = v.m_unitOfMeasure.value();
	}
}