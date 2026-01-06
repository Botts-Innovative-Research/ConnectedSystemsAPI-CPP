#pragma once

#include <string>

#include "APIRequest.h"
#include "APIResponse.h"
#include "DataModels/ControlStream.h"
#include "DataModels/CommandSchema.h"

namespace ConnectedSystemsAPI {
	/// <summary>
	/// API for interacting with control streams in the Connected Systems API.
	/// </summary>
	class ControlStreamsAPI {
	private:
		std::string apiRoot;
		std::string authHeader;

	public:
		ControlStreamsAPI() = default;
		ControlStreamsAPI(const std::string& apiRoot, const std::string& authHeader)
			: apiRoot(apiRoot), authHeader(authHeader) {
		}

		/// <summary>
		/// List all control streams available from this server endpoint.
		/// </summary>
		/// <param name="queryString">The query string.</param>
		/// <returns>A response object containing a list of control streams.</returns>
		APIResponse<DataModels::ControlStream> getControlStreams(const std::string& queryString = "") const {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("controlstreams")
				.setQueryString(queryString)
				.build()
				.execute<DataModels::ControlStream>();
			return response;
		}

		/// <summary>
		/// List all control streams available from the parent system.
		/// </summary>
		/// <param name="systemId">The local identifier of a system.</param>
		/// <param name="queryString">The query string.</param>
		/// <returns>A response object containing a list of control streams.</returns>
		APIResponse<DataModels::ControlStream> getControlStreamsOfSystem(const std::string& systemId, const std::string& queryString = "") const {
			if (systemId.empty())
				return APIResponse<DataModels::ControlStream>(400, "Invalid systemId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("systems")
				.setResourceId(systemId)
				.setSubResourcePath("controlstreams")
				.setQueryString(queryString)
				.build()
				.execute<DataModels::ControlStream>();
			return response;
		}

		/// <summary>
		/// Get a specific control stream by its ID.
		/// </summary>
		/// <param name="controlStreamId">The local identifier of a control stream.</param>
		/// <returns>A response object containing the control stream.</returns>
		APIResponse<DataModels::ControlStream> getControlStreamById(const std::string& controlStreamId) const {
			if (controlStreamId.empty())
				return APIResponse<DataModels::ControlStream>(400, "Invalid controlStreamId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("controlstreams")
				.setResourceId(controlStreamId)
				.build()
				.execute<DataModels::ControlStream>();
			return response;
		}

		/// <summary>
		/// Get the command schema for a specific control stream by its ID.
		/// </summary>
		/// <param name="controlStreamId">The local identifier of a control stream.</param>
		/// <returns>A response object containing the command schema.</returns>
		APIResponse<DataModels::CommandSchema> getControlStreamSchema(const std::string& controlStreamId) const {
			if (controlStreamId.empty())
				return APIResponse<DataModels::CommandSchema>(400, "Invalid controlStreamId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("controlstreams")
				.setResourceId(controlStreamId)
				.setSubResourcePath("schema")
				.build()
				.execute<DataModels::CommandSchema>();
			return response;
		}

		/// <summary>
		/// Create a new control stream under the specified parent system.
		/// </summary>
		/// <param name="systemId">The local identifier of the parent system.</param>
		/// <param name="controlStream">The control stream to create.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> createControlStream(const std::string& systemId, const DataModels::ControlStream& controlStream) const {
			if (systemId.empty())
				return APIResponse<void>(400, "Invalid systemId", "", {});

			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, controlStream);

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("POST")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("systems")
				.setResourceId(systemId)
				.setSubResourcePath("controlstreams")
				.setBody(j.dump())
				.build()
				.execute<void>();
			return response;
		}

		/// <summary>
		/// Update an existing control stream by its ID.
		/// </summary>
		/// <param name="controlStreamId">The local identifier of the control stream to update.</param>
		/// <param name="controlStream">The updated control stream.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> updateControlStream(const std::string& controlStreamId, const DataModels::ControlStream& controlStream) const {
			if (controlStreamId.empty())
				return APIResponse<void>(400, "Invalid controlStreamId", "", {});

			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, controlStream);

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("PUT")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("controlstreams")
				.setResourceId(controlStreamId)
				.setBody(j.dump())
				.build()
				.execute<void>();
			return response;
		}

		/// <summary>
		/// Delete a control stream by its ID, with an option to delete all associated commands.
		/// </summary>
		/// <param name="controlStreamId">The local identifier of the control stream to delete.</param>
		/// <param name="cascade">If true, all associated commands will also be deleted.
		///                       If false, associated commands will be preserved.
		///                       If undefined, the server's default behavior will be applied (false unless otherwise specified).</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> deleteControlStream(const std::string& controlStreamId, std::optional<bool> cascade = std::nullopt) const {
			if (controlStreamId.empty())
				return APIResponse<void>(400, "Invalid controlStreamId", "", {});

			std::string queryString;
			if (cascade.has_value())
				queryString = cascade.value() ? "?cascade=true" : "?cascade=false";

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("DELETE")
				.setAuthHeader(authHeader)
				.setResourcePath("controlstreams")
				.setResourceId(controlStreamId)
				.setQueryString(queryString)
				.build()
				.execute<void>();
			return response;
		}
	};
}