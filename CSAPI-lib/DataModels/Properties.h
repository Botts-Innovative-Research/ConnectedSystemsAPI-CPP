#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "TimeExtent.h"
#include "Link.h"

namespace ConnectedSystemsAPI::DataModels {
	class Properties;
	void to_json(nlohmann::ordered_json& j, const Properties& v);

	class Properties {
	private:
		std::string featureType;
		std::string uid;
		std::string name;
		std::optional<std::string> description;
		std::optional<std::string> assetType;
		std::optional<TimeExtent> validTime;
		std::optional<Link> systemKind;

	public:
		Properties() = default;
		Properties(const std::string& featureType,
			const std::string& uid,
			const std::string& name,
			const std::optional<std::string>& description = std::nullopt,
			const std::optional<std::string>& assetType = std::nullopt,
			const std::optional<TimeExtent>& validTime = std::nullopt,
			const std::optional<Link>& systemKind = std::nullopt)
			: featureType(featureType), uid(uid), name(name), description(description), assetType(assetType), validTime(validTime), systemKind(systemKind) {
		}

		/// <summary>
		/// Identifier of the feature, either a URI, a CURIE, or a simple token.
		/// </summary>
		const std::string& getFeatureType() const { return featureType; }
		/// <summary>
		/// Globally unique identifier of the feature.
		/// </summary>
		const std::string& getUid() const { return uid; }
		/// <summary>
		/// Human-readable name of the feature.
		/// </summary>
		const std::string& getName() const { return name; }
		/// <summary>
		/// Human-readable description of the feature.
		/// </summary>
		const std::optional<std::string>& getDescription() const { return description; }
		/// <summary>
		/// Type of asset represented by this system.
		/// </summary>
		const std::optional<std::string>& getAssetType() const { return assetType; }
		/// <summary>
		/// Time period during which the system description is valid.
		/// </summary>
		const std::optional<TimeExtent>& getValidTime() const { return validTime; }
		/// <summary>
		/// Link to the system kind description (i.e., its nature or specifications).
		/// </summary>
		const std::optional<Link>& getSystemKind() const { return systemKind; }

		friend void from_json(const nlohmann::json& j, Properties& p);
		friend void to_json(nlohmann::ordered_json& j, const Properties& p);

		friend std::ostream& operator<<(std::ostream& os, const Properties& p) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, p);
			return os << j.dump(2);
		}
	};

	inline void from_json(const nlohmann::json& j, Properties& p) {
		p.featureType = j.at("featureType").get<std::string>();
		p.uid = j.at("uid").get<std::string>();
		p.name = j.at("name").get<std::string>();
		p.description = j.value("description", std::optional<std::string>{});
		p.assetType = j.value("assetType", std::optional<std::string>{});
		p.validTime = j.value("validTime", std::optional<TimeExtent>{});
		p.systemKind = j.value("systemKind@link", std::optional<Link>{});
	}

	inline void to_json(nlohmann::ordered_json& j, const Properties& p) {
		j = nlohmann::ordered_json::object();

		j["featureType"] = p.featureType;
		j["uid"] = p.uid;
		j["name"] = p.name;
		if (p.description.has_value()) { j["description"] = p.description.value(); }
		if (p.assetType.has_value()) { j["assetType"] = p.assetType.value(); }
		if (p.validTime.has_value()) { j["validTime"] = p.validTime.value(); }
		if (p.systemKind.has_value()) { j["systemKind@link"] = p.systemKind.value(); }
	}
}