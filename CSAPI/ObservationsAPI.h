#pragma once

#include <string>

#include "APIRequest.h"
#include "APIResponse.h"
#include "DataModels/Observation.h"
#include "Query/ObservationsQuery.h"
#include "Query/ObservationsOfDataStreamQuery.h"

namespace ConnectedSystemsAPI {
	/// <summary>
	/// API for interacting with observations in the Connected Systems API.
	/// </summary>
	class ObservationsAPI {
	private:
		std::string apiRoot;
		std::string authHeader;

	public:
		ObservationsAPI() = default;
		ObservationsAPI(const std::string& apiRoot, const std::string& authHeader)
			: apiRoot(apiRoot), authHeader(authHeader) {
		}

		/// <summary>
		/// List or search all observations available from this server endpoint.
		/// </summary>
		/// <param name="query">The query string parameters.</param>
		/// <returns>A response object containing a list of observations.</returns>
		APIResponse<DataModels::Observation> fetchObservations(const Query::ObservationsQuery& query) const {
			return fetchObservations(query.toString());
		}

		/// <summary>
		/// List or search all observations available from this server endpoint.
		/// </summary>
		/// <param name="query">The query string.</param>
		/// <returns>A response object containing a list of observations.</returns>
		APIResponse<DataModels::Observation> fetchObservations(const std::string& queryString = "") const {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("observations")
				.setQueryString(queryString)
				.build()
				.execute<DataModels::Observation>();
			return response;
		}

		/// <summary>
		/// List or search all observations available from a datastream.
		/// </summary>
		/// <param name="query">The query string parameters.</param>
		/// <returns>A response object containing a list of observations.</returns>
		APIResponse<DataModels::Observation> fetchObservationsOfDataStream(const std::string& dataStreamId, const Query::ObservationsOfDataStreamQuery& query) const {
			return fetchObservationsOfDataStream(dataStreamId, query.toString());
		}

		/// <summary>
		/// List or search all observations available from a datastream.
		/// </summary>
		/// <param name="query">The query string.</param>
		/// <returns>A response object containing a list of observations.</returns>
		APIResponse<DataModels::Observation> fetchObservationsOfDataStream(const std::string& dataStreamId, const std::string& queryString = "") const {
			if (dataStreamId.empty())
				return APIResponse<DataModels::Observation>(400, "Invalid dataStreamId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("datastreams")
				.setResourceId(dataStreamId)
				.setSubResourcePath("observations")
				.setQueryString(queryString)
				.build()
				.execute<DataModels::Observation>();
			return response;
		}

		/// <summary>
		/// Get a specific observation by its ID.
		/// </summary>
		/// <param name="observationId">The ID of the observation to retrieve.</param>
		/// <returns>A response object containing the requested observation.</returns>
		APIResponse<DataModels::Observation> fetchObservationById(const std::string& observationId) const {
			if (observationId.empty())
				return APIResponse<DataModels::Observation>(400, "Invalid observationId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("observations")
				.setResourceId(observationId)
				.build()
				.execute<DataModels::Observation>();
			return response;
		}

		/// <summary>
		/// Add a new observation to an existing data stream.
		/// </summary>
		/// <param name="dataStreamId">The local identifier of the data stream.</param>
		/// <param name="observation">The observation to create.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> createObservation(const std::string& dataStreamId, const DataModels::Observation& observation) const {
			if (dataStreamId.empty())
				return APIResponse<void>(400, "Invalid dataStreamId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("POST")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("datastreams")
				.setResourceId(dataStreamId)
				.setSubResourcePath("observations")
				.setBody(observation.toJson().dump())
				.build()
				.execute<void>();
			return response;
		}

		/// <summary>
		/// Update an existing observation.
		/// </summary>
		/// <param name="observationId">The ID of the observation to update.</param>
		/// <param name="observation">The updated observation.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> updateObservation(const std::string& observationId, const DataModels::Observation& observation) const {
			if (observationId.empty())
				return APIResponse<void>(400, "Invalid observationId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("PUT")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("observations")
				.setResourceId(observationId)
				.setBody(observation.toJson().dump())
				.build()
				.execute<void>();
			return response;
		}

		/// <summary>
		/// Delete an observation by its ID.
		/// </summary>
		/// <param name="observationId">The ID of the observation to delete.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> deleteObservation(const std::string& observationId) const {
			if (observationId.empty())
				return APIResponse<void>(400, "Invalid observationId", "", {});

			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("DELETE")
				.setAuthHeader(authHeader)
				.setResourcePath("observations")
				.setResourceId(observationId)
				.build()
				.execute<void>();
			return response;
		}
	};
}