#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class DataComponent {
			private:
				std::string type;
				std::optional<std::string> id;
				std::string label;
				std::optional<std::string> description;
				std::optional<bool> updatable;
				std::optional<bool> optional;
				std::string definition;

			public:
				virtual ~DataComponent() = default;
				virtual nlohmann::ordered_json toJson() const = 0;

				void validate() const {
					if (type.empty())
						throw std::invalid_argument("DataComponent: type is required.");
					if (label.empty())
						throw std::invalid_argument("DataComponent: label is required.");
				}

				/// <summary>Type of the component.</summary>
				const std::string& getType() const { return type; }
				/// <summary>Type of the component.</summary>
				void setType(const std::string& type) { this->type = type; }

				/// <summary>The ID of the object, referenceable using a URI fragment.</summary>
				const std::optional<std::string> getId() const { return id; }
				/// <summary>The ID of the object, referenceable using a URI fragment.</summary>
				void setId(const std::optional<std::string> id) { this->id = id; }

				/// <summary>Human-readable label for the object.</summary>
				const std::string& getLabel() const { return label; }
				/// <summary>Human-readable label for the object.</summary>
				void setLabel(const std::string& label) { this->label = label; }

				/// <summary>Human-readable description of the object.</summary>
				const std::optional<std::string> getDescription() const { return description; }
				/// <summary>Human-readable description of the object.</summary>
				void setDescription(const std::optional<std::string> description) { this->description = description; }

				/// <summary>Specifies if the value of a data component can be updated externally (i.e., is variable).</summary>
				const std::optional<bool>& isUpdatable() const { return updatable; }
				/// <summary>Specifies if the value of a data component can be updated externally (i.e., is variable).</summary>
				void setUpdatable(const std::optional<bool>& updatable) { this->updatable = updatable; }

				/// <summary>Specifies if the data for this component can be omitted in the data stream.</summary>
				const std::optional<bool>& isOptional() const { return optional; }
				/// <summary>Specifies if the data for this component can be omitted in the data stream.</summary>
				void setOptional(const std::optional<bool>& optional) { this->optional = optional; }

				/// <summary>The definition of the property whose value is provided by this component (semantic link).</summary>
				const std::string& getDefinition() const { return definition; }
				/// <summary>The definition of the property whose value is provided by this component (semantic link).</summary>
				void setDefinition(const std::string& definition) { this->definition = definition; }
			};

			inline void from_json(const nlohmann::json& j, DataComponent& c) {
				if (j.contains("type"))
					c.setType(j.at("type").get<std::string>());
				if (j.contains("id"))
					c.setId(j.at("id").get<std::string>());
				if (j.contains("label"))
					c.setLabel(j.at("label").get<std::string>());
				if (j.contains("description"))
					c.setDescription(j.at("description").get<std::string>());
				if (j.contains("updatable"))
					c.setUpdatable(j.at("updatable").get<std::optional<bool>>());
				if (j.contains("optional"))
					c.setOptional(j.at("optional").get<std::optional<bool>>());
				if (j.contains("definition"))
					c.setDefinition(j.at("definition").get<std::string>());
			}

			inline void to_json(nlohmann::ordered_json& j, const DataComponent& c) {
				j = nlohmann::ordered_json::object();
				j["type"] = c.getType();
				if (c.getId())
					j["id"] = c.getId();
				j["label"] = c.getLabel();
				if (c.getDescription())
					j["description"] = c.getDescription();
				if (c.isUpdatable())
					j["updatable"] = c.isUpdatable().value();
				if (c.isOptional())
					j["optional"] = c.isOptional().value();
				j["definition"] = c.getDefinition();
			}

			inline std::ostream& operator<<(std::ostream& os, const DataComponent& c) {
				nlohmann::ordered_json j;
				to_json(j, c);
				return os << j.dump(2);
			}
		}
	}
}