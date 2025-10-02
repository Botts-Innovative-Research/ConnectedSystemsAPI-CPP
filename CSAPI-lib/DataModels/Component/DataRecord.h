#pragma once

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include "DataComponent.h"
#include "DataComponentRegistry.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class DataRecord;
			void to_json(nlohmann::ordered_json& j, const DataRecord& r);

			class DataRecord : public DataComponent {
			private:
				std::vector<std::unique_ptr<DataComponent>> fields;

			public:
				DataRecord() = default;

				void validate() const {
					DataComponent::validate();
				}

				nlohmann::ordered_json toJson() const override {
					nlohmann::ordered_json j;
					to_json(j, *this);
					return j;
				}

				/// <summary>
				/// Definition of the record fields.
				/// Fields can be scalars or can themself be aggregates such as records, vectors, arrays, or choices.
				/// </summary>
				const std::vector<std::unique_ptr<DataComponent>>& getFields() const { return fields; }
				/// <summary>
				/// Definition of the record fields.
				/// Fields can be scalars or can themself be aggregates such as records, vectors, arrays, or choices.
				/// </summary>
				void setFields(std::vector<std::unique_ptr<DataComponent>> f) { fields = std::move(f); }
			};

			// Register with the DataComponentRegistry
			struct DataRecordRegistrar {
				DataRecordRegistrar() {
					DataComponentRegistry::registerType(
						"DataRecord", [](const nlohmann::json& j) {
						return std::make_unique<DataRecord>(j.get<DataRecord>());
					}
					);
				}
			};
			static DataRecordRegistrar dataRecordRegistrar;

			inline void from_json(const nlohmann::json& j, DataRecord& r) {
				from_json(j, static_cast<DataComponent&>(r)); // Populate base fields

				// Now handle DataRecord-specific fields
				if (j.contains("fields") && j["fields"].is_array()) {
					std::vector<std::unique_ptr<DataComponent>> tempFields;
					for (const auto& fieldJson : j["fields"]) {
						tempFields.push_back(DataComponentRegistry::createDataComponent(fieldJson));
					}
					r.setFields(std::move(tempFields));
				}
			}

			inline void to_json(nlohmann::ordered_json& j, const DataRecord& r) {
				to_json(j, static_cast<const DataComponent&>(r));

				j["fields"] = nlohmann::ordered_json::array();
				for (const auto& fieldPtr : r.getFields()) {
					if (fieldPtr) {
						j["fields"].push_back(fieldPtr->toJson());
					}
				}
			}

			inline std::ostream& operator<<(std::ostream& os, const DataRecord& r) {
				nlohmann::ordered_json j;
				to_json(j, r);
				return os << j.dump(2);
			}
		}
	}
}