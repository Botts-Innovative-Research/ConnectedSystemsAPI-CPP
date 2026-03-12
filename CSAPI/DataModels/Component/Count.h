#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "ScalarComponent.h"
#include "Util/JsonUtils.h"
#include "DataComponent.h"
#include "SimpleComponent.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class Count;
	void to_json(nlohmann::ordered_json& j, const Count& q);

	class Count : public ScalarComponent {
	private:
		std::optional<int> value;

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
		const std::optional<int>& getValue() const noexcept { return value; }
		void setValue(std::optional<int> v) noexcept { value = std::move(v); }
		void setValue(int v) noexcept { value = v; }
		bool hasValue() const noexcept { return value.has_value(); }
		void clearValue() noexcept { value.reset(); }

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

		v.value = ConnectedSystemsAPI::JsonUtils::tryParseInteger(j, "value");
	}

	inline void to_json(nlohmann::ordered_json& j, const Count& v) {
		to_json(j, static_cast<const ScalarComponent&>(v));

		if (v.hasValue()) j["value"] = v.value.value();
	}
}