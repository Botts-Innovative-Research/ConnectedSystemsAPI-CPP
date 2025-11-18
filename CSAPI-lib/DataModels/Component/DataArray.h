#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "Count.h"
#include "DataComponent.h"
#include "DataComponentRegistry.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class DataArray;
			void to_json(nlohmann::ordered_json& j, const DataArray& v);

			class DataArray : public DataComponent {
			private:
				Count elementCount;
				std::unique_ptr<DataComponent> elementType;

			public:
				DataArray() = default;

				void validate() const {
					DataComponent::validate();
				}

				nlohmann::ordered_json toJson() const override {
					nlohmann::ordered_json j;
					to_json(j, *this);
					return j;
				}

				/// <summary>Specifies the size of the array (i.e., the number of elements of the defined type it contains).</summary>
				const Count& getElementCount() const { return elementCount; }
				/// <summary>Specifies the size of the array (i.e., the number of elements of the defined type it contains).</summary>
				void setElementCount(const Count& elementCount) { this->elementCount = elementCount; }
				/// <summary>Defines the structure of the element that will be repeated in the array.</summary>
				const DataComponent* getElementType() const { return elementType.get(); }
				/// <summary>Defines the structure of the element that will be repeated in the array.</summary>
				void setElementType(std::unique_ptr<DataComponent> elementType) { this->elementType = std::move(elementType); }
			};

			// Register with the DataComponentRegistry
			struct DataArrayRegistrar {
				DataArrayRegistrar() {
					DataComponentRegistry::registerType(
						"DataArray", [](const nlohmann::json& j) {
						return std::make_unique<DataArray>(j.get<DataArray>());
					}
					);
				}
			};
			static DataArrayRegistrar DdataArraytorRegistrar;

			inline void from_json(const nlohmann::json& j, DataArray& v) {
				from_json(j, static_cast<DataComponent&>(v));

				if (j.contains("elementCount"))
					v.setElementCount(j.at("elementCount").get<Count>());

				if (j.contains("elementType") && j["elementType"].is_object()) {
					v.setElementType(DataComponentRegistry::createDataComponent(j.at("elementType")));
				}
				else {
					v.setElementType(nullptr);
				}
			}

			inline void to_json(nlohmann::ordered_json& j, const DataArray& v) {
				to_json(j, static_cast<const DataComponent&>(v));

				j["elementCount"] = v.getElementCount();
				j["elementType"] = v.getElementType()->toJson();
			}
		}
	}
}