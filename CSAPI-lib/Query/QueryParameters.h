#pragma once

#include <string>
#include <map>
#include <sstream>
#include "..\DataModels\TimeExtent.h"

namespace ConnectedSystemsAPI {
	namespace Query {
		class QueryParameters {
		private:
			std::map<std::string, std::string> params;

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
				nlohmann::ordered_json j;
				to_json(j, value);
				std::ostringstream oss;
				oss << j;
				params[key] = oss.str();
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
					oss << key << "=" << value;
					first = false;
				}
				return oss.str();
			}
		};
	}
}