#pragma once

#include <chrono>
#include <optional>
#include <string>

#include "APIRequest.h"
#include "APIResponse.h"
#include "ConnectionStatus.h"
#include "DataModels/Conformance.h"
#include "DataModels/System.h"
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

		/// <summary>
		/// Default time limit for each request made by checkConnection.
		/// </summary>
		static constexpr std::chrono::milliseconds DEFAULT_CONNECTION_CHECK_TIMEOUT{ 5000 };

		/// <summary>
		/// Get the server's conformance declaration: the list of OGC API conformance classes it implements.
		/// </summary>
		/// <param name="timeout">Optional time limit for the request. Without it, httplib's defaults apply (up to 300 seconds).</param>
		APIResponse<DataModels::Conformance> getConformance(std::optional<std::chrono::milliseconds> timeout = std::nullopt) const {
			auto builder = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("conformance");
			if (timeout) builder.setTimeout(timeout.value());
			return builder.build().execute<DataModels::Conformance>();
		}

		/// <summary>
		/// Check whether the server can be reached, is a Connected Systems API server, and accepts the credentials.
		/// Requests the conformance declaration, then the first system, since some servers allow reading the
		/// conformance declaration without credentials.
		/// This blocks for up to twice the timeout, so call it off any UI or game thread.
		/// </summary>
		/// <param name="timeout">Time limit for each of the two requests.</param>
		ConnectionStatus checkConnection(std::chrono::milliseconds timeout = DEFAULT_CONNECTION_CHECK_TIMEOUT) const {
			auto conformance = getConformance(timeout);
			if (auto failure = connectionFailure(conformance.getResponseCode(), conformance.getResponseMessage(), conformance.isSuccessful()))
				return failure.value();
			if (!conformance.getItem().isConnectedSystemsAPI()) {
				return ConnectionStatus(ConnectionState::NOT_CONNECTED_SYSTEMS_API, conformance.getResponseCode(),
					"The server at " + apiRoot + " is an OGC API server, but does not implement the Connected Systems API");
			}

			auto systems = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("systems")
				.setQueryString("?limit=1")
				.setTimeout(timeout)
				.build()
				.execute<DataModels::System>();
			if (auto failure = connectionFailure(systems.getResponseCode(), systems.getResponseMessage(), systems.isSuccessful()))
				return failure.value();

			return ConnectionStatus(ConnectionState::CONNECTED, systems.getResponseCode(), "Connected to " + apiRoot);
		}

	private:
		/// <summary>
		/// The connection state a failed response indicates, or nullopt if the response succeeded.
		/// </summary>
		std::optional<ConnectionStatus> connectionFailure(int responseCode, const std::string& responseMessage, bool successful) const {
			if (responseCode == 0)
				return ConnectionStatus(ConnectionState::UNREACHABLE, 0, "Could not reach " + apiRoot + ": " + responseMessage);
			if (responseCode == 401 || responseCode == 403)
				return ConnectionStatus(ConnectionState::UNAUTHORIZED, responseCode, "The server at " + apiRoot + " rejected the credentials (HTTP "
					+ std::to_string(responseCode) + " " + responseMessage + ")");
			if (responseCode >= 500)
				return ConnectionStatus(ConnectionState::SERVER_ERROR, responseCode, "The server at " + apiRoot + " reported an error (HTTP "
					+ std::to_string(responseCode) + " " + responseMessage + ")");
			if (!successful)
				return ConnectionStatus(ConnectionState::NOT_CONNECTED_SYSTEMS_API, responseCode, "No Connected Systems API found at " + apiRoot + " (HTTP "
					+ std::to_string(responseCode) + (responseMessage.empty() ? "" : " " + responseMessage) + ")");
			return std::nullopt;
		}
	};
}