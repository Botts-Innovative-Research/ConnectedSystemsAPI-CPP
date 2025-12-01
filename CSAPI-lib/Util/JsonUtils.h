#pragma once

#include <string>
#include <optional>
#include <vector>
#include <nlohmann/json.hpp>

namespace ConnectedSystemsAPI {
	namespace JsonUtils {
		static std::optional<bool> tryParseBoolean(const nlohmann::json& json, std::string field) {
			if (json.contains(field)) {
				const auto& value = json.at(field);
				if (value.is_boolean()) {
					return value.get<bool>();
				}
				if (value.is_number_integer()) {
					int n = value.get<int>();
					if (n == 0) return false;
					else if (n == 1) return true;
				}
				if (value.is_string()) {
					std::string s = value.get<std::string>();
					if (s == "true" || s == "1" || s == "yes") return true;
					else if (s == "false" || s == "0" || s == "no") return false;
				}
			}
			return std::nullopt;
		}

		static std::optional<int> tryParseInteger(const nlohmann::json& json, std::string field) {
			if (json.contains(field)) {
				const auto& value = json.at(field);
				if (value.is_number_integer()) {
					return value.get<int>();
				}
			}
			return std::nullopt;
		}

		static std::optional<std::vector<int>> tryParseIntegerArray(const nlohmann::json& json, std::string field) {
			if (json.contains(field)) {
				const auto& value = json.at(field);
				if (value.is_array()) {
					std::vector<int> result;
					for (const auto& item : value) {
						if (item.is_number_integer()) {
							result.push_back(item.get<int>());
						}
						else {
							// Non-integer item found, return nullopt
							return std::nullopt;
						}
					}
					return result;
				}
			}
			return std::nullopt;
		}

		static std::optional<double> tryParseDouble(const nlohmann::json& json, std::string field) {
			if (json.contains(field)) {
				const auto& value = json.at(field);
				if (value.is_number()) {
					return value.get<double>();
				}
			}
			return std::nullopt;
		}

		static std::optional<std::vector<double>> tryParseDoubleArray(const nlohmann::json& json, std::string field) {
			if (json.contains(field)) {
				const auto& value = json.at(field);
				if (value.is_array()) {
					std::vector<double> result;
					for (const auto& item : value) {
						if (item.is_number()) {
							result.push_back(item.get<double>());
						}
						else {
							// Non-number item found, return nullopt
							return std::nullopt;
						}
					}
					return result;
				}
			}
			return std::nullopt;
		}

		static std::optional<std::string> tryParseString(const nlohmann::json& json, std::string field) {
			if (json.contains(field)) {
				const auto& value = json.at(field);
				if (value.is_string()) {
					return value.get<std::string>();
				}
			}
			return std::nullopt;
		}

		static std::optional<std::vector<std::string>> tryParseStringArray(const nlohmann::json& json, std::string field) {
			if (json.contains(field)) {
				const auto& value = json.at(field);
				if (value.is_array()) {
					std::vector<std::string> result;
					for (const auto& item : value) {
						if (item.is_string()) {
							result.push_back(item.get<std::string>());
						}
						else {
							// Non-string item found, return nullopt
							return std::nullopt;
						}
					}
					return result;
				}
			}
			return std::nullopt;
		}
	}
}