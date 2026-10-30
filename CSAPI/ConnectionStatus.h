#pragma once

#include <string>
#include <utility>

namespace ConnectedSystemsAPI {
	/// <summary>
	/// The outcome of checking the connection to a Connected Systems API server.
	/// </summary>
	enum class ConnectionState {
		/// <summary>
		/// The server is a Connected Systems API server and accepted the credentials.
		/// </summary>
		CONNECTED,

		/// <summary>
		/// No HTTP response: the host could not be resolved, refused the connection, or did not answer in time.
		/// </summary>
		UNREACHABLE,

		/// <summary>
		/// The server rejected the credentials (HTTP 401 or 403).
		/// </summary>
		UNAUTHORIZED,

		/// <summary>
		/// Something answered, but there is no Connected Systems API at the API root, e.g., a wrong path or a different kind of server.
		/// </summary>
		NOT_CONNECTED_SYSTEMS_API,

		/// <summary>
		/// The server reported an error (HTTP 5xx).
		/// </summary>
		SERVER_ERROR
	};

	inline const char* toString(ConnectionState state) {
		switch (state) {
		case ConnectionState::CONNECTED:                 return "CONNECTED";
		case ConnectionState::UNREACHABLE:               return "UNREACHABLE";
		case ConnectionState::UNAUTHORIZED:              return "UNAUTHORIZED";
		case ConnectionState::NOT_CONNECTED_SYSTEMS_API: return "NOT_CONNECTED_SYSTEMS_API";
		case ConnectionState::SERVER_ERROR:              return "SERVER_ERROR";
		}
		return "UNKNOWN";
	}

	/// <summary>
	/// Result of checking the connection to a Connected Systems API server.
	/// </summary>
	class ConnectionStatus {
	private:
		ConnectionState state;
		int responseCode;
		std::string message;

	public:
		ConnectionStatus(ConnectionState state, int responseCode, std::string message)
			: state(state), responseCode(responseCode), message(std::move(message)) {
		}

		/// <summary>
		/// True if the server is a Connected Systems API server and accepted the credentials.
		/// </summary>
		bool isConnected() const { return state == ConnectionState::CONNECTED; }

		ConnectionState getState() const { return state; }

		/// <summary>
		/// HTTP status code of the response that determined the state, or 0 if there was no response.
		/// </summary>
		int getResponseCode() const { return responseCode; }

		/// <summary>
		/// Human-readable description of the state, suitable for logging or display.
		/// </summary>
		const std::string& getMessage() const { return message; }
	};
}
