#pragma once

#include <string>
#include <unordered_map>
#include <ostream>
#include <utility>
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>

#include "DataBlock.h"
#include "DataType.h"
#include "DataValue.h"

namespace ConnectedSystemsAPI::DataModels::Data {
	class DataBlockMixed : public DataBlock {
	private:
		std::vector<std::pair<std::string, DataValue>> values;
		std::unordered_map<std::string, std::size_t> valuesIndex;

	public:
		DataBlockMixed() = default;
		DataBlockMixed(const DataBlockMixed&) = default;
		DataBlockMixed(DataBlockMixed&&) noexcept = default;
		DataBlockMixed& operator=(const DataBlockMixed&) = default;
		DataBlockMixed& operator=(DataBlockMixed&&) noexcept = default;
		~DataBlockMixed() override = default;

		DataType getDataType() const noexcept override {
			return DataType::Mixed;
		}

		bool hasField(const std::string& name) const noexcept {
			return valuesIndex.find(name) != valuesIndex.end();
		}

		const DataValue* getField(const std::string& name) const noexcept {
			auto it = valuesIndex.find(name);
			if (it == valuesIndex.end()) return nullptr;
			return &values[it->second].second;
		}

		void setField(const std::string& name, const DataValue& value) {
			if (hasField(name)) {
				values[valuesIndex[name]].second = value;
			}
			else {
				values.emplace_back(name, value);
				valuesIndex[name] = values.size() - 1;
			}
		}

		friend void from_json(const nlohmann::ordered_json& j, DataBlockMixed& v);
		friend void to_json(nlohmann::ordered_json& j, const DataBlockMixed& v);

		friend std::ostream& operator<<(std::ostream& os, const DataBlockMixed& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			os << j.dump(2);
			return os;
		}
	};

	inline void from_json(const nlohmann::ordered_json& j, DataBlockMixed& v) {
		v.values.clear();
		v.valuesIndex.clear();

		if (!j.is_object()) return;

		for (auto it = j.begin(); it != j.end(); ++it) {
			const std::string key = it.key();
			DataValue dv = it.value().get<DataValue>();
			v.values.emplace_back(key, std::move(dv));
			v.valuesIndex[key] = v.values.size() - 1;
		}
	}

	inline void to_json(nlohmann::ordered_json& j, const DataBlockMixed& v) {
		j = nlohmann::ordered_json::object();
		for (const auto& pair : v.values) {
			j[pair.first] = pair.second;
		}
	}

	// DataBlockMixedBox and DataValue definitions that need the complete DataBlockMixed type

	inline DataBlockMixedBox::DataBlockMixedBox() : block(std::make_unique<DataBlockMixed>()) {}
	inline DataBlockMixedBox::DataBlockMixedBox(const DataBlockMixed& v) : block(std::make_unique<DataBlockMixed>(v)) {}
	inline DataBlockMixedBox::DataBlockMixedBox(DataBlockMixed&& v) : block(std::make_unique<DataBlockMixed>(std::move(v))) {}
	inline DataBlockMixedBox::DataBlockMixedBox(const DataBlockMixedBox& other) : block(std::make_unique<DataBlockMixed>(*other.block)) {}
	inline DataBlockMixedBox::DataBlockMixedBox(DataBlockMixedBox&& other) noexcept : block(std::move(other.block)) {}
	inline DataBlockMixedBox& DataBlockMixedBox::operator=(const DataBlockMixedBox& other) {
		if (this != &other) block = std::make_unique<DataBlockMixed>(*other.block);
		return *this;
	}
	inline DataBlockMixedBox& DataBlockMixedBox::operator=(DataBlockMixedBox&& other) noexcept {
		block = std::move(other.block);
		return *this;
	}
	inline DataBlockMixedBox::~DataBlockMixedBox() = default;

	inline std::string DataValue::toString() const {
		if (std::holds_alternative<bool>(value)) {
			return std::get<bool>(value) ? "true" : "false";
		}
		else if (std::holds_alternative<int64_t>(value)) {
			return std::to_string(std::get<int64_t>(value));
		}
		else if (std::holds_alternative<uint64_t>(value)) {
			return std::to_string(std::get<uint64_t>(value));
		}
		else if (std::holds_alternative<double>(value)) {
			return std::to_string(std::get<double>(value));
		}
		else if (std::holds_alternative<std::string>(value)) {
			return std::get<std::string>(value);
		}
		else if (isArray() || isDataBlock()) {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j.dump();
		}
		else {
			return "";
		}
	}

	inline void from_json(const nlohmann::ordered_json& j, DataValue& v) {
		if (j.is_boolean()) {
			v.value = j.get<bool>();
		}
		else if (j.is_number_integer()) {
			v.value = j.get<int64_t>();
		}
		else if (j.is_number_unsigned()) {
			v.value = j.get<uint64_t>();
		}
		else if (j.is_number_float()) {
			v.value = j.get<double>();
		}
		else if (j.is_string()) {
			v.value = j.get<std::string>();
		}
		else if (j.is_array()) {
			DataValue::Array array;
			array.reserve(j.size());
			for (const auto& element : j) {
				array.push_back(element.get<DataValue>());
			}
			v.value = std::move(array);
		}
		else if (j.is_object()) {
			v.value = DataBlockMixedBox(j.get<DataBlockMixed>());
		}
	}

	inline void to_json(nlohmann::ordered_json& j, const DataValue& v) {
		if (std::holds_alternative<bool>(v.value)) {
			j = std::get<bool>(v.value);
		}
		else if (std::holds_alternative<int64_t>(v.value)) {
			j = std::get<int64_t>(v.value);
		}
		else if (std::holds_alternative<uint64_t>(v.value)) {
			j = std::get<uint64_t>(v.value);
		}
		else if (std::holds_alternative<double>(v.value)) {
			j = std::get<double>(v.value);
		}
		else if (std::holds_alternative<std::string>(v.value)) {
			j = std::get<std::string>(v.value);
		}
		else if (auto array = v.getArray()) {
			j = nlohmann::ordered_json::array();
			for (const auto& element : *array) {
				nlohmann::ordered_json elementJson;
				to_json(elementJson, element);
				j.push_back(std::move(elementJson));
			}
		}
		else if (auto block = v.getDataBlock()) {
			to_json(j, *block);
		}
	}
}