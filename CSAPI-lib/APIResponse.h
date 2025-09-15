#pragma once

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

namespace ConnectedSystemsAPI {
	template <typename T>
	class APIResponse {
	private:
		static constexpr const char* JSON_ARRAY_ITEMS = "items";

		int responseCode;
		std::string responseMessage;
		std::string responseBody;
		std::vector<T> items;
		bool isSingleItem;
		std::map<std::string, std::vector<std::string>> headers;

		T deserializeItem(const nlohmann::json& jsonElement) {
			return jsonElement.get<T>();
		}

	public:
		APIResponse(int responseCode, const std::string& responseMessage, const std::string& responseBody, const std::map<std::string, std::vector<std::string>>& headers)
			: responseCode(responseCode), responseMessage(responseMessage), responseBody(responseBody), headers(headers), isSingleItem(false) {
			if (responseBody.empty() || responseBody[0] != '{') {
				isSingleItem = false;
				return;
			}

			auto jsonObj = nlohmann::json::parse(responseBody);
			if (jsonObj.contains(JSON_ARRAY_ITEMS)) {
				isSingleItem = false;
				for (const auto& itemElement : jsonObj[JSON_ARRAY_ITEMS]) {
					items.push_back(deserializeItem(itemElement));
				}
			}
			else {
				isSingleItem = true;
				items.push_back(deserializeItem(jsonObj));
			}
		}

		bool isSuccessful() const {
			return responseCode >= 200 && responseCode < 400;
		}

		int getResponseCode() const { return responseCode; }
		const std::string& getResponseMessage() const { return responseMessage; }
		const std::string& getResponseBody() const { return responseBody; }
		bool getIsSingleItem() const { return isSingleItem; }
		const std::map<std::string, std::vector<std::string>>& getHeaders() const { return headers; }

		T getItem() const {
			return (isSingleItem && !items.empty()) ? items[0] : T{};
		}

		const std::vector<T>& getItems() const {
			return items;
		}
	};
}