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
			class Text;
			void to_json(nlohmann::ordered_json& j, const Text& q);

			class Text : public ScalarComponent {
			private:
				std::optional<std::string> value;

			public:
				Text() = default;
				Text(const Text&) = default;
				Text(Text&&) noexcept = default;
				Text& operator=(const Text&) = default;
				Text& operator=(Text&&) noexcept = default;
				~Text() override = default;

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
				std::optional<std::string> getValue() const noexcept { return value; }
				void setValue(const std::optional<std::string>& value) { this->value = value; }
				void setValue(std::optional<std::string>&& value) noexcept { this->value = std::move(value); }
				void setValue(const std::string& value) { this->value = value; }
				void setValue(std::string&& value) noexcept { this->value = std::move(value); }
				bool hasValue() const noexcept { return value.has_value(); }
				void clearValue() noexcept { value.reset(); }
			};

			inline DataComponent::Registrar<Text> registerText{ "Text" };
			inline bool operator==(const Text& a, const Text& b) { return a.toJson() == b.toJson(); }
			inline bool operator!=(const Text& a, const Text& b) { return !(a == b); }

			inline void from_json(const nlohmann::json& j, Text& v) {
				from_json(j, static_cast<ScalarComponent&>(v));

				v.setValue(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "value"));
			}

			inline void to_json(nlohmann::ordered_json& j, const Text& v) {
				to_json(j, static_cast<const ScalarComponent&>(v));

				if (v.getValue()) j["value"] = v.getValue().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const Text& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}