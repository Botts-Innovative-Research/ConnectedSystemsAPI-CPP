#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "Category.h"
#include "DataComponent.h"
#include "DataComponentRegistry.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			class DataChoice;
			void to_json(nlohmann::ordered_json& j, const DataChoice& v);

			class DataChoice : public DataComponent {
			private:
				std::optional<Category> choiceValue;
				std::vector<std::unique_ptr<DataComponent>> items;

			public:
				DataChoice() = default;

				void validate() const {
					DataComponent::validate();
				}

				nlohmann::ordered_json toJson() const override {
					nlohmann::ordered_json j;
					to_json(j, *this);
					return j;
				}

				/// <summary>
				/// This category component marks the data stream element that will indicate the actual choice made.
				/// Possible choices are listed in the Category constraint section as an enumeration and should map to item names.
				/// </summary>
				const std::optional<Category> getChoiceValue() const { return choiceValue; }
				/// <summary>
				/// This category component marks the data stream element that will indicate the actual choice made.
				/// Possible choices are listed in the Category constraint section as an enumeration and should map to item names.
				/// </summary>				
				void setChoiceValue(const std::optional<Category>& choiceValue) { this->choiceValue = choiceValue; }
				/// <summary>
				/// Definition of the choice items.
				/// Items can be of any component types.
				/// </summary>				
				const std::vector<std::unique_ptr<DataComponent>>& getItems() const { return items; }
				/// <summary>
				/// Definition of the choice items.
				/// Items can be of any component types.
				/// </summary>				
				void setItems(std::vector<std::unique_ptr<DataComponent>> f) { items = std::move(f); }
			};

			// Register with the DataComponentRegistry
			struct DataChoiceRegistrar {
				DataChoiceRegistrar() {
					DataComponentRegistry::registerType(
						"DataChoice", [](const nlohmann::json& j) {
						return std::make_unique<DataChoice>(j.get<DataChoice>());
					}
					);
				}
			};
			static DataChoiceRegistrar dataChoiceRegistrar;

			inline void from_json(const nlohmann::json& j, DataChoice& v) {
				from_json(j, static_cast<DataComponent&>(v));

				if (j.contains("choiceValue"))
					v.setChoiceValue(j.at("choiceValue").get<Category>());

				if (j.contains("items") && j["items"].is_array()) {
					std::vector<std::unique_ptr<DataComponent>> items;
					for (const auto& item : j["items"]) {
						items.push_back(DataComponentRegistry::createDataComponent(item));
					}
					v.setItems(std::move(items));
				}
			}

			inline void to_json(nlohmann::ordered_json& j, const DataChoice& v) {
				to_json(j, static_cast<const DataComponent&>(v));

				if (v.getChoiceValue())
					j["choiceValue"] = v.getChoiceValue().value();

				if (!v.getItems().empty()) {
					j["items"] = nlohmann::ordered_json::array();
					for (const auto& item : v.getItems()) {
						j["items"].push_back(item->toJson());
					}
				}
			}
		}
	}
}