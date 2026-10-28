#pragma once

#include <string>
#include <optional>
#include <vector>
#include <ostream>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "SimpleComponent.h"
#include "AllowedValues.h"
#include "../../Util/JsonUtils.h"
#include "DataComponent.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class CountRange;
	void to_json(nlohmann::ordered_json& j, const CountRange& q);

	class CountRange : public SimpleComponent {
	private:
		std::optional<AllowedValues> m_constraint;
		std::optional<std::vector<int>> m_value;

	public:
		CountRange() = default;
		CountRange(const CountRange&) = default;
		CountRange(CountRange&&) noexcept = default;
		CountRange& operator=(const CountRange&) = default;
		CountRange& operator=(CountRange&&) noexcept = default;
		~CountRange() override = default;

		nlohmann::ordered_json toJson() const override {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// Inline value(s) for the component.
		/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)
		/// </summary>
		const std::optional<std::vector<int>>& getValue() const noexcept { return m_value; }
		void setValue(const std::optional<std::vector<int>>& value) { m_value = value; }
		void setValue(std::optional<std::vector<int>>&& value) noexcept { m_value = std::move(value); }
		void setValue(const std::vector<int>& value) { m_value = value; }
		void setValue(std::vector<int>&& value) noexcept { m_value = std::move(value); }
		bool hasValue() const noexcept { return m_value.has_value(); }
		void clearValue() noexcept { m_value.reset(); }

		/// <summary>
		/// Constraint on the possible values of the component.
		/// </summary>
		const std::optional<AllowedValues>& getConstraint() const noexcept { return m_constraint; }
		void setConstraint(std::optional<AllowedValues> constraint) { m_constraint = std::move(constraint); }
		bool hasConstraint() const noexcept { return m_constraint.has_value(); }
		void clearConstraint() noexcept { m_constraint.reset(); }

		friend void from_json(const nlohmann::json& j, CountRange& v);
		friend void to_json(nlohmann::ordered_json& j, const CountRange& v);

		friend bool operator==(const CountRange& a, const CountRange& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const CountRange& a, const CountRange& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const CountRange& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			return os << j.dump(2);
		}
	};

	const inline DataComponent::Registrar<CountRange> registerCountRange{ "CountRange" };

	inline void from_json(const nlohmann::json& j, CountRange& v) {
		from_json(j, static_cast<SimpleComponent&>(v));

		if (j.contains("constraint") && j["constraint"].is_object())
			v.m_constraint = j["constraint"].get<AllowedValues>();
		else
			v.m_constraint.reset();

		v.m_value = ConnectedSystemsAPI::JsonUtils::tryParseIntegerArray(j, "value");
	}

	inline void to_json(nlohmann::ordered_json& j, const CountRange& v) {
		to_json(j, static_cast<const SimpleComponent&>(v));

		if (v.m_constraint) j["constraint"] = v.m_constraint->toJson();

		if (v.hasValue()) j["value"] = v.m_value.value();
	}
}