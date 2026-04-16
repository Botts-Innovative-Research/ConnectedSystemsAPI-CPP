#pragma once

#include <string>

#include "APIRequest.h"
#include "APIResponse.h"
#include "DataModels/Command.h"
#include "Query/CommandsQuery.h"
#include "Query/CommandsOfControlStreamQuery.h"

namespace ConnectedSystemsAPI {
	/// <summary>
	/// API for interacting with commands in the Connected Systems API.
	/// </summary>
	class CommandsAPI {
	private:
		std::string apiRoot;
		std::string authHeader;

	public:
		CommandsAPI() = default;
		CommandsAPI(const std::string& apiRoot, const std::string& authHeader)
			: apiRoot(apiRoot), authHeader(authHeader) {
		}

		/// <summary>
		/// List or search all commands available from this server endpoint.
		/// </summary>
		/// <param name="query">The query string parameters.</param>
		/// <returns>A response object containing a list of commands.</returns>
		APIResponse<DataModels::Command> fetchCommands(const Query::CommandsQuery& query) const {
			return fetchCommands(query.toString());
		}

		/// <summary>
		/// List or search all commands available from this server endpoint.
		/// </summary>
		/// <param name="queryString">The query string.</param>
		/// <returns>A response object containing a list of commands.</returns>
		APIResponse<DataModels::Command> fetchCommands(const std::string& queryString = "") const {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("commands")
				.setQueryString(queryString)
				.build()
				.execute<DataModels::Command>();
			return response;
		}

		/// <summary>
		/// List or search all commands available from a control stream.
		/// </summary>
		/// <param name="controlStreamId">The local identifier of the control stream.</param>
		/// <param name="query">The query string parameters.</param>
		/// <returns>A response object containing a list of commands.</returns>
		APIResponse<DataModels::Command> fetchCommandsOfControlStream(const std::string& controlStreamId, const Query::CommandsOfControlStreamQuery& query) const {
			return fetchCommandsOfControlStream(controlStreamId, query.toString());
		}

		/// <summary>
		/// List or search all commands available from a control stream.
		/// </summary>
		/// <param name="controlStreamId">The local identifier of the control stream.</param>
		/// <param name="queryString">The query string.</param>
		/// <returns>A response object containing a list of commands.</returns>
		APIResponse<DataModels::Command> fetchCommandsOfControlStream(const std::string& controlStreamId, const std::string& queryString = "") const {
			if (controlStreamId.empty())
				return APIResponse<DataModels::Command>(400, "Invalid controlStreamId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("controlstreams")
				.setResourceId(controlStreamId)
				.setSubResourcePath("commands")
				.setQueryString(queryString)
				.build()
				.execute<DataModels::Command>();
			return response;
		}

		/// <summary>
		/// Get a specific command by its ID.
		/// </summary>
		/// <param name="commandId">The ID of the command to retrieve.</param>
		/// <returns>A response object containing the requested command.</returns>
		APIResponse<DataModels::Command> fetchCommandById(const std::string& commandId) const {
			if (commandId.empty())
				return APIResponse<DataModels::Command>(400, "Invalid commandId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("commands")
				.setResourceId(commandId)
				.build()
				.execute<DataModels::Command>();
			return response;
		}

		/// <summary>
		/// Add a new command to an existing control stream.
		/// </summary>
		/// <param name="controlStreamId">The local identifier of the control stream.</param>
		/// <param name="command">The command to create.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> createCommand(const std::string& controlStreamId, const DataModels::Command& command) const {
			if (controlStreamId.empty())
				return APIResponse<void>(400, "Invalid controlStreamId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("POST")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("controlstreams")
				.setResourceId(controlStreamId)
				.setSubResourcePath("commands")
				.setBody(command.toJson().dump())
				.build()
				.execute<void>();
			return response;
		}

		/// <summary>
		/// Update an existing command.
		/// </summary>
		/// <param name="commandId">The ID of the command to update.</param>
		/// <param name="command">The updated command.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> updateCommand(const std::string& commandId, const DataModels::Command& command) const {
			if (commandId.empty())
				return APIResponse<void>(400, "Invalid commandId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("PUT")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("commands")
				.setResourceId(commandId)
				.setBody(command.toJson().dump())
				.build()
				.execute<void>();
			return response;
		}

		/// <summary>
		/// Delete a command by its ID.
		/// </summary>
		/// <param name="commandId">The ID of the command to delete.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> deleteCommand(const std::string& commandId) const {
			if (commandId.empty())
				return APIResponse<void>(400, "Invalid commandId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("DELETE")
				.setAuthHeader(authHeader)
				.setResourcePath("commands")
				.setResourceId(commandId)
				.build()
				.execute<void>();
			return response;
		}
	};
}