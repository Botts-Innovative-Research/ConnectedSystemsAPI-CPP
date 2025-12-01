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
			class Category;
			void to_json(nlohmann::ordered_json& j, const Category& q);

			class Category : public ScalarComponent {
			private:
				std::optional<std::string> value;
				std::optional<std::string> codeSpace;

			public:
				Category() = default;
				Category(const Category&) = default;
				Category(Category&&) noexcept = default;
				Category& operator=(const Category&) = default;
				Category& operator=(Category&&) noexcept = default;
				~Category() override = default;

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
				const std::optional<std::string>& getValue() const noexcept { return value; }
				void setValue(const std::optional<std::string>& v) { value = v; }
				void setValue(std::optional<std::string>&& v) { value = std::move(v); }
				void setValue(const std::string& v) { value = v; }
				void setValue(std::string&& v) { value = std::move(v); }
				void setValue(const char* v) { value = v ? std::optional<std::string>(v) : std::nullopt; }
				bool hasValue() const noexcept { return value.has_value(); }
				void clearValue() noexcept { value.reset(); }

				/// <summary>
				/// Name of the dictionary where the possible values for this component are listed and defined.
				/// </summary>
				const std::optional<std::string>& getCodeSpace() const noexcept { return codeSpace; }
				void setCodeSpace(const std::optional<std::string>& cs) { codeSpace = cs; }
				void setCodeSpace(std::optional<std::string>&& cs) { codeSpace = std::move(cs); }
				void setCodeSpace(const std::string& cs) { codeSpace = cs; }
				void setCodeSpace(std::string&& cs) { codeSpace = std::move(cs); }
				void setCodeSpace(const char* cs) { codeSpace = cs ? std::optional<std::string>(cs) : std::nullopt; }
				bool hasCodeSpace() const noexcept { return codeSpace.has_value(); }
				void clearCodeSpace() noexcept { codeSpace.reset(); }
			};

			inline DataComponent::Registrar<Category> registerCategory{ "Category" };
			inline bool operator==(const Category& a, const Category& b) { return a.toJson() == b.toJson(); }
			inline bool operator!=(const Category& a, const Category& b) { return !(a == b); }

			inline void from_json(const nlohmann::json& j, Category& v) {
				from_json(j, static_cast<ScalarComponent&>(v));

				v.setValue(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "value"));
				v.setCodeSpace(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "codeSpace"));
			}

			inline void to_json(nlohmann::ordered_json& j, const Category& v) {
				to_json(j, static_cast<const ScalarComponent&>(v));

				if (v.getValue()) j["value"] = v.getValue().value();
				if (v.getCodeSpace()) j["codeSpace"] = v.getCodeSpace().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const Category& v) {
				nlohmann::ordered_json j;
				to_json(j, v);
				return os << j.dump(2);
			}
		}
	}
}