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
			class Boolean;
			void to_json(nlohmann::ordered_json& j, const Boolean& v);

			class Boolean : public ScalarComponent {
			private:
				std::optional<bool> value = std::nullopt;

			public:
				Boolean() = default;
				Boolean(const Boolean&) = default;
				Boolean(Boolean&&) noexcept = default;
				Boolean& operator=(const Boolean&) = default;
				Boolean& operator=(Boolean&&) noexcept = default;
				~Boolean() override = default;

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
				const std::optional<bool>& getValue() const noexcept { return value; }
				void setValue(std::optional<bool> v) noexcept { value = std::move(v); }
				void setValue(bool v) noexcept { value = v; }
				bool hasValue() const noexcept { return value.has_value(); }
				void clearValue() noexcept { value.reset(); }
			};

			inline DataComponent::Registrar<Boolean> registerBoolean{ "Boolean" };
			inline bool operator==(const Boolean& a, const Boolean& b) { return a.toJson() == b.toJson(); }
			inline bool operator!=(const Boolean& a, const Boolean& b) { return !(a == b); }

			inline void from_json(const nlohmann::json& j, Boolean& v) {
				from_json(j, static_cast<ScalarComponent&>(v));

				v.setValue(ConnectedSystemsAPI::JsonUtils::tryParseBoolean(j, "value"));
			}

			inline void to_json(nlohmann::ordered_json& j, const Boolean& v) {
				to_json(j, static_cast<const ScalarComponent&>(v));

				if (v.getValue()) j["value"] = v.getValue().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const Boolean& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}