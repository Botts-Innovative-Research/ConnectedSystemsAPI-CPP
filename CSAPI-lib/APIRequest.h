#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <curl/curl.h>
#include <map>
#include "APIResponse.h"

namespace ConnectedSystemsAPI {
	struct RawHttpResponse {
		int responseCode = 0;
		std::string responseMessage;
		std::string responseBody;
		std::map<std::string, std::vector<std::string>> headers;
	};

	class APIRequest {
	private:
		std::string apiRoot;
		std::string endpoint;
		std::string requestMethod;
		std::map<std::string, std::string> headers;
		std::string body;

		APIRequest(const std::string& apiRoot, const std::string& endpoint, const std::string& requestMethod, const std::map<std::string, std::string>& headers, const std::string& body)
			: apiRoot(apiRoot), endpoint(endpoint), requestMethod(requestMethod), headers(headers), body(body) {
		}

	public:
		template<typename T>
		APIResponse<T> execute() {
			RawHttpResponse rawResponse = execute();
			return APIResponse<T>(rawResponse.responseCode, rawResponse.responseMessage, rawResponse.responseBody, rawResponse.headers);
		}

		RawHttpResponse execute() const {
			CURL* curl = curl_easy_init();
			RawHttpResponse response;
			if (curl) {
				std::string url = apiRoot + endpoint;
				struct curl_slist* header_list = nullptr;
				for (const auto& h : headers) {
					header_list = curl_slist_append(header_list, (h.first + ": " + h.second).c_str());
				}

				curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.responseBody);

				curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HeaderCallback);
				curl_easy_setopt(curl, CURLOPT_HEADERDATA, &response);

				if (requestMethod == "POST") {
					curl_easy_setopt(curl, CURLOPT_POST, 1L);
					curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
				}
				else if (requestMethod == "PUT") {
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
					curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
				}
				else if (requestMethod == "DELETE") {
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
				}

				CURLcode res = curl_easy_perform(curl);
				if (res != CURLE_OK)
					std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;

				curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response.responseCode);

				curl_slist_free_all(header_list);
				curl_easy_cleanup(curl);
			}
			return response;
		}

	private:
		static size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
			auto* response = static_cast<std::string*>(userdata);
			response->append(ptr, size * nmemb);
			return size * nmemb;
		}

		static size_t HeaderCallback(char* buffer, size_t size, size_t nitems, void* userdata) {
			size_t totalSize = size * nitems;
			auto* response = static_cast<RawHttpResponse*>(userdata);
			std::string headerLine(buffer, totalSize);

			// Check for the status line (e.g., "HTTP/1.1 200 OK\r\n")
			if (headerLine.find("HTTP/") == 0) {
				// Remove trailing \r\n
				headerLine.erase(headerLine.find_last_not_of("\r\n") + 1);
				// Remove "HTTP/1.1 " prefix to get the status message
				size_t firstSpace = headerLine.find(' ');
				if (firstSpace != std::string::npos) {
					size_t secondSpace = headerLine.find(' ', firstSpace + 1);
					if (secondSpace != std::string::npos) {
						response->responseMessage = headerLine.substr(secondSpace + 1);
					}
				}
				else
					response->responseMessage = headerLine;
			}
			else {
				// Parse header: "Key: Value"
				auto colonPos = headerLine.find(':');
				if (colonPos != std::string::npos) {
					std::string key = headerLine.substr(0, colonPos);
					std::string value = headerLine.substr(colonPos + 1);
					// Trim whitespace and trailing \r\n
					key.erase(key.find_last_not_of(" \t\r\n") + 1);
					value.erase(0, value.find_first_not_of(" \t"));
					value.erase(value.find_last_not_of("\r\n") + 1);
					response->headers[key].push_back(value);
				}
			}
			return totalSize;
		}

	public:
		// Builder inner class
		class Builder {
		private:
			std::string apiRoot;
			std::string requestMethod = "GET";
			std::map<std::string, std::string> headers;
			std::string body;
			std::string resourcePath;
			std::string resourceId;
			std::string subResourcePath;
			std::string subResourceId;

		public:
			Builder& setApiRoot(const std::string& root) {
				this->apiRoot = root;
				return *this;
			}

			Builder& setMethod(const std::string& requestMethod) {
				this->requestMethod = requestMethod;
				return *this;
			}

			Builder& setAuthHeader(const std::string& authHeader) {
				this->headers["Authorization"] = authHeader;
				return *this;
			}

			Builder& addHeader(const std::string& key, const std::string& value) {
				this->headers[key] = value;
				return *this;
			}

			Builder& setBody(const std::string& body) {
				this->body = body;
				return *this;
			}

			Builder& setResourcePath(const std::string& resourcePath) {
				this->resourcePath = resourcePath;
				return *this;
			}

			Builder& setResourceId(const std::string& resourceId) {
				this->resourceId = resourceId;
				return *this;
			}

			Builder& setSubResourcePath(const std::string& subResourcePath) {
				this->subResourcePath = subResourcePath;
				return *this;
			}

			Builder& setSubResourceId(const std::string& subResourceId) {
				this->subResourceId = subResourceId;
				return *this;
			}

			APIRequest build() {
				if (apiRoot.empty()) {
					throw std::invalid_argument("API root must be set");
				}
				if (requestMethod.empty()) {
					throw std::invalid_argument("Request method must be set.");
				}

				std::string endpoint;
				endpoint = appendPath(endpoint, resourcePath);
				endpoint = appendPath(endpoint, resourceId);
				endpoint = appendPath(endpoint, subResourcePath);
				endpoint = appendPath(endpoint, subResourceId);

				return APIRequest(apiRoot, endpoint, requestMethod, headers, body);
			}

		private:
			static std::string appendPath(const std::string& base, const std::string& path) {
				size_t start = 0;
				size_t end = path.size();

				// Trim leading slashes
				while (start < end && path[start] == '/')
					++start;
				// Trim trailing slashes
				while (end > start && path[end - 1] == '/')
					--end;

				std::string trimmed = path.substr(start, end - start);
				if (trimmed.empty())
					return base;

				if (base.empty())
					return "/" + trimmed;

				// Ensure single slash between base and trimmed path
				if (base.back() == '/')
					return base + trimmed;
				else
					return base + "/" + trimmed;
			}
		};
	};
}