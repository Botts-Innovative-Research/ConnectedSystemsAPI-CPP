#pragma once

#include <string>
#include <optional>
#include <vector>
#include <ostream>
#include <nlohmann/json.hpp>
#include "SimpleComponent.h"
#include "Util/JsonUtils.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class CountRange;
			void to_json(nlohmann::ordered_json& j, const CountRange& q);

			class CountRange : public SimpleComponent {
			private:
				std::optional<std::vector<int>> value;

			public:
				CountRange() = default;
				CountRange(const CountRange&) = default;
				CountRange(CountRange&&) noexcept = default;
				CountRange& operator=(const CountRange&) = default;
				CountRange& operator=(CountRange&&) noexcept = default;
				~CountRange() override = default;

				void validate() const {
					SimpleComponent::validate();
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
				const std::optional<std::vector<int>>& getValue() const noexcept { return value; }
				void setValue(const std::optional<std::vector<int>>& v) { value = v; }
				void setValue(std::optional<std::vector<int>>&& v) noexcept { value = std::move(v); }
				void setValue(const std::vector<int>& v) { value = v; }
				void setValue(std::vector<int>&& v) noexcept { value = std::move(v); }
				bool hasValue() const noexcept { return value.has_value(); }
				void clearValue() noexcept { value.reset(); }
			};

			inline DataComponent::Registrar<CountRange> registerCountRange{ "CountRange" };
			inline bool operator==(const CountRange& a, const CountRange& b) { return a.toJson() == b.toJson(); }
			inline bool operator!=(const CountRange& a, const CountRange& b) { return !(a == b); }

			inline void from_json(const nlohmann::json& j, CountRange& v) {
				from_json(j, static_cast<SimpleComponent&>(v));

				v.setValue(ConnectedSystemsAPI::JsonUtils::tryParseIntegerArray(j, "value"));
			}

			inline void to_json(nlohmann::ordered_json& j, const CountRange& v) {
				to_json(j, static_cast<const SimpleComponent&>(v));

				if (v.getValue()) j["value"] = v.getValue().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const CountRange& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}