#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "DataComponent.h"
#include "DataComponentRegistry.h"
#include "NilValue.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class SimpleComponent : public DataComponent {
			private:
				std::optional<std::string> referenceFrame;
				std::optional<std::string> axisId;
				std::optional<std::vector<NilValue>> nilValues;
				// Todo: constraint

			public:
				SimpleComponent() = default;

				void validate() const {
					DataComponent::validate();

					if (nilValues) {
						for (const auto& nv : nilValues.value()) {
							nv.validate();
						}
					}
				}

				/// <summary>Frame of reference (usually temporal or spatial) with respect to which the value of the component is expressed.
				/// A reference frame anchors a value to a real world datum.</summary>
				const std::optional<std::string> getReferenceFrame() const { return referenceFrame; }
				/// <summary>Frame of reference (usually temporal or spatial) with respect to which the value of the component is expressed.
				/// A reference frame anchors a value to a real world datum.</summary>
				void setReferenceFrame(const std::optional<std::string> referenceFrame) { this->referenceFrame = referenceFrame; }
				/// <summary>Specifies the reference axis (refer to CRS axisID).
				/// The reference frame URI should also be specified unless it is inherited from parent Vector.</summary>
				const std::optional<std::string> getAxisId() const { return axisId; }
				/// <summary>Specifies the reference axis (refer to CRS axisID).
				/// The reference frame URI should also be specified unless it is inherited from parent Vector.</summary>
				void setAxisId(const std::optional<std::string> axisId) { this->axisId = axisId; }
				/// <summary>Defines reserved values with special meaning (e.g., missing, out-of-range, etc.)</summary>
				const std::optional<std::vector<NilValue>>& getNilValues() const { return nilValues; }
				/// <summary>Defines reserved values with special meaning (e.g., missing, out-of-range, etc.)</summary>
				void setNilValues(const std::optional<std::vector<NilValue>>& nilValues) { this->nilValues = nilValues; }
			};

			inline void from_json(const nlohmann::json& j, SimpleComponent& s) {
				from_json(j, static_cast<DataComponent&>(s));

				if (j.contains("refFrame") && j["refFrame"].is_string())
					s.setReferenceFrame(j["refFrame"].get<std::string>());
				else
					s.setReferenceFrame(std::nullopt);

				if (j.contains("axisID") && j["axisID"].is_string())
					s.setAxisId(j["axisID"].get<std::string>());
				else
					s.setAxisId(std::nullopt);

				if (j.contains("nilValues") && j["nilValues"].is_array())
					s.setNilValues(j.at("nilValues").get<std::vector<NilValue>>());
				else
					s.setNilValues(std::nullopt);
			}

			inline void to_json(nlohmann::ordered_json& j, const SimpleComponent& s) {
				to_json(j, static_cast<const DataComponent&>(s));

				if (s.getReferenceFrame())
					j["refFrame"] = s.getReferenceFrame();
				if (s.getAxisId())
					j["axisID"] = s.getAxisId();
				if (s.getNilValues())
					j["nilValues"] = s.getNilValues().value();
			}

			inline std::ostream& operator<<(std::ostream& os, const SimpleComponent& s) {
				nlohmann::ordered_json j;
				to_json(j, s);
				return os << j.dump(2);
			}
		}
	}
}