#pragma once

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "DataComponent.h"
#include "DataComponentRegistry.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class Vector;
			void to_json(nlohmann::ordered_json& j, const Vector& r);

			class Vector : public DataComponent {
			private:
				std::optional<std::string> referenceFrame;
				std::optional<std::string> localFrame;
				std::vector<std::unique_ptr<DataComponent>> coordinates;

			public:
				Vector() = default;

				void validate() const {
					DataComponent::validate();
				}

				nlohmann::ordered_json toJson() const override {
					nlohmann::ordered_json j;
					to_json(j, *this);
					return j;
				}

				/// <summary>
				/// Frame of reference (usually spatial) with respect to which the coordinates of this vector are expressed.
				/// A reference frame anchors a vector value to a real world datum.
				/// </summary>
				std::optional<std::string> getReferenceFrame() const { return referenceFrame; }
				/// <summary>
				/// Frame of reference (usually spatial) with respect to which the coordinates of this vector are expressed.
				/// A reference frame anchors a vector value to a real world datum.
				/// </summary>
				void setReferenceFrame(const std::optional<std::string>& referenceTime) { this->referenceFrame = referenceFrame; }
				/// <summary>Frame of reference whose origin is located by the coordinates of this vector.</summary>
				std::optional<std::string> getLocalFrame() const { return localFrame; }
				/// <summary>Frame of reference whose origin is located by the coordinates of this vector.</summary>
				void setLocalFrame(const std::optional<std::string>& localFrame) { this->localFrame = localFrame; }
				/// <summary>
				/// Definition of the coordinate provided as a data component with a numerical representation.
				/// </summary>
				const std::vector<std::unique_ptr<DataComponent>>& getCoordinates() const { return coordinates; }
				/// <summary>
				/// Definition of the coordinate provided as a data component with a numerical representation.
				/// </summary>
				void setCoordinates(std::vector<std::unique_ptr<DataComponent>> f) { coordinates = std::move(f); }
			};

			// Register with the DataComponentRegistry
			struct VectorRegistrar {
				VectorRegistrar() {
					DataComponentRegistry::registerType(
						"Vector", [](const nlohmann::json& j) {
						return std::make_unique<Vector>(j.get<Vector>());
					}
					);
				}
			};
			static VectorRegistrar vectorRegistrar;

			inline void from_json(const nlohmann::json& j, Vector& r) {
				from_json(j, static_cast<DataComponent&>(r));

				if (j.contains("coordinates") && j["coordinates"].is_array()) {
					std::vector<std::unique_ptr<DataComponent>> tempFields;
					for (const auto& fieldJson : j["coordinates"]) {
						tempFields.push_back(DataComponentRegistry::createDataComponent(fieldJson));
					}
					r.setCoordinates(std::move(tempFields));
				}
			}

			inline void to_json(nlohmann::ordered_json& j, const Vector& r) {
				to_json(j, static_cast<const DataComponent&>(r));

				j["coordinates"] = nlohmann::ordered_json::array();
				for (const auto& coordPtr : r.getCoordinates()) {
					if (coordPtr) {
						j["coordinates"].push_back(coordPtr->toJson());
					}
				}
			}

			inline std::ostream& operator<<(std::ostream& os, const Vector& r) {
				nlohmann::ordered_json j;
				to_json(j, r);
				return os << j.dump(2);
			}
		}
	}
}