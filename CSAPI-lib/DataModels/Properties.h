#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "TimeExtent.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class Properties {
		private:
			std::string featureType;
			std::string uid;
			std::string name;
			std::string description;
			std::string assetType;
			std::optional<TimeExtent> validTime;

		public:
			Properties() = default;
			Properties(const std::string& featureType,
				const std::string& uid,
				const std::string& name,
				const std::string& description,
				const std::string& assetType,
				const std::optional<TimeExtent>& validTime)
				: featureType(featureType), uid(uid), name(name), description(description), assetType(assetType), validTime(validTime) {
			}

			/// <returns>Identifier of the feature, either a URI, a CURIE, or a simple token.</returns>
			const std::string& getFeatureType() const { return featureType; }
			/// <returns>Globally unique identifier of the feature.</returns>
			const std::string& getUid() const { return uid; }
			/// <returns>Human-readable name of the feature.</returns>
			const std::string& getName() const { return name; }
			/// <returns>Human-readable description of the feature.</returns>
			const std::string& getDescription() const { return description; }
			/// <returns>Type of asset represented by this system.</returns>
			const std::string& getAssetType() const { return assetType; }
			/// <returns>Time period during which the system description is valid.</returns>
			const std::optional<TimeExtent>& getValidTime() const { return validTime; }
		};

		inline void from_json(const nlohmann::json& j, Properties& s) {
			s = Properties(
				j.at("featureType").get<std::string>(),
				j.at("uid").get<std::string>(),
				j.at("name").get<std::string>(),
				j.value("description", ""),
				j.value("assetType", ""),
				j.value("validTime", std::optional<TimeExtent>{})
			);
		}

		inline void to_json(nlohmann::ordered_json& j, const Properties& p) {
			j = nlohmann::ordered_json::object();
			j["featureType"] = p.getFeatureType();
			j["uid"] = p.getUid();
			j["name"] = p.getName();

			if (!p.getDescription().empty()) {
				j["description"] = p.getDescription();
			}
			if (!p.getAssetType().empty()) {
				j["assetType"] = p.getAssetType();
			}
			if (p.getValidTime()) {
				j["validTime"] = *p.getValidTime();
			}
		}

		inline std::ostream& operator<<(std::ostream& os, const Properties& p) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, p);
			return os << j.dump(2);
		}
	}
}