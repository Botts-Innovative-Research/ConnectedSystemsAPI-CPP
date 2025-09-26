#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class ObservedProperty {
		private:
			std::string definition;
			std::string label;
			std::string description;

		public:
			ObservedProperty() = default;
			ObservedProperty(const std::string& definition,
				const std::string& label,
				const std::string& description)
				: definition(definition), label(label), description(description) {
			}

			/// <returns>Definition of the observed property.</returns>
			const std::string& getDefinition() const { return definition; }
			/// <returns>Human-readable label of the observed property.</returns>
			const std::string& getLabel() const { return label; }
			/// <returns>Human-readable description of the observed property.</returns>
			const std::string& getDescription() const { return description; }
		};

		inline void from_json(const nlohmann::json& j, ObservedProperty& o) {
			o = ObservedProperty(
				j.value("definition", ""),
				j.value("label", ""),
				j.value("description", "")
			);
		}

		inline void to_json(nlohmann::ordered_json& j, const ObservedProperty& o) {
			j = nlohmann::ordered_json::object();

			if (!o.getDefinition().empty())
				j["definition"] = o.getDefinition();

			j["label"] = o.getLabel();

			if (!o.getDescription().empty())
				j["description"] = o.getDescription();
		}

		inline std::ostream& operator<<(std::ostream& os, const ObservedProperty& o) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, o);
			os << j.dump(2);
		}
	}
}