#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <vector>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "SimpleComponent.h"
#include "Util/JsonUtils.h"
#include "DataComponent.h"
#include "UnitOfMeasure.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class QuantityRange;
	void to_json(nlohmann::ordered_json& j, const QuantityRange& q);

	class QuantityRange : public SimpleComponent {
	private:
		std::optional<std::vector<double>> value;
		UnitOfMeasure unitOfMeasure;

	public:
		QuantityRange() = default;
		QuantityRange(const QuantityRange&) = default;
		QuantityRange(QuantityRange&&) noexcept = default;
		QuantityRange& operator=(const QuantityRange&) = default;
		QuantityRange& operator=(QuantityRange&&) noexcept = default;
		~QuantityRange() override = default;

		nlohmann::ordered_json toJson() const override {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		void validate() const override {
			SimpleComponent::validate();
			unitOfMeasure.validate();
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
		/// Unit of measure used to express the value of this data component.
		/// </summary>
		const UnitOfMeasure& getUnitOfMeasure() const noexcept { return unitOfMeasure; }
		void setUnitOfMeasure(const UnitOfMeasure& unitOfMeasure) { this->unitOfMeasure = unitOfMeasure; }
		void setUnitOfMeasure(UnitOfMeasure&& unitOfMeasure) noexcept { this->unitOfMeasure = std::move(unitOfMeasure); }

		friend void from_json(const nlohmann::json& j, QuantityRange& v);
		friend void to_json(nlohmann::ordered_json& j, const QuantityRange& v);

		friend bool operator==(const QuantityRange& a, const QuantityRange& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const QuantityRange& a, const QuantityRange& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const QuantityRange& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			return os << j.dump(2);
		}
	};

	const inline DataComponent::Registrar<QuantityRange> registerQuantityRange{ "QuantityRange" };

	inline void from_json(const nlohmann::json& j, QuantityRange& v) {
		from_json(j, static_cast<SimpleComponent&>(v));

		v.value = ConnectedSystemsAPI::JsonUtils::tryParseDoubleArray(j, "value");
		v.unitOfMeasure = j.at("uom").get<UnitOfMeasure>();
	}

	inline void to_json(nlohmann::ordered_json& j, const QuantityRange& v) {
		to_json(j, static_cast<const SimpleComponent&>(v));

		if (v.value) j["value"] = v.value.value();
		j["uom"] = v.unitOfMeasure;
	}
}