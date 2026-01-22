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
	class Quantity;
	void to_json(nlohmann::ordered_json& j, const Quantity& q);

	class Quantity : public ScalarComponent {
	private:
		std::optional<double> value;
		UnitOfMeasure unitOfMeasure;

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
			unitOfMeasure.validate();
		}

		/// <summary>
		/// Inline value(s) for the component.
		/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)
		/// </summary>
		const std::optional<double>& getValue() const noexcept { return value; }
		void setValue(const std::optional<double>& v) noexcept { value = v; }
		void setValue(std::optional<double>&& v) noexcept { value = std::move(v); }
		void setValue(double v) noexcept { value = v; }
		bool hasValue() const noexcept { return value.has_value(); }
		void clearValue() noexcept { value.reset(); }

		/// <summary>
		/// Unit of measure used to express the value of this data component.
		/// </summary>
		const UnitOfMeasure& getUnitOfMeasure() const noexcept { return unitOfMeasure; }
		void setUnitOfMeasure(const UnitOfMeasure& uom) { unitOfMeasure = uom; }
		void setUnitOfMeasure(UnitOfMeasure&& uom) noexcept { unitOfMeasure = std::move(uom); }

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

		v.value = ConnectedSystemsAPI::JsonUtils::tryParseDouble(j, "value");
		v.unitOfMeasure = j.at("uom").get<UnitOfMeasure>();
	}

	inline void to_json(nlohmann::ordered_json& j, const Quantity& v) {
		to_json(j, static_cast<const ScalarComponent&>(v));

		if (v.value) j["value"] = v.value.value();
		j["uom"] = v.unitOfMeasure;
	}
}