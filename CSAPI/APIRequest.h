#pragma once

#include <algorithm>
#include <cctype>
#include <string>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <httplib.h>

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
			RawHttpResponse response;

			// Split the API root into scheme, host[:port], and base path. No scheme means http.
			size_t schemeEnd = apiRoot.find("://");
			std::string scheme = schemeEnd == std::string::npos ? "http" : apiRoot.substr(0, schemeEnd);
			std::transform(scheme.begin(), scheme.end(), scheme.begin(),
				[](unsigned char c) { return static_cast<char>(std::tolower(c)); });

			size_t hostStart = schemeEnd == std::string::npos ? 0 : schemeEnd + 3;
			size_t pathStart = apiRoot.find('/', hostStart);
			std::string hostAndPort = pathStart == std::string::npos
				? apiRoot.substr(hostStart)
				: apiRoot.substr(hostStart, pathStart - hostStart);
			std::string basePath = pathStart == std::string::npos ? "" : apiRoot.substr(pathStart);

			// httplib throws for schemes it can't handle, so report them as a failed request instead
#if defined(CPPHTTPLIB_SSL_ENABLED) || defined(CPPHTTPLIB_OPENSSL_SUPPORT)
			const bool httpsSupported = true;
#else
			const bool httpsSupported = false;
#endif
			if (scheme != "http" && !(scheme == "https" && httpsSupported)) {
				response.responseMessage = scheme == "https"
					? "HTTPS is not supported: cpp-httplib was built without SSL support (define CPPHTTPLIB_OPENSSL_SUPPORT)"
					: "Unsupported URL scheme: " + scheme;
				return response;
			}

			httplib::Client cli(scheme + "://" + hostAndPort);
			if (!cli.is_valid()) {
				response.responseMessage = "Invalid API root: " + apiRoot;
				return response;
			}
			std::string fullEndpoint = basePath + endpoint;

			// Build headers
			httplib::Headers h;
			for (const auto& kv : headers)
			{
				h.insert({ kv.first.c_str(), kv.second.c_str() });
			}

			// Perform request
			httplib::Result res;

			if (requestMethod == "POST")
			{
				res = cli.Post(fullEndpoint.c_str(), h, body, "application/json");
			}
			else if (requestMethod == "PUT")
			{
				res = cli.Put(fullEndpoint.c_str(), h, body, "application/json");
			}
			else if (requestMethod == "DELETE")
			{
				res = cli.Delete(fullEndpoint.c_str(), h);
			}
			else // GET
			{
				res = cli.Get(fullEndpoint.c_str(), h);
			}

			// Fill response struct
			if (res)
			{
				response.responseCode = res->status;
				response.responseMessage = res->reason;
				response.responseBody = res->body;

				// Copy headers
				for (const auto& kv : res->headers)
				{
					response.headers[kv.first].push_back(kv.second);
				}
			}
			else
			{
				// httplib error (connection failure, timeout, etc.)
				response.responseCode = 0;
				response.responseMessage = "HTTP request failed: " + httplib::to_string(res.error());
			}

			return response;
		}

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
			std::string queryString;

		public:
			Builder& setApiRoot(const std::string& root) {
				this->apiRoot = root;
				return *this;
			}

			Builder& setMethod(const std::string& requestMethod) {
				this->requestMethod = requestMethod;
				return *this;
			}

			/// <summary>
			/// Sets the value of the Authorization header, e.g., "Basic dXNlcjpwYXNz" (without the "Authorization: " prefix).
			/// </summary>
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

			Builder& setQueryString(const std::string& queryString) {
				this->queryString = queryString;
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
				endpoint += queryString;

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