#pragma once

#include <string>

#include "DataStreamsAPI.h"
#include "CommandsAPI.h"
#include "ObservationsAPI.h"
#include "SystemsAPI.h"
#include "ControlStreamsAPI.h"
#include "Util/Utilities.h"
#include "RegistryInit.h" // Ensure registry is initialized before any API calls. Do not remove this include.

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
		CommandsAPI commandsAPI;

	public:
		/// <summary>
		/// Constructs a ConnectedSystemsAPI instance, used to access an API endpoint on a server.
		/// </summary>
		/// <param name="apiRoot">e.g. "http://localhost:8181/sensorhub/api". If no protocol is specified, http:// is used. https:// requires cpp-httplib built with SSL support.</param>
		/// <param name="authenticationToken">If isBasicAuth is true, this should be a base64-encoded "username:password" string. If isBasicAuth is false, this should be a bearer token.</param>
		/// <param name="isBasicAuth">If true, use Basic authentication with the provided base64-encoded "username:password" string. If false, use Bearer token authentication.</param>
		ConSysAPI(const std::string& apiRoot, const std::string& authenticationToken, bool isBasicAuth)
			: apiRoot(apiRoot),
			authHeader((isBasicAuth ? "Basic " : "Bearer ") + authenticationToken),
			systemsAPI(this->apiRoot, authHeader),
			dataStreamsAPI(this->apiRoot, authHeader),
			observationsAPI(this->apiRoot, authHeader),
			controlStreamsAPI(this->apiRoot, authHeader),
			commandsAPI(this->apiRoot, authHeader) {
		}

		/// <summary>
		/// Constructs a ConnectedSystemsAPI instance, used to access an API endpoint on a server.
		/// </summary>
		/// <param name="apiRoot">e.g. "http://localhost:8181/sensorhub/api". If no protocol is specified, http:// is used. https:// requires cpp-httplib built with SSL support.</param>
		/// <param name="username">Username for Basic authentication</param>
		/// <param name="password">Password for Basic authentication</param>
		ConSysAPI(const std::string& apiRoot, const std::string& username, const std::string& password)
			: ConSysAPI(apiRoot, Utilities::base64_encode(username + ":" + password), true) {
		}

		// Overload for a string literal password. Without it, a const char* password converts to bool
		// and selects the (apiRoot, authenticationToken, isBasicAuth) constructor instead.
		/// <summary>
		/// Constructs a ConnectedSystemsAPI instance, used to access an API endpoint on a server.
		/// </summary>
		/// <param name="apiRoot">e.g. "http://localhost:8181/sensorhub/api". If no protocol is specified, http:// is used. https:// requires cpp-httplib built with SSL support.</param>
		/// <param name="username">Username for Basic authentication</param>
		/// <param name="password">Password for Basic authentication</param>
		ConSysAPI(const std::string& apiRoot, const std::string& username, const char* password)
			: ConSysAPI(apiRoot, username, std::string(password)) {
		}

		const std::string& getApiRoot() const { return apiRoot; }
		/// <summary>
		/// The value of the Authorization header sent with each request, e.g., "Basic dXNlcjpwYXNz" or "Bearer token".
		/// </summary>
		const std::string& getAuthHeader() const { return authHeader; }
		SystemsAPI& getSystemsAPI() { return systemsAPI; }
		DataStreamsAPI& getDataStreamsAPI() { return dataStreamsAPI; }
		ObservationsAPI& getObservationsAPI() { return observationsAPI; }
		ControlStreamsAPI& getControlStreamsAPI() { return controlStreamsAPI; }
		CommandsAPI& getCommandsAPI() { return commandsAPI; }
	};
}