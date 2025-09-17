#pragma once

#include <string>
#include <vector>
#include <optional>
#include "Link.h"
#include "Properties.h"
#include "System.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class SystemBuilder {
		private:
			std::string id;
			std::optional<Properties> properties;
			std::optional<std::vector<double>> bbox;
			std::optional<std::vector<Link>> links;

		public:
			/// <summary>
			/// Local ID of the feature (ignored on create or update).
			/// </summary>
			SystemBuilder& setId(const std::string& id) {
				this->id = id;
				return *this;
			}

			/// <summary>
			/// Feature properties.
			/// </summary>
			SystemBuilder& setProperties(const Properties& properties) {
				this->properties = properties;
				return *this;
			}

			/// <summary>
			/// Optional bounding box for the feature.
			/// </summary>
			SystemBuilder& setBbox(const std::vector<double>& bbox) {
				this->bbox = bbox;
				return *this;
			}

			/// <summary>
			/// Clear the bounding box.
			/// </summary>
			SystemBuilder& setBbox(std::nullopt_t) {
				this->bbox = std::nullopt;
				return *this;
			}

			/// <summary>
			/// Links to related resources.
			/// </summary>
			SystemBuilder& setLinks(const std::vector<Link>& links) {
				this->links = links;
				return *this;
			}

			/// <summary>
			/// Clear the links.
			/// </summary>
			SystemBuilder& setLinks(std::nullopt_t) {
				links = std::nullopt;
				return *this;
			}

			System build() const {
				if (!properties) {
					throw std::runtime_error("Properties are required");
				}

				return System("Feature", id, properties.value(), bbox, links);
			}
		};
	}
}