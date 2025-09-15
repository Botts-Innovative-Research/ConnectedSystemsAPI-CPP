#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "Properties.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class System {
		private:
			std::string type;
			std::string id;
			Properties properties;
		public:
			System() = default;
			System(const std::string& type, const std::string& id, const Properties& properties)
				: type(type), id(id), properties(properties) {
			}

			const std::string& getType() const { return type; }
			const std::string& getId() const { return id; }
			const Properties& getProperties() const { return properties; }
		};

		inline void from_json(const nlohmann::json& j, System& s) {
			s = System(
				j.at("type").get<std::string>(),
				j.at("id").get<std::string>(),
				j.at("properties").get<Properties>()
			);
		}

		inline void to_json(nlohmann::ordered_json& j, const System& s) {
			j = nlohmann::ordered_json{
				{"type", s.getType()},
				{"id", s.getId()},
				{"properties", s.getProperties()}
			};
		}

		inline std::ostream& operator<<(std::ostream& os, const System& s) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, s);
			return os << j.dump(4);
		}

	}
}