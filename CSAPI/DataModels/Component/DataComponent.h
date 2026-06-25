#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "../../Util/JsonUtils.h"
#include "DataComponentRegistry.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class DataComponent;
	void to_json(nlohmann::ordered_json& j, const DataComponent& v);

	/// <summary>
	/// Base class for all data components.
	/// Derived types must:
	/// - Implement the toJson() method for serialization.
	/// - Use the DataComponent::Registrar to register themselves with the DataComponentRegistry for polymorphic deserialization.
	/// - Implement from_json and to_json functions for JSON (de)serialization.
	/// - Be added to RegistryInit.h to ensure registration occurs if they aren't directly referenced in the codebase.
	/// </summary>
	class DataComponent {
	private:
		std::string m_type;
		std::optional<std::string> m_id;
		std::optional<std::string> m_name;
		std::optional<std::string> m_label;
		std::optional<std::string> m_description;
		std::optional<bool> m_updatable;
		std::optional<bool> m_optional;
		std::optional<std::string> m_definition;

	protected:
		DataComponent() = default;

	public:
		virtual ~DataComponent() = default;
		virtual nlohmann::ordered_json toJson() const = 0;

		virtual	void validate() const {
			if (m_type.empty())
				throw std::invalid_argument("DataComponent.type is required.");
		}

		/// <summary>
		/// Type of the component.
		/// </summary>
		const std::string& getType() const noexcept { return m_type; }
		void setType(std::string type) noexcept { m_type = std::move(type); }

		/// <summary>
		/// Name of the component.
		/// </summary>
		const std::optional<std::string>& getName() const noexcept { return m_name; }
		void setName(std::optional<std::string> name) noexcept { m_name = std::move(name); }
		void clearName() noexcept { m_name.reset(); }

		/// <summary>
		/// The ID of the object, referenceable using a URI fragment.
		/// </summary>
		const std::optional<std::string>& getId() const noexcept { return m_id; }
		void setId(std::optional<std::string> id) noexcept { m_id = std::move(id); }
		void clearId() noexcept { m_id.reset(); }

		/// <summary>
		/// Human-readable label for the object.
		/// </summary>
		const std::optional<std::string>& getLabel() const noexcept { return m_label; }
		void setLabel(std::optional<std::string> label) noexcept { m_label = std::move(label); }
		void clearLabel() noexcept { m_label.reset(); }

		/// <summary>
		/// Human-readable description of the object.
		/// </summary>
		const std::optional<std::string>& getDescription() const noexcept { return m_description; }
		void setDescription(std::optional<std::string> description) noexcept { m_description = std::move(description); }
		void clearDescription() noexcept { m_description.reset(); }

		/// <summary>
		/// Specifies if the value of a data component can be updated externally (i.e., is variable).
		/// </summary>
		const std::optional<bool>& isUpdatable() const noexcept { return m_updatable; }
		void setUpdatable(std::optional<bool> updatable) noexcept { m_updatable = std::move(updatable); }
		void clearUpdatable() noexcept { m_updatable.reset(); }

		/// <summary>
		/// Specifies if the data for this component can be omitted in the data stream.
		/// </summary>
		const std::optional<bool>& isOptional() const noexcept { return m_optional; }
		void setOptional(std::optional<bool> optional) noexcept { m_optional = std::move(optional); }
		void clearOptional() noexcept { m_optional.reset(); }

		/// <summary>
		/// The definition of the property whose value is provided by this component (semantic link).
		/// </summary>
		const std::optional<std::string>& getDefinition() const noexcept { return m_definition; }
		void setDefinition(std::optional<std::string> definition) noexcept { m_definition = std::move(definition); }
		void clearDefinition() noexcept { m_definition.reset(); }

		/// <summary>
		/// Registrar for DataComponent derived types.
		/// All derived types must have a static instance of this registrar to register themselves with the DataComponentRegistry.
		/// This enables polymorphic deserialization based on the "type" field in the JSON.
		/// </summary>
		template<typename T>
		struct Registrar {
			explicit Registrar(const std::string& typeName) {
				ConnectedSystemsAPI::DataModels::Component::DataComponentRegistry::registerType(
					typeName, [](const nlohmann::json& j) {
					// Deserialize a T from json and return a polymorphic unique_ptr
					return std::make_unique<T>(j.get<T>());
				}
				);
			}
		};

		friend std::ostream& operator<<(std::ostream& os, const DataComponent& c) {
			nlohmann::ordered_json j;
			to_json(j, c);
			return os << j.dump(2);
		}
	};

	inline void from_json(const nlohmann::json& j, DataComponent& v) {
		v.setType(j.at("type").get<std::string>());
		v.setName(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "name"));
		v.setId(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "id"));
		v.setLabel(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "label"));
		v.setDescription(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "description"));
		v.setUpdatable(ConnectedSystemsAPI::JsonUtils::tryParseBoolean(j, "updatable"));
		v.setOptional(ConnectedSystemsAPI::JsonUtils::tryParseBoolean(j, "optional"));
		v.setDefinition(ConnectedSystemsAPI::JsonUtils::tryParseString(j, "definition"));
	}

	inline void to_json(nlohmann::ordered_json& j, const DataComponent& c) {
		j = nlohmann::ordered_json::object();
		j["type"] = c.getType();
		if (c.getName()) j["name"] = c.getName().value();
		if (c.getId()) j["id"] = c.getId().value();
		if (c.getLabel()) j["label"] = c.getLabel().value();
		if (c.getDescription()) j["description"] = c.getDescription().value();
		if (c.isUpdatable()) j["updatable"] = c.isUpdatable().value();
		if (c.isOptional()) j["optional"] = c.isOptional().value();
		if (c.getDefinition()) j["definition"] = c.getDefinition().value();
	}
}