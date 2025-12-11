#pragma once

#include <string>
#include <variant>

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Data {
			struct DataValue {
				using ValueType = std::variant<
					bool,
					int64_t,
					uint64_t,
					double,
					std::string
				>;
				ValueType value;

				DataValue() = default;
				DataValue(const DataValue&) = default;
				DataValue(DataValue&&) noexcept = default;
				DataValue& operator=(const DataValue&) = default;
				DataValue& operator=(DataValue&&) noexcept = default;
				~DataValue() = default;

				DataValue(bool v) : value(v) {}
				DataValue(int64_t v) : value(v) {}
				DataValue(uint64_t v) : value(v) {}
				DataValue(double v) : value(v) {}
				DataValue(const std::string& v) : value(v) {}
				DataValue(std::string&& v) : value(std::move(v)) {}
				DataValue(const char* s) : value(std::string(s)) {}

				// Assignment operators for underlying types
				DataValue& operator=(bool v) { value = v; return *this; }
				DataValue& operator=(int64_t v) { value = v; return *this; }
				DataValue& operator=(uint64_t v) { value = v; return *this; }
				DataValue& operator=(double v) { value = v; return *this; }
				DataValue& operator=(const std::string& v) { value = v; return *this; }
				DataValue& operator=(std::string&& v) { value = std::move(v); return *this; }
				DataValue& operator=(const char* s) { value = std::string(s); return *this; }

				std::string toString() const {
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
					else {
						return "";
					}
				}

				friend void from_json(const nlohmann::json& j, DataValue& v);
				friend void to_json(nlohmann::ordered_json& j, const DataValue& v);
			};

			inline void from_json(const nlohmann::json& j, DataValue& v) {
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
			}
		}
	}
}