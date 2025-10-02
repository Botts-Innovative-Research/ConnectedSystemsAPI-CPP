#pragma once

#include <string>
#include "APIRequest.h"
#include "APIResponse.h"
#include "DataModels/DataStream.h"
#include "DataModels/ObservationSchema.h"
#include "Query/DataStreamsOfSystemQuery.h"
#include "Query/DataStreamsQuery.h"

namespace ConnectedSystemsAPI {
	class DataStreamsAPI {
	private:
		std::string apiRoot;
		std::string authHeader;

	public:
		DataStreamsAPI() {}
		DataStreamsAPI(const std::string& apiRoot, const std::string& authHeader)
			: apiRoot(apiRoot), authHeader(authHeader) {
		}

		/// <summary>
		/// List all data streams available from this server endpoint.
		/// </summary>
		/// <param name="query">The query string parameters.</param>
		/// <returns>A response object containing a list of data streams.</returns>
		APIResponse<DataModels::DataStream> getDataStreams(const Query::DataStreamsQuery& query) {
			return getDataStreams(query.toString());
		}

		/// <summary>
		/// List all data streams available from this server endpoint.
		/// </summary>
		/// <param name="queryString">The query string.</param>
		/// <returns>A response object containing a list of data streams.</returns>
		APIResponse<DataModels::DataStream> getDataStreams(std::string queryString = "") {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("/datastreams" + queryString)
				.build()
				.execute<DataModels::DataStream>();
			return response;
		}

		/// <summary>
		/// List all data streams available from the parent system.
		/// </summary>
		/// <param name="systemId">The local identifier of a system.</param>
		/// <param name="query">The query string parameters.</param>
		/// <returns>A response object containing a list of data streams.</returns>
		APIResponse<DataModels::DataStream> getDataStreamsOfSystem(const std::string& systemId, const Query::DataStreamsOfSystemQuery& query) {
			return getDataStreamsOfSystem(systemId, query.toString());
		}

		/// <summary>
		/// List all data streams available from the parent system.
		/// </summary>
		/// <param name="systemId">The local identifier of a system.</param>
		/// <param name="query">The query string.</param>
		/// <returns>A response object containing a list of data streams.</returns>
		APIResponse<DataModels::DataStream> getDataStreamsOfSystem(const std::string& systemId, std::string queryString = "") {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("/systems/" + systemId + "/datastreams" + queryString)
				.build()
				.execute<DataModels::DataStream>();
			return response;
		}

		/// <summary>
		/// Get a specific data stream by its ID.
		/// </summary>
		/// <param name="dataStreamId">The local identifier of a data stream.</param>
		/// <param name="queryString">The query string.</param>
		/// <returns>A response object containing a list of data streams.</returns>
		APIResponse<DataModels::DataStream> getDataStreamById(const std::string& dataStreamId, std::string queryString = "") {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("/datastreams/" + dataStreamId + queryString)
				.build()
				.execute<DataModels::DataStream>();
			return response;
		}

		APIResponse<DataModels::ObservationSchema> getObservationSchema(const std::string& dataStreamId) {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("/datastreams/" + dataStreamId + "/schema")
				.build()
				.execute<DataModels::ObservationSchema>();
			return response;
		}
	};
}