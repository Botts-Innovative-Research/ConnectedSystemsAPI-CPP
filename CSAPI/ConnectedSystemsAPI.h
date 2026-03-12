#pragma once

#include <string>

#include "DataStreamsAPI.h"
#include "ObservationsAPI.h"
#include "SystemsAPI.h"
#include "ControlStreamsAPI.h"
#include "Util/Utilities.h"

namespace ConnectedSystemsAPI {
	/// <summary>
	/// Main API class for interacting with the Connected Systems API on a server.
	/// </summary>
	class ConSysAPI {
	private:
		std::string apiRoot;
		std::string authHeader;
		SystemsAPI systemsAPI;
		DataStreamsAPI dataStreamsAPI;
		ObservationsAPI observationsAPI;
		ControlStreamsAPI controlStreamsAPI;

	public:
		/// <summary>
		/// Constructs a ConnectedSystemsAPI instance, used to access an API endpoint on a server.
		/// </summary>
		/// <param name="apiRoot">e.g. "localhost:8181/sensorhub/api". If no protocol is specified, defaults to https://</param>
		/// <param name="authenticationToken">If isBasicAuth is true, this should be a base64-encoded "username:password" string. If isBasicAuth is false, this should be a bearer token.</param>
		/// <param name="isBasicAuth">If true, use Basic authentication with the provided base64-encoded "username:password" string. If false, use Bearer token authentication.</param>
		ConSysAPI(const std::string& apiRoot, const std::string& authenticationToken, bool isBasicAuth)
			: apiRoot(apiRoot),
			authHeader(isBasicAuth ? std::string("Authorization: Basic ") + authenticationToken : std::string("Authorization: Bearer ") + authenticationToken),
			systemsAPI(this->apiRoot, authHeader),
			dataStreamsAPI(this->apiRoot, authHeader),
			observationsAPI(this->apiRoot, authHeader),
			controlStreamsAPI(this->apiRoot, authHeader) {
		}

		/// <summary>
		/// Constructs a ConnectedSystemsAPI instance, used to access an API endpoint on a server.
		/// </summary>
		/// <param name="apiRoot">e.g. "localhost:8181/sensorhub/api". If no protocol is specified, defaults to https://</param>
		/// <param name="username">Username for Basic authentication</param>
		/// <param name="password">Password for Basic authentication</param>
		ConSysAPI(const std::string& apiRoot, const std::string& username, const std::string& password)
			: ConSysAPI(apiRoot, Utilities::base64_encode(username + ":" + password), true) {
		}

		// Overload to accept C-style string literals to avoid list-initialization narrowing issues
		/// <summary>
		/// Constructs a ConnectedSystemsAPI instance, used to access an API endpoint on a server.
		/// </summary>
		/// <param name="apiRoot">e.g. "localhost:8181/sensorhub/api". If no protocol is specified, defaults to https://</param>
		/// <param name="username">Username for Basic authentication</param>
		/// <param name="password">Password for Basic authentication</param>
		ConSysAPI(const char* apiRootC, const char* usernameC, const char* passwordC)
			: ConSysAPI(std::string(apiRootC), std::string(usernameC), std::string(passwordC)) {
		}

		const std::string& getApiRoot() const { return apiRoot; }
		const std::string& getAuthHeader() const { return authHeader; }
		SystemsAPI& getSystemsAPI() { return systemsAPI; }
		DataStreamsAPI& getDataStreamsAPI() { return dataStreamsAPI; }
		ObservationsAPI& getObservationsAPI() { return observationsAPI; }
		ControlStreamsAPI& getControlStreamsAPI() { return controlStreamsAPI; }
	};
}