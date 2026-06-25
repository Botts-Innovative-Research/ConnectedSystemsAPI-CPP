#pragma once

#include <string>
#include <optional>
#include <stdexcept>

namespace ConnectedSystemsAPI::DataModels::Component {
	template<typename Derived, typename Target>
	class DataComponentBuilder {
	protected:
		std::string m_type;
		std::optional<std::string> m_id;
		std::optional<std::string> m_name;
		std::optional<std::string> m_label;
		std::optional<std::string> m_description;
		std::optional<bool> m_updatable;
		std::optional<bool> m_optional;
		std::optional<std::string> m_definition;

		Derived& self() noexcept { return *reinterpret_cast<Derived*>(this); }
		const Derived& self() const noexcept { return *reinterpret_cast<const Derived*>(this); }

	public:
		DataComponentBuilder() = default;

		// Fluent setters for DataComponent base properties
		Derived& withType(const std::string& type) { m_type = type; return self(); }
		Derived& withId(const std::string& id) { m_id = id; return self(); }
		Derived& withName(const std::string& name) { m_name = name; return self(); }
		Derived& withLabel(const std::string& label) { m_label = label; return self(); }
		Derived& withDescription(const std::string& description) { m_description = description; return self(); }
		Derived& withUpdatable(bool updatable) { m_updatable = updatable; return self(); }
		Derived& withOptional(bool optional) { m_optional = optional; return self(); }
		Derived& withDefinition(const std::string& definition) { m_definition = definition; return self(); }

	protected:
		void validateBase() const {
			if (m_type.empty())
				throw std::runtime_error("DataComponentBuilder: type is required.");
		}

		void applyBase(Target& target) const {
			if (!m_type.empty()) target.setType(m_type);
			if (m_name) target.setName(m_name);
			if (m_id) target.setId(m_id);
			if (m_label) target.setLabel(m_label);
			if (m_description) target.setDescription(m_description);
			if (m_updatable) target.setUpdatable(m_updatable);
			if (m_optional) target.setOptional(m_optional);
			if (m_definition) target.setDefinition(m_definition);
		}
	};
}