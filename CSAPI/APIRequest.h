#pragma once

#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <curl/curl.h>
#include <curl/easy.h>

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

			// Check for the status line (e.g., "HTTP/1.1 200 OK\r\n")
			if (std::string headerLine(buffer, totalSize); headerLine.find("HTTP/") == 0) {
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
			std::string m_apiRoot;
			std::string m_requestMethod = "GET";
			std::map<std::string, std::string> m_headers;
			std::string m_body;
			std::string m_resourcePath;
			std::string m_resourceId;
			std::string m_subResourcePath;
			std::string m_subResourceId;
			std::string m_queryString;

		public:
			Builder& setApiRoot(const std::string& apiRoot) {
				m_apiRoot = apiRoot;
				return *this;
			}

			Builder& setMethod(const std::string& requestMethod) {
				m_requestMethod = requestMethod;
				return *this;
			}

			Builder& setAuthHeader(const std::string& authHeader) {
				m_headers["Authorization"] = authHeader;
				return *this;
			}

			Builder& addHeader(const std::string& key, const std::string& value) {
				m_headers[key] = value;
				return *this;
			}

			Builder& setBody(const std::string& body) {
				m_body = body;
				return *this;
			}

			Builder& setResourcePath(const std::string& resourcePath) {
				m_resourcePath = resourcePath;
				return *this;
			}

			Builder& setResourceId(const std::string& resourceId) {
				m_resourceId = resourceId;
				return *this;
			}

			Builder& setSubResourcePath(const std::string& subResourcePath) {
				m_subResourcePath = subResourcePath;
				return *this;
			}

			Builder& setSubResourceId(const std::string& subResourceId) {
				m_subResourceId = subResourceId;
				return *this;
			}

			Builder& setQueryString(const std::string& queryString) {
				m_queryString = queryString;
				return *this;
			}

			APIRequest build() {
				if (m_apiRoot.empty()) {
					throw std::invalid_argument("API root must be set");
				}
				if (m_requestMethod.empty()) {
					throw std::invalid_argument("Request method must be set.");
				}

				std::string endpointString;
				endpointString = appendPath(endpointString, m_resourcePath);
				endpointString = appendPath(endpointString, m_resourceId);
				endpointString = appendPath(endpointString, m_subResourcePath);
				endpointString = appendPath(endpointString, m_subResourceId);
				endpointString += m_queryString;

				return APIRequest(m_apiRoot, endpointString, m_requestMethod, m_headers, m_body);
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