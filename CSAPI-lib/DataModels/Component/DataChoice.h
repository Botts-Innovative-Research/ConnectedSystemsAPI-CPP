#pragma once

#include <optional>
#include <nlohmann/json.hpp>
#include "Category.h"
#include "DataComponent.h"
#include "DataComponentRegistry.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class DataChoice;
	void to_json(nlohmann::ordered_json& j, const DataChoice& v);

	class DataChoice : public DataComponent {
	private:
		std::optional<Category> choiceValue;
		std::vector<std::unique_ptr<DataComponent>> items;

	public:
		DataChoice() = default;
		DataChoice(const DataChoice& other) {
			nlohmann::ordered_json j = other.toJson();
			from_json(j, *this);
		}
		DataChoice& operator=(const DataChoice& other) {
			if (this != &other) {
				nlohmann::ordered_json j = other.toJson();
				from_json(j, *this);
			}
			return *this;
		}
		DataChoice(DataChoice&&) noexcept = default;
		DataChoice& operator=(DataChoice&&) noexcept = default;
		~DataChoice() override = default;

		void validate() const {
			DataComponent::validate();
			if (choiceValue)
				choiceValue->validate();
			for (const auto& item : items) {
				if (item) item->validate();
			}
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
		const std::optional<Category>& getChoiceValue() const noexcept { return choiceValue; }
		void setChoiceValue(const std::optional<Category>& v) { choiceValue = v; }
		void setChoiceValue(std::optional<Category>&& v) { choiceValue = std::move(v); }
		void clearChoiceValue() noexcept { choiceValue.reset(); }

		/// <summary>
		/// Definition of the choice items.
		/// Items can be of any component types.
		/// </summary>				
		const std::vector<std::unique_ptr<DataComponent>>& getItems() const noexcept { return items; }
		void setItems(std::vector<std::unique_ptr<DataComponent>> f) { items = std::move(f); }
		void addItem(std::unique_ptr<DataComponent> item) { items.push_back(std::move(item)); }
		void clearItems() noexcept { items.clear(); }
	};

	inline DataComponent::Registrar<DataChoice> registerDataChoice{ "DataChoice" };
	inline bool operator==(const DataChoice& a, const DataChoice& b) { return a.toJson() == b.toJson(); }
	inline bool operator!=(const DataChoice& a, const DataChoice& b) { return !(a == b); }

	inline void from_json(const nlohmann::json& j, DataChoice& v) {
		from_json(j, static_cast<DataComponent&>(v));

		if (j.contains("choiceValue"))
			v.setChoiceValue(j.at("choiceValue").get<Category>());

		if (j.contains("items") && j["items"].is_array()) {
			std::vector<std::unique_ptr<DataComponent>> items;
			for (const auto& item : j["items"]) {
				auto created = DataComponentRegistry::createDataComponent(item);
				if (created) items.push_back(std::move(created));
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
				j["items"].push_back(item ? item->toJson() : nlohmann::ordered_json(nullptr));
			}
		}
	}
}