#pragma once

#include <optional>
#include <memory>
#include <ostream>
#include <utility>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Category.h"
#include "DataComponent.h"
#include "DataComponentRegistry.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class DataChoice;
	void to_json(nlohmann::ordered_json& j, const DataChoice& v);

	class DataChoice : public DataComponent {
	private:
		std::optional<Category> m_choiceValue;
		std::vector<std::unique_ptr<DataComponent>> m_items;

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

		void validate() const override {
			DataComponent::validate();
			if (m_choiceValue)
				m_choiceValue->validate();
			for (const auto& item : m_items) {
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
		const std::optional<Category>& getChoiceValue() const noexcept { return m_choiceValue; }
		void setChoiceValue(const std::optional<Category>& choiceValue) { m_choiceValue = choiceValue; }
		void setChoiceValue(std::optional<Category>&& choiceValue) { m_choiceValue = std::move(choiceValue); }
		void clearChoiceValue() noexcept { m_choiceValue.reset(); }

		/// <summary>
		/// Definition of the choice items.
		/// Items can be of any component types.
		/// </summary>				
		const std::vector<std::unique_ptr<DataComponent>>& getItems() const noexcept { return m_items; }
		void setItems(std::vector<std::unique_ptr<DataComponent>> items) { m_items = std::move(items); }
		void addItem(std::unique_ptr<DataComponent> item) { m_items.push_back(std::move(item)); }
		void clearItems() noexcept { m_items.clear(); }

		friend void from_json(const nlohmann::json& j, DataChoice& v);
		friend void to_json(nlohmann::ordered_json& j, const DataChoice& v);

		friend bool operator==(const DataChoice& a, const DataChoice& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const DataChoice& a, const DataChoice& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const DataChoice& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			return os << j.dump(2);
		}
	};

	const inline DataComponent::Registrar<DataChoice> registerDataChoice{ "DataChoice" };

	inline void from_json(const nlohmann::json& j, DataChoice& v) {
		from_json(j, static_cast<DataComponent&>(v));

		if (j.contains("choiceValue"))
			v.m_choiceValue = j.at("choiceValue").get<Category>();

		if (j.contains("items") && j["items"].is_array()) {
			std::vector<std::unique_ptr<DataComponent>> items;
			for (const auto& item : j["items"]) {
				auto created = DataComponentRegistry::createDataComponent(item);
				if (created) items.push_back(std::move(created));
			}
			v.m_items = std::move(items);
		}
	}

	inline void to_json(nlohmann::ordered_json& j, const DataChoice& v) {
		to_json(j, static_cast<const DataComponent&>(v));

		if (v.m_choiceValue)
			j["choiceValue"] = v.m_choiceValue.value();

		if (!v.m_items.empty()) {
			j["items"] = nlohmann::ordered_json::array();
			for (const auto& item : v.m_items) {
				j["items"].push_back(item ? item->toJson() : nlohmann::ordered_json(nullptr));
			}
		}
	}
}