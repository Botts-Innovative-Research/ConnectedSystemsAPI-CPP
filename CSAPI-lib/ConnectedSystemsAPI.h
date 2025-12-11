#pragma once

#include <string>

#include "DataStreamsAPI.h"
#include "ObservationsAPI.h"
#include "SystemsAPI.h"

namespace ConnectedSystemsAPI {
	class ConSysAPI {
	private:
		std::string apiRoot;
		std::string authHeader;
		SystemsAPI systemsAPI;
		DataStreamsAPI dataStreamsAPI;
		ObservationsAPI observationsAPI;

	public:
		/// <param name="apiRoot">e.g. "localhost:8181/sensorhub/api"</param>
		/// <param name="authenticationToken">If isBasicAuth is true, this should be a base64-encoded "username:password" string. If isBasicAuth is false, this should be a bearer token.</param>
		/// <param name="isBasicAuth">If true, use Basic authentication with the provided base64-encoded "username:password" string. If false, use Bearer token authentication.</param>
		ConSysAPI(const std::string& apiRoot, const std::string& authenticationToken, bool isBasicAuth)
			: apiRoot(apiRoot) {
			if (isBasicAuth) {
				authHeader = "Authorization: Basic " + authenticationToken;
			}
			else {
				authHeader = "Authorization: Bearer " + authenticationToken;
			}
			systemsAPI = SystemsAPI(apiRoot, authHeader);
			dataStreamsAPI = DataStreamsAPI(apiRoot, authHeader);
			observationsAPI = ObservationsAPI(apiRoot, authHeader);
		}

		// Overload to accept C-style string literals to avoid list-initialization narrowing issues
		ConSysAPI(const char* apiRootC, const char* usernameC, const char* passwordC)
			: ConSysAPI(std::string(apiRootC), std::string(usernameC), std::string(passwordC)) {
		}

		/// <param name="apiRoot">e.g. "localhost:8181/sensorhub/api"</param>
		/// <param name="username">Username for Basic authentication</param>
		/// <param name="password">Password for Basic authentication</param>
		ConSysAPI(const std::string& apiRoot, const std::string& username, const std::string& password)
			: ConSysAPI(apiRoot, base64_encode(username + ":" + password), true) {
		}

		const std::string& getApiRoot() const { return apiRoot; }
		const std::string& getAuthHeader() const { return authHeader; }
		SystemsAPI& getSystemsAPI() { return systemsAPI; }
		DataStreamsAPI& getDataStreamsAPI() { return dataStreamsAPI; }
		ObservationsAPI getObservationsAPI() { return observationsAPI; }
	private:
		std::string base64_encode(const std::string& in) {
			static const std::string base64_chars =
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz"
				"0123456789+/";

			std::string out;
			int val = 0, valb = -6;
			for (unsigned char c : in) {
				val = (val << 8) + c;
				valb += 8;
				while (valb >= 0) {
					out.push_back(base64_chars[(val >> valb) & 0x3F]);
					valb -= 6;
				}
			}
			if (valb > -6) out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
			while (out.size() % 4) out.push_back('=');
			return out;
		}
	};
}