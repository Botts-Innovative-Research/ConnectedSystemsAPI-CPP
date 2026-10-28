#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "ScalarComponent.h"
#include "AllowedValues.h"
#include "../../Util/JsonUtils.h"
#include "DataComponent.h"
#include "SimpleComponent.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class Count;
	void to_json(nlohmann::ordered_json& j, const Count& q);

	class Count : public ScalarComponent {
	private:
		std::optional<AllowedValues> m_constraint;
		std::optional<int> m_value;

	public:
		Count() = default;
		Count(const Count&) = default;
		Count(Count&&) noexcept = default;
		Count& operator=(const Count&) = default;
		Count& operator=(Count&&) noexcept = default;
		~Count() override = default;

		nlohmann::ordered_json toJson() const override {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// Inline value(s) for the component.
		/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)
		/// </summary>
		const std::optional<int>& getValue() const noexcept { return m_value; }
		void setValue(std::optional<int> value) noexcept { m_value = std::move(value); }
		void setValue(int value) noexcept { m_value = value; }
		bool hasValue() const noexcept { return m_value.has_value(); }
		void clearValue() noexcept { m_value.reset(); }

		/// <summary>
		/// Constraint on the possible values of the component.
		/// </summary>
		const std::optional<AllowedValues>& getConstraint() const noexcept { return m_constraint; }
		void setConstraint(std::optional<AllowedValues> constraint) { m_constraint = std::move(constraint); }
		bool hasConstraint() const noexcept { return m_constraint.has_value(); }
		void clearConstraint() noexcept { m_constraint.reset(); }

		friend void from_json(const nlohmann::json& j, Count& v);
		friend void to_json(nlohmann::ordered_json& j, const Count& v);

		friend bool operator==(const Count& a, const Count& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const Count& a, const Count& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const Count& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			return os << j.dump(2);
		}
	};

	const inline DataComponent::Registrar<Count> registerCount{ "Count" };

	inline void from_json(const nlohmann::json& j, Count& v) {
		from_json(j, static_cast<ScalarComponent&>(v));

		if (j.contains("constraint") && j["constraint"].is_object())
			v.m_constraint = j["constraint"].get<AllowedValues>();
		else
			v.m_constraint.reset();

		v.m_value = ConnectedSystemsAPI::JsonUtils::tryParseInteger(j, "value");
	}

	inline void to_json(nlohmann::ordered_json& j, const Count& v) {
		to_json(j, static_cast<const ScalarComponent&>(v));

		if (v.m_constraint) j["constraint"] = v.m_constraint->toJson();

		if (v.hasValue()) j["value"] = v.m_value.value();
	}
}