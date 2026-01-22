#pragma once

#include <string>
#include <unordered_map>
#include <ostream>
#include <utility>
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
}