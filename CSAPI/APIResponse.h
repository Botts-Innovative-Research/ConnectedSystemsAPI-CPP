#pragma once

#include <exception>
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

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
		bool hasContent = false;
		std::vector<std::string> parseErrors;
		std::map<std::string, std::vector<std::string>> headers;

		static bool isHttpSuccess(int code) {
			return code >= 200 && code < 400;
		}

	public:
		APIResponse(int responseCode, const std::string& responseMessage, const std::string& responseBody, const std::map<std::string, std::vector<std::string>>& headers)
			: responseCode(responseCode), responseMessage(responseMessage), responseBody(responseBody), headers(headers), isSingleItem(false) {
			// Error responses have no resource to read
			if (!isHttpSuccess(responseCode)) return;

			if (responseBody.empty() || responseBody[0] != '{') {
				parseErrors.push_back("Response body is empty or not a JSON object");
				return;
			}

			nlohmann::ordered_json jsonObj;
			try {
				jsonObj = nlohmann::ordered_json::parse(responseBody);
			}
			catch (const std::exception& e) {
				parseErrors.push_back(std::string("Invalid JSON: ") + e.what());
				return;
			}

			if (jsonObj.contains(JSON_ARRAY_ITEMS) && jsonObj[JSON_ARRAY_ITEMS].is_array()) {
				// Items that fail to deserialize are skipped and reported, so the rest of the list is still usable
				hasContent = true;
				const auto& itemsJson = jsonObj[JSON_ARRAY_ITEMS];
				for (size_t i = 0; i < itemsJson.size(); ++i) {
					try {
						items.push_back(itemsJson[i].template get<T>());
					}
					catch (const std::exception& e) {
						parseErrors.push_back("items[" + std::to_string(i) + "]: " + e.what());
					}
				}
			}
			else {
				isSingleItem = true;
				try {
					items.push_back(jsonObj.template get<T>());
					hasContent = true;
				}
				catch (const std::exception& e) {
					parseErrors.push_back(e.what());
				}
			}
		}

		/// <summary>
		/// True if the request succeeded and its body was read: a list of items (some of which may have failed to
		/// deserialize, see getParseErrors()), or a single item that deserialized.
		/// False for HTTP errors, and for successful statuses whose body is empty, not JSON, or a single item that
		/// failed to deserialize (e.g., an OpenSensorHub node redirects an unknown ID to its web UI with an empty body).
		/// </summary>
		bool isSuccessful() const {
			return isHttpSuccess(responseCode) && hasContent;
		}

		/// <summary>
		/// Errors encountered while reading the response body, e.g., invalid JSON or items that failed to deserialize.
		/// </summary>
		const std::vector<std::string>& getParseErrors() const { return parseErrors; }
		bool hasParseErrors() const { return !parseErrors.empty(); }

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

	template <>
	class APIResponse<void> {
	private:
		int responseCode;
		std::string responseMessage;
		std::string responseBody;
		std::map<std::string, std::vector<std::string>> headers;

	public:
		APIResponse(int responseCode, const std::string& responseMessage, const std::string& responseBody, const std::map<std::string, std::vector<std::string>>& headers)
			: responseCode(responseCode), responseMessage(responseMessage), responseBody(responseBody), headers(headers) {
		}

		bool isSuccessful() const {
			return responseCode >= 200 && responseCode < 400;
		}

		int getResponseCode() const { return responseCode; }
		const std::string& getResponseMessage() const { return responseMessage; }
		const std::string& getResponseBody() const { return responseBody; }
		const std::map<std::string, std::vector<std::string>>& getHeaders() const { return headers; }
	};
}