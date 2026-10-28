#pragma once

#include <string>
#include <variant>
#include <vector>
#include <memory>
#include <cstdint>
#include <utility>
#include <nlohmann/json_fwd.hpp>

namespace ConnectedSystemsAPI::DataModels::Data {
	class DataBlockMixed;

	/// <summary>
	/// Value-semantic holder for a nested DataBlockMixed.
	/// DataBlockMixed contains DataValues, so DataValue can only refer to it indirectly.
	/// Copies are deep. Definitions that need the complete type are in DataBlockMixed.h.
	/// </summary>
	class DataBlockMixedBox {
	private:
		std::unique_ptr<DataBlockMixed> block;

	public:
		DataBlockMixedBox();
		explicit DataBlockMixedBox(const DataBlockMixed& v);
		explicit DataBlockMixedBox(DataBlockMixed&& v);
		DataBlockMixedBox(const DataBlockMixedBox& other);
		DataBlockMixedBox(DataBlockMixedBox&& other) noexcept;
		DataBlockMixedBox& operator=(const DataBlockMixedBox& other);
		DataBlockMixedBox& operator=(DataBlockMixedBox&& other) noexcept;
		~DataBlockMixedBox();

		const DataBlockMixed& get() const { return *block; }
		DataBlockMixed& get() { return *block; }
	};

	struct DataValue {
		using Array = std::vector<DataValue>;
		using ValueType = std::variant<
			bool,
			int64_t,
			uint64_t,
			double,
			std::string,
			Array,
			DataBlockMixedBox
		>;
		ValueType value;

		DataValue() = default;
		DataValue(const DataValue&) = default;
		DataValue(DataValue&&) noexcept = default;
		DataValue& operator=(const DataValue&) = default;
		DataValue& operator=(DataValue&&) noexcept = default;
		~DataValue() = default;

		explicit DataValue(bool v) : value(v) {}
		explicit DataValue(int64_t v) : value(v) {}
		explicit DataValue(uint64_t v) : value(v) {}
		explicit DataValue(double v) : value(v) {}
		explicit DataValue(const std::string& v) : value(v) {}
		explicit DataValue(std::string&& v) : value(std::move(v)) {}
		explicit DataValue(const char* s) : value(std::string(s)) {}
		explicit DataValue(const Array& v) : value(v) {}
		explicit DataValue(Array&& v) : value(std::move(v)) {}
		explicit DataValue(const DataBlockMixed& v) : value(DataBlockMixedBox(v)) {}
		explicit DataValue(DataBlockMixed&& v) : value(DataBlockMixedBox(std::move(v))) {}

		// Assignment operators for underlying types
		DataValue& operator=(bool v) { value = v; return *this; }
		DataValue& operator=(int64_t v) { value = v; return *this; }
		DataValue& operator=(uint64_t v) { value = v; return *this; }
		DataValue& operator=(double v) { value = v; return *this; }
		DataValue& operator=(const std::string& v) { value = v; return *this; }
		DataValue& operator=(std::string&& v) { value = std::move(v); return *this; }
		DataValue& operator=(const char* s) { value = std::string(s); return *this; }
		DataValue& operator=(const Array& v) { value = v; return *this; }
		DataValue& operator=(Array&& v) { value = std::move(v); return *this; }
		DataValue& operator=(const DataBlockMixed& v) { value = DataBlockMixedBox(v); return *this; }
		DataValue& operator=(DataBlockMixed&& v) { value = DataBlockMixedBox(std::move(v)); return *this; }

		bool isArray() const noexcept { return std::holds_alternative<Array>(value); }
		bool isDataBlock() const noexcept { return std::holds_alternative<DataBlockMixedBox>(value); }

		/// <summary>
		/// The nested array, or nullptr if this value is not an array.
		/// </summary>
		const Array* getArray() const noexcept { return std::get_if<Array>(&value); }
		Array* getArray() noexcept { return std::get_if<Array>(&value); }

		/// <summary>
		/// The nested data block (e.g., a DataRecord or Vector), or nullptr if this value is not a data block.
		/// </summary>
		const DataBlockMixed* getDataBlock() const noexcept {
			auto box = std::get_if<DataBlockMixedBox>(&value);
			return box ? &box->get() : nullptr;
		}
		DataBlockMixed* getDataBlock() noexcept {
			auto box = std::get_if<DataBlockMixedBox>(&value);
			return box ? &box->get() : nullptr;
		}

		/// <summary>
		/// String representation of the value. Arrays and data blocks are returned as JSON.
		/// </summary>
		std::string toString() const;

		friend void from_json(const nlohmann::json& j, DataValue& v);
		friend void to_json(nlohmann::ordered_json& j, const DataValue& v);
	};
}

// DataValue's remaining definitions need the complete DataBlockMixed type
#include "DataBlockMixed.h"
