#pragma once

#include <string>
#include <string_view>
#include <map>
#include <sstream>
#include <vector>
#include "DataModels/TimeExtent.h"

namespace ConnectedSystemsAPI::Query {
	class QueryParameters {
	private:
		std::map<std::string, std::string> params;

		/// <summary>
		/// Percent-encodes a query string key or value.
		/// Unreserved characters are kept, as are ':' and '/' (used in times) and ',' (the list separator).
		/// </summary>
		static std::string encode(std::string_view text) {
			static constexpr char HEX[] = "0123456789ABCDEF";
			std::string encoded;
			encoded.reserve(text.size());
			for (unsigned char c : text) {
				if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
					|| c == '-' || c == '.' || c == '_' || c == '~' || c == ':' || c == '/' || c == ',') {
					encoded += static_cast<char>(c);
				}
				else {
					encoded += '%';
					encoded += HEX[c >> 4];
					encoded += HEX[c & 0x0F];
				}
			}
			return encoded;
		}

	public:
		template<typename T>
		void addParameter(const std::string& key, const T& value) {
			std::ostringstream oss;
			oss << value;
			params[key] = oss.str();
		}

		void addParameter(const std::string& key, const bool value) {
			params[key] = value ? "true" : "false";
		}

		void addParameter(const std::string& key, const DataModels::TimeExtent& value) {
			params[key] = value.toStringUTC();
		}

		void addParameter(const std::string& key, const std::vector<std::string>& values) {
			std::ostringstream oss;
			for (size_t i = 0; i < values.size(); ++i) {
				if (i > 0) oss << ",";
				oss << values[i];
			}
			params[key] = oss.str();
		}

		std::string toString() const {
			if (params.empty()) return "";

			std::ostringstream oss;
			oss << "?";
			bool first = true;
			for (const auto& [key, value] : params) {
				if (!first) oss << "&";
				oss << encode(key) << "=" << encode(value);
				first = false;
			}
			return oss.str();
		}
	};
}
