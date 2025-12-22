#pragma once

#include <string>

#include "APIRequest.h"
#include "APIResponse.h"
#include "DataModels/Observation.h"
#include "Query/ObservationsQuery.h"
#include "Query/ObservationsOfDataStreamQuery.h"

namespace ConnectedSystemsAPI {
	class ObservationsAPI {
	private:
		std::string apiRoot;
		std::string authHeader;

	public:
		ObservationsAPI() {}
		ObservationsAPI(const std::string& apiRoot, const std::string& authHeader)
			: apiRoot(apiRoot), authHeader(authHeader) {
		}

		/// <summary>
		/// List or search all observations available from this server endpoint.
		/// </summary>
		/// <param name="query">The query string parameters.</param>
		/// <returns>A response object containing a list of observations.</returns>
		APIResponse<DataModels::Observation> getObservations(const Query::ObservationsQuery& query) {
			return getObservations(query.toString());
		}

		/// <summary>
		/// List or search all observations available from this server endpoint.
		/// </summary>
		/// <param name="query">The query string.</param>
		/// <returns>A response object containing a list of observations.</returns>
		APIResponse<DataModels::Observation> getObservations(std::string queryString = "") {
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
		APIResponse<DataModels::Observation> getObservationsOfDataStream(const std::string& dataStreamId, const Query::ObservationsOfDataStreamQuery& query) {
			return getObservationsOfDataStream(dataStreamId, query.toString());
		}

		/// <summary>
		/// List or search all observations available from a datastream.
		/// </summary>
		/// <param name="query">The query string.</param>
		/// <returns>A response object containing a list of observations.</returns>
		APIResponse<DataModels::Observation> getObservationsOfDataStream(const std::string& dataStreamId, std::string queryString = "") {
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
		APIResponse<DataModels::Observation> getObservationById(const std::string& observationId) {
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
		APIResponse<void> createObservation(const std::string& dataStreamId, const DataModels::Observation& observation) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, observation);
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("POST")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("datastreams")
				.setResourceId(dataStreamId)
				.setSubResourcePath("observations")
				.setBody(j.dump())
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
		APIResponse<void> updateObservation(const std::string& observationId, const DataModels::Observation& observation) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, observation);
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("PUT")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("observations")
				.setResourceId(observationId)
				.setBody(j.dump())
				.build()
				.execute<void>();
			return response;
		}

		/// <summary>
		/// Delete an observation by its ID.
		/// </summary>
		/// <param name="observationId">The ID of the observation to delete.</param>
		/// <returns>A response object indicating success or failure.</returns>
		APIResponse<void> deleteObservation(const std::string& observationId) {
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