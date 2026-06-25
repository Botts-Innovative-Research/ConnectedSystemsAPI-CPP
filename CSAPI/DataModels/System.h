#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Properties.h"
#include "Link.h"

namespace ConnectedSystemsAPI::DataModels {
	class System;
	void to_json(nlohmann::ordered_json& j, const System& v);

	class System {
	private:
		std::string type;
		std::string id;
		Properties properties;
		std::optional<std::vector<double>> bbox;
		std::optional<std::vector<Link>> links;

	public:
		System() = default;
		System(const std::string& type,
			const std::string& id,
			const Properties& properties,
			const std::optional<std::vector<double>>& bbox,
			const std::optional<std::vector<Link>>& links)
			: type(type), id(id), properties(properties), bbox(bbox), links(links) {
		}

		const std::string& getType() const { return type; }
		/// <returns>Local ID of the feature.</returns>
		const std::string& getId() const { return id; }
		/// <returns>Feature properties.</returns>
		const Properties& getProperties() const { return properties; }
		/// <returns>Optional bounding box for the feature.</returns>
		const std::optional<std::vector<double>>& getBbox() const { return bbox; }
		/// <returns>Links to related resources.</returns>
		const std::optional<std::vector<Link>>& getLinks() const { return links; }

		friend void from_json(const nlohmann::json& j, System& s);
		friend void to_json(nlohmann::ordered_json& j, const System& s);

		friend std::ostream& operator<<(std::ostream& os, const System& s) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, s);
			return os << j.dump(2);
		}
	};

	inline void from_json(const nlohmann::json& j, System& s) {
		s.type = j.at("type").get<std::string>();
		s.id = j.at("id").get<std::string>();
		s.properties = j.at("properties").get<Properties>();

		if (j.contains("bbox") && !j["bbox"].is_null())
			s.bbox = j["bbox"].get<std::vector<double>>();
		else
			s.bbox.reset();

		if (j.contains("links") && !j["links"].is_null())
			s.links = j["links"].get<std::vector<Link>>();
		else
			s.links.reset();
	}

	inline void to_json(nlohmann::ordered_json& j, const System& s) {
		j = nlohmann::ordered_json::object();
		j["type"] = s.getType();
		j["id"] = s.getId();
		j["properties"] = s.getProperties();
		if (s.getBbox()) j["bbox"] = s.getBbox().value();
		if (s.getLinks()) j["links"] = s.getLinks().value();
	}
}