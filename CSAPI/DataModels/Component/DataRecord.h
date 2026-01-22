#pragma once

#include <string>
#include <ostream>
#include <exception>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "DataComponent.h"
#include "DataComponentRegistry.h"
#include "DataModels/Data/DataBlockMixed.h"
#include <DataModels/Data/DataValue.h>

namespace ConnectedSystemsAPI::DataModels::Component {
	class DataRecord;
	void to_json(nlohmann::ordered_json& j, const DataRecord& r);

	class DataRecord : public DataComponent {
	private:
		std::vector<std::unique_ptr<DataComponent>> fields;

	public:
		DataRecord() = default;
		DataRecord(const DataRecord& other) {
			nlohmann::ordered_json j = other.toJson();
			from_json(j, *this);
		}
		DataRecord& operator=(const DataRecord& other) {
			if (this != &other) {
				nlohmann::ordered_json j = other.toJson();
				from_json(j, *this);
			}
			return *this;
		}
		DataRecord(DataRecord&&) noexcept = default;
		DataRecord& operator=(DataRecord&&) noexcept = default;
		~DataRecord() override = default;

		void validate() const override {
			DataComponent::validate();
			for (const auto& f : fields) {
				if (f) {
					f->validate();
				}
			}
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
		const std::vector<std::unique_ptr<DataComponent>>& getFields() const noexcept { return fields; }
		void setFields(std::vector<std::unique_ptr<DataComponent>> f) noexcept { fields = std::move(f); }
		void clearFields() noexcept { fields.clear(); }
		void addField(std::unique_ptr<DataComponent> field) { if (field) fields.push_back(std::move(field)); }

		DataModels::Data::DataBlockMixed createDataBlock() const {
			DataModels::Data::DataBlockMixed dataBlock;
			for (const auto& field : fields) {
				auto fieldName = field->getName().value_or("");
				auto fieldValue = DataModels::Data::DataValue();
				if (field->getType() == "Boolean") {
					fieldValue = DataModels::Data::DataValue(false);
				}
				else if (field->getType() == "Count") {
					fieldValue = DataModels::Data::DataValue(int64_t(0));
				}
				else if (field->getType() == "Quantity") {
					fieldValue = DataModels::Data::DataValue(0.0);
				}
				else if (field->getType() == "Text") {
					fieldValue = DataModels::Data::DataValue(std::string(""));
				}
				if (fieldName.empty()) continue;
				dataBlock.setField(fieldName, fieldValue);
			}
			return dataBlock;
		}

		friend void from_json(const nlohmann::json& j, DataRecord& v);
		friend void to_json(nlohmann::ordered_json& j, const DataRecord& v);

		friend bool operator==(const DataRecord& a, const DataRecord& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const DataRecord& a, const DataRecord& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const DataRecord& r) {
			nlohmann::ordered_json j;
			to_json(j, r);
			return os << j.dump(2);
		}
	};

	const inline DataComponent::Registrar<DataRecord> registerDataRecord{ "DataRecord" };

	inline void from_json(const nlohmann::json& j, DataRecord& r) {
		from_json(j, static_cast<DataComponent&>(r));

		if (j.contains("fields") && j["fields"].is_array()) {
			std::vector<std::unique_ptr<DataComponent>> tempFields;
			tempFields.reserve(j["fields"].size());
			for (const auto& fieldJson : j["fields"]) {
				try {
					auto comp = DataComponentRegistry::createDataComponent(fieldJson);
					if (comp)
						tempFields.push_back(std::move(comp));
				}
				catch (const std::exception&) {
					// Skip invalid field.
				}
			}
			r.fields = std::move(tempFields);
		}
		else {
			r.clearFields();
		}
	}

	inline void to_json(nlohmann::ordered_json& j, const DataRecord& r) {
		to_json(j, static_cast<const DataComponent&>(r));

		j["fields"] = nlohmann::ordered_json::array();
		for (const auto& fieldPtr : r.fields) {
			if (fieldPtr) {
				j["fields"].push_back(fieldPtr->toJson());
			}
		}
	}
}