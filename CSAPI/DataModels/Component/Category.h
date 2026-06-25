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
	class Category;
	void to_json(nlohmann::ordered_json& j, const Category& q);

	class Category : public ScalarComponent {
	private:
		std::optional<std::string> m_value;
		std::optional<std::string> m_codeSpace;

	public:
		Category() = default;
		Category(const Category&) = default;
		Category(Category&&) noexcept = default;
		Category& operator=(const Category&) = default;
		Category& operator=(Category&&) noexcept = default;
		~Category() override = default;

		nlohmann::ordered_json toJson() const override {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// Inline value(s) for the component.
		/// This property is optional to enable structure to act as a schema for values provided separately (e.g., in a datastream)
		/// </summary>
		const std::optional<std::string>& getValue() const noexcept { return m_value; }
		void setValue(std::optional<std::string> value) { m_value = std::move(value); }
		bool hasValue() const noexcept { return m_value.has_value(); }
		void clearValue() noexcept { m_value.reset(); }

		/// <summary>
		/// Name of the dictionary where the possible values for this component are listed and defined.
		/// </summary>
		const std::optional<std::string>& getCodeSpace() const noexcept { return m_codeSpace; }
		void setCodeSpace(std::optional<std::string> codeSpace) { m_codeSpace = std::move(codeSpace); }
		bool hasCodeSpace() const noexcept { return m_codeSpace.has_value(); }
		void clearCodeSpace() noexcept { m_codeSpace.reset(); }

		friend void from_json(const nlohmann::json& j, Category& v);
		friend void to_json(nlohmann::ordered_json& j, const Category& v);

		friend bool operator==(const Category& a, const Category& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const Category& a, const Category& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const Category& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			return os << j.dump(2);
		}
	};

	const inline DataComponent::Registrar<Category> registerCategory{ "Category" };

	inline void from_json(const nlohmann::json& j, Category& v) {
		from_json(j, static_cast<ScalarComponent&>(v));

		v.m_value = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "value");
		v.m_codeSpace = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "codeSpace");
	}

	inline void to_json(nlohmann::ordered_json& j, const Category& v) {
		to_json(j, static_cast<const ScalarComponent&>(v));

		if (v.hasValue()) j["value"] = v.m_value.value();
		if (v.hasCodeSpace()) j["codeSpace"] = v.m_codeSpace.value();
	}
}