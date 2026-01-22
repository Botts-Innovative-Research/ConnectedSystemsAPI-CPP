#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <vector>
#include <initializer_list>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "SimpleComponent.h"
#include "Util/JsonUtils.h"
#include "DataComponent.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class CategoryRange;
	void to_json(nlohmann::ordered_json& j, const CategoryRange& q);

	class CategoryRange : public SimpleComponent {
	private:
		std::optional<std::vector<std::string>> value;
		std::optional<std::string> codeSpace;

	public:
		CategoryRange() = default;
		CategoryRange(const CategoryRange&) = default;
		CategoryRange(CategoryRange&&) noexcept = default;
		CategoryRange& operator=(const CategoryRange&) = default;
		CategoryRange& operator=(CategoryRange&&) noexcept = default;
		~CategoryRange() override = default;

		nlohmann::ordered_json toJson() const override {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// Inline value(s) for the component.
		/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)
		/// </summary>
		const std::optional<std::vector<std::string>>& getValue() const noexcept { return value; }
		void setValue(std::optional<std::vector<std::string>> v) noexcept { value = std::move(v); }
		void setValue(std::vector<std::string> v) noexcept { value = std::move(v); }
		void setValue(std::initializer_list<std::string> il) { value = std::vector<std::string>(il); }
		bool hasValue() const noexcept { return value.has_value(); }
		void clearValue() noexcept { value = std::nullopt; }
		void addValue(const std::string& v) {
			if (!value) value = std::vector<std::string>{};
			value->push_back(v);
		}
		void addValue(std::string&& v) {
			if (!value) value = std::vector<std::string>{};
			value->push_back(std::move(v));
		}

		/// <summary>
		/// Name of the dictionary where the possible values for this component are listed and defined.
		/// </summary>
		const std::optional<std::string>& getCodeSpace() const noexcept { return codeSpace; }
		void setCodeSpace(std::optional<std::string> cs) noexcept { codeSpace = std::move(cs); }
		void setCodeSpace(std::string cs) { codeSpace = std::move(cs); }
		bool hasCodeSpace() const noexcept { return codeSpace.has_value(); }
		void clearCodeSpace() noexcept { codeSpace = std::nullopt; }

		friend void from_json(const nlohmann::json& j, CategoryRange& v);
		friend void to_json(nlohmann::ordered_json& j, const CategoryRange& v);

		friend bool operator==(const CategoryRange& a, const CategoryRange& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const CategoryRange& a, const CategoryRange& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const CategoryRange& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			return os << j.dump(2);
		}
	};

	const inline DataComponent::Registrar<CategoryRange> registerCategoryRange{ "CategoryRange" };

	inline void from_json(const nlohmann::json& j, CategoryRange& v) {
		from_json(j, static_cast<SimpleComponent&>(v));

		v.value = ConnectedSystemsAPI::JsonUtils::tryParseStringArray(j, "value");
		v.codeSpace = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "codeSpace");
	}

	inline void to_json(nlohmann::ordered_json& j, const CategoryRange& v) {
		to_json(j, static_cast<const SimpleComponent&>(v));

		if (v.hasValue()) j["value"] = v.value.value();
		if (v.hasCodeSpace()) j["codeSpace"] = v.codeSpace.value();
	}
}