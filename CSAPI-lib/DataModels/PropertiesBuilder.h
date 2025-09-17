#pragma once

#include <string>
#include "TimeExtent.h"
#include "Properties.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class PropertiesBuilder {
		private:
			std::string featureType;
			std::string uid;
			std::string name;
			std::string description;
			std::string assetType;
			std::optional<TimeExtent> validTime;

		public:
			/// <summary>
			/// Identifier of the feature, either a URI, a CURIE, or a simple token.
			/// </summary>
			PropertiesBuilder& setFeatureType(const std::string& featureType) {
				this->featureType = featureType;
				return *this;
			}

			/// <summary>
			/// Globally unique identifier of the feature.
			/// </summary>
			PropertiesBuilder& setUid(const std::string& uid) {
				this->uid = uid;
				return *this;
			}

			/// <summary>
			/// Human-readable name of the feature.
			/// </summary>
			PropertiesBuilder& setName(const std::string& name) {
				this->name = name;
				return *this;
			}

			/// <summary>
			/// Human-readable description of the feature.
			/// </summary>
			PropertiesBuilder& setDescription(const std::string& description) {
				this->description = description;
				return *this;
			}

			/// <summary>
			/// Type of asset represented by this system.
			/// </summary>
			PropertiesBuilder& setAssetType(const std::string& assetType) {
				this->assetType = assetType;
				return *this;
			}

			/// <summary>
			/// Time period during which the system description is valid.
			/// </summary>
			PropertiesBuilder& setValidTime(const TimeExtent& validTime) {
				this->validTime = validTime;
				return *this;
			}

			/// <summary>
			/// Clear the valid time.
			/// </summary>
			PropertiesBuilder& setValidTime(std::nullopt_t) {
				this->validTime = std::nullopt;
				return *this;
			}

			Properties build() const {
				if (featureType.empty()) {
					throw std::runtime_error("FeatureType is required");
				}
				if (uid.empty()) {
					throw std::runtime_error("UID is required");
				}
				if (name.empty()) {
					throw std::runtime_error("Name is required");
				}
				return Properties(featureType, uid, name, description, assetType, validTime);
			}
		};
	}
}