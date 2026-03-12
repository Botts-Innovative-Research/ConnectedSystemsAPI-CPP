#pragma once

#include <string>
#include <nlohmann/json_fwd.hpp>

#include "APIRequest.h"
#include "APIResponse.h"
#include "DataModels/System.h"
#include "DataModels/TimeExtent.h"
#include "Query/SystemsQuery.h"

namespace ConnectedSystemsAPI {
	class SystemsAPI {
	private:
		std::string apiRoot;
		std::string authHeader;

	public:
		SystemsAPI() = delete;
		SystemsAPI(const std::string& apiRoot, const std::string& authHeader)
			: apiRoot(apiRoot), authHeader(authHeader) {
		}

		/// <summary>
		/// List all systems available from this server endpoint.
		/// </summary>
		/// <param name="query">The query string parameters.</param>
		/// <returns>A response object containing a list of systems.</returns>
		APIResponse<DataModels::System> getSystems(const Query::SystemsQuery& query) const {
			return getSystems(query.toString());
		}

		/// <summary>
		/// List all systems available from this server endpoint.
		/// By default, only top level systems are included.
		/// </summary>
		/// <param name="queryString">The query string.</param>
		/// <returns>A response object containing a list of systems.</returns>
		APIResponse<DataModels::System> getSystems(std::string queryString = "") const {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("systems")
				.setQueryString(queryString)
				.build()
				.execute<DataModels::System>();
			return response;
		}

		/// <summary>
		/// List all systems that are subsystems (i.e., components) of a specific parent system.
		/// </summary>
		/// <param name="systemId">The ID of the parent system.</param>
		/// <param name="query">The query string parameters.</param>
		/// <returns>A response object containing a list of subsystems.</returns>
		APIResponse<DataModels::System> getSubsystems(const std::string& systemId, const Query::SystemsQuery& query) const {
			getSubsystems(systemId, query.toString());
		}

		/// <summary>
		/// List all systems that are subsystems (i.e., components) of a specific parent system.
		/// </summary>
		/// <param name="systemId">The ID of the parent system.</param>
		/// <param name="query">The query string.</param>
		/// <returns>A response object containing a list of subsystems.</returns>
		APIResponse<DataModels::System> getSubsystems(const std::string& systemId, std::string queryString = "") const {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("systems")
				.setResourceId(systemId)
				.setSubResourcePath("subsystems")
				.setQueryString(queryString)
				.build()
				.execute<DataModels::System>();
			return response;
		}

		/// <summary>
		/// Get the latest system valid before or at the specified time.
		/// </summary>
		/// <param name="systemId">The ID of the system to retrieve.</param>
		/// <param name="queryString">The query string.</param>
		/// <returns>A response object containing the system.</returns>
		APIResponse<DataModels::System> getSystemById(const std::string& systemId, const DataModels::TimeExtent& datetime) const {
			return getSystemById(systemId, "?datetime=" + datetime.toStringUTC());
		}

		/// <summary>
		/// Get the latest system valid before or at the current time, by default.
		/// </summary>
		/// <param name="systemId">The ID of the system to retrieve.</param>
		/// <param name="queryString">The query string.</param>
		/// <returns>A response object containing the system.</returns>
		APIResponse<DataModels::System> getSystemById(const std::string& systemId, std::string queryString = "") const {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("systems")
				.setResourceId(systemId)
				.setQueryString(queryString)
				.build()
				.execute<DataModels::System>();
			return response;
		}

		/// <summary>
		/// Add a new top-level system to the server (i.e., the system will have no parent).
		/// </summary>
		/// <param name="system">The system to create.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> createSystem(const DataModels::System& system) const {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, system);
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("POST")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("systems")
				.setBody(j.dump())
				.build()
				.execute<void>();
			return response;
		}

		/// <summary>
		/// Add a new system as a subsystem of an existing system.
		/// </summary>
		/// <param name="systemId">The ID of the parent system.</param>
		/// <param name="subsystem">The subsystem to create.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> createSubsystem(const std::string& systemId, const DataModels::System& subsystem) const {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, subsystem);
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("POST")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("systems")
				.setResourceId(systemId)
				.setSubResourcePath("subsystems")
				.setBody(j.dump())
				.build()
				.execute<void>();
			return response;
		}

		/// <summary>
		/// This will completely replace the existing description of the system with the provided system object.
		/// If system history is supported and the validTime property starts after the time of the previous description,
		/// the provided description becomes the current one,
		/// and all previous descriptions are made available via the history subcollection.
		/// </summary>
		/// <param name="systemId">The ID of the system to update.</param>
		/// <param name="system">The updated system object.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> updateSystem(const std::string& systemId, const DataModels::System& system) const {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, system);
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("PUT")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("systems")
				.setResourceId(systemId)
				.setBody(j.dump())
				.build()
				.execute<void>();
			return response;
		}

		/// <summary>
		/// Delete the given system from the server and remove it from all collections it is associated to.
		/// </summary>
		/// <param name="systemId">The ID of the system to delete.</param>
		/// <param name="cascade">If true, all associated sub-resources hosted by the same server,
		///                       (sampling features, data streams, command streams, observations, and commands)
		///                       are also deleted.</param>
		/// <returns></returns>
		APIResponse<void> deleteSystem(const std::string& systemId, const bool cascade = false) const {
			std::string queryString = cascade ? "?cascade=true" : "";
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("DELETE")
				.setAuthHeader(authHeader)
				.setResourcePath("systems")
				.setResourceId(systemId)
				.setQueryString(queryString)
				.build()
				.execute<void>();
			return response;
		}
	};
}