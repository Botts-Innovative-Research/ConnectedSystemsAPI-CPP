#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <nlohmann/json.hpp>
#include "ScalarComponent.h"
#include "Util/JsonUtils.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
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

				void validate() const override {
					ScalarComponent::validate();
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
				const std::optional<int>& getValue() const noexcept { return value; }
				void setValue(std::optional<int> v) noexcept { value = std::move(v); }
				void setValue(int v) noexcept { value = v; }
				bool hasValue() const noexcept { return value.has_value(); }
				void clearValue() noexcept { value.reset(); }
			};

			inline DataComponent::Registrar<Count> registerCount{ "Count" };
			inline bool operator==(const Count& a, const Count& b) { return a.toJson() == b.toJson(); }
			inline bool operator!=(const Count& a, const Count& b) { return !(a == b); }

			inline void from_json(const nlohmann::json& j, Count& v) {
				from_json(j, static_cast<ScalarComponent&>(v));

				v.setValue(ConnectedSystemsAPI::JsonUtils::tryParseInteger(j, "value"));
			}

			inline void to_json(nlohmann::ordered_json& j, const Count& v) {
				to_json(j, static_cast<const ScalarComponent&>(v));

				if (v.getValue()) j["value"] = v.getValue().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const Count& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}