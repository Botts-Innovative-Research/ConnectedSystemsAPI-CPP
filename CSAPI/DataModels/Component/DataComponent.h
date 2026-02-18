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
		std::string type;
		std::optional<std::string> id;
		std::optional<std::string> name;
		std::optional<std::string> label;
		std::optional<std::string> description;
		std::optional<bool> updatable;
		std::optional<bool> optional;
		std::optional<std::string> definition;

	protected:
		DataComponent() = default;

	public:
		virtual ~DataComponent() = default;
		virtual nlohmann::ordered_json toJson() const = 0;

		virtual	void validate() const {
			if (type.empty())
				throw std::invalid_argument("DataComponent.type is required.");
		}

		/// <summary>
		/// Type of the component.
		/// </summary>
		const std::string& getType() const noexcept { return type; }
		void setType(std::string t) noexcept { type = std::move(t); }

		/// <summary>
		/// Name of the component.
		/// </summary>
		const std::optional<std::string>& getName() const noexcept { return name; }
		void setName(std::optional<std::string> n) noexcept { name = std::move(n); }
		void clearName() noexcept { name.reset(); }

		/// <summary>
		/// The ID of the object, referenceable using a URI fragment.
		/// </summary>
		const std::optional<std::string>& getId() const noexcept { return id; }
		void setId(std::optional<std::string> i) noexcept { id = std::move(i); }
		void clearId() noexcept { id.reset(); }

		/// <summary>
		/// Human-readable label for the object.
		/// </summary>
		const std::optional<std::string>& getLabel() const noexcept { return label; }
		void setLabel(std::optional<std::string> l) noexcept { label = std::move(l); }
		void clearLabel() noexcept { label.reset(); }

		/// <summary>
		/// Human-readable description of the object.
		/// </summary>
		const std::optional<std::string>& getDescription() const noexcept { return description; }
		void setDescription(std::optional<std::string> d) noexcept { description = std::move(d); }
		void clearDescription() noexcept { description.reset(); }

		/// <summary>
		/// Specifies if the value of a data component can be updated externally (i.e., is variable).
		/// </summary>
		const std::optional<bool>& isUpdatable() const noexcept { return updatable; }
		void setUpdatable(std::optional<bool> u) noexcept { updatable = std::move(u); }
		void clearUpdatable() noexcept { updatable.reset(); }

		/// <summary>
		/// Specifies if the data for this component can be omitted in the data stream.
		/// </summary>
		const std::optional<bool>& isOptional() const noexcept { return optional; }
		void setOptional(std::optional<bool> o) noexcept { optional = std::move(o); }
		void clearOptional() noexcept { optional.reset(); }

		/// <summary>
		/// The definition of the property whose value is provided by this component (semantic link).
		/// </summary>
		const std::optional<std::string>& getDefinition() const noexcept { return definition; }
		void setDefinition(std::optional<std::string> d) noexcept { definition = std::move(d); }
		void clearDefinition() noexcept { definition.reset(); }

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