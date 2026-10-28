#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "ScalarComponent.h"
#include "AllowedValues.h"
#include "UnitOfMeasure.h"
#include "../../Util/JsonUtils.h"
#include "DataComponent.h"
#include "SimpleComponent.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class Quantity;
	void to_json(nlohmann::ordered_json& j, const Quantity& q);

	class Quantity : public ScalarComponent {
	private:
		std::optional<AllowedValues> m_constraint;
		std::optional<double> m_value;
		UnitOfMeasure m_unitOfMeasure;

	public:
		Quantity() = default;
		Quantity(const Quantity&) = default;
		Quantity(Quantity&&) noexcept = default;
		Quantity& operator=(const Quantity&) = default;
		Quantity& operator=(Quantity&&) noexcept = default;
		~Quantity() override = default;

		nlohmann::ordered_json toJson() const override {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		void validate() const override {
			ScalarComponent::validate();
			m_unitOfMeasure.validate();
		}

		/// <summary>
		/// Inline value(s) for the component.
		/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)
		/// </summary>
		const std::optional<double>& getValue() const noexcept { return m_value; }
		void setValue(const std::optional<double>& value) noexcept { m_value = value; }
		void setValue(std::optional<double>&& value) noexcept { m_value = std::move(value); }
		void setValue(double value) noexcept { m_value = value; }
		bool hasValue() const noexcept { return m_value.has_value(); }
		void clearValue() noexcept { m_value.reset(); }

		/// <summary>
		/// Unit of measure used to express the value of this data component.
		/// </summary>
		const UnitOfMeasure& getUnitOfMeasure() const noexcept { return m_unitOfMeasure; }
		void setUnitOfMeasure(const UnitOfMeasure& unitOfMeasure) { m_unitOfMeasure = unitOfMeasure; }
		void setUnitOfMeasure(UnitOfMeasure&& unitOfMeasure) noexcept { m_unitOfMeasure = std::move(unitOfMeasure); }

		/// <summary>
		/// Constraint on the possible values of the component.
		/// </summary>
		const std::optional<AllowedValues>& getConstraint() const noexcept { return m_constraint; }
		void setConstraint(std::optional<AllowedValues> constraint) { m_constraint = std::move(constraint); }
		bool hasConstraint() const noexcept { return m_constraint.has_value(); }
		void clearConstraint() noexcept { m_constraint.reset(); }

		friend void from_json(const nlohmann::json& j, Quantity& v);
		friend void to_json(nlohmann::ordered_json& j, const Quantity& v);

		friend bool operator==(const Quantity& a, const Quantity& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const Quantity& a, const Quantity& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const Quantity& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			return os << j.dump(2);
		}
	};

	const inline DataComponent::Registrar<Quantity> registerQuantity{ "Quantity" };

	inline void from_json(const nlohmann::json& j, Quantity& v) {
		from_json(j, static_cast<ScalarComponent&>(v));

		if (j.contains("constraint") && j["constraint"].is_object())
			v.m_constraint = j["constraint"].get<AllowedValues>();
		else
			v.m_constraint.reset();

		v.m_value = ConnectedSystemsAPI::JsonUtils::tryParseDouble(j, "value");
		v.m_unitOfMeasure = j.at("uom").get<UnitOfMeasure>();
	}

	inline void to_json(nlohmann::ordered_json& j, const Quantity& v) {
		to_json(j, static_cast<const ScalarComponent&>(v));

		if (v.m_constraint) j["constraint"] = v.m_constraint->toJson();

		if (v.m_value) j["value"] = v.m_value.value();
		j["uom"] = v.m_unitOfMeasure;
	}
}