#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

namespace ConnectedSystemsAPI::DataModels {
	class ControlledProperty {
	private:
		std::optional<std::string> definition;
		std::optional<std::string> label;
		std::optional<std::string> description;

	public:
		ControlledProperty() = default;
		ControlledProperty(const std::optional<std::string>& definition,
			const std::optional<std::string>& label,
			const std::optional<std::string>& description)
			: definition(definition), label(label), description(description) {
		}

		/// <summary>
		/// Definition of the observed property.
		/// </summary>
		const std::optional<std::string>& getDefinition() const { return definition; }
		/// <summary>
		/// Human-readable label of the observed property.
		/// </summary>
		const std::optional<std::string>& getLabel() const { return label; }
		/// <summary>
		/// Human-readable description of the observed property.
		/// </summary>
		const std::optional<std::string>& getDescription() const { return description; }

		friend void from_json(const nlohmann::json& j, ControlledProperty& v);
		friend void to_json(nlohmann::ordered_json& j, const ControlledProperty& v);

		friend bool operator==(const ControlledProperty& a, const ControlledProperty& b) {
			return a.definition == b.definition
				&& a.label == b.label
				&& a.description == b.description;
		}

		friend bool operator!=(const ControlledProperty& a, const ControlledProperty& b) {
			return !(a == b);
		}

		friend std::ostream& operator<<(std::ostream& os, const ControlledProperty& v) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, v);
			os << j.dump(2);
			return os;
		}
	};

	inline void from_json(const nlohmann::json& j, ControlledProperty& v) {
		v.definition = j.value("definition", std::optional<std::string>{});
		v.label = j.value("label", std::optional<std::string>{});
		v.description = j.value("description", std::optional<std::string>{});
	}

	inline void to_json(nlohmann::ordered_json& j, const ControlledProperty& v) {
		j = nlohmann::ordered_json::object();

		if (v.definition.has_value()) j["definition"] = v.definition.value();
		if (v.label.has_value()) j["label"] = v.label.value();
		if (v.description.has_value()) j["description"] = v.description.value();
	}
}