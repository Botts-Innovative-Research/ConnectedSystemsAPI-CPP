#pragma once

#include <string>

#include "APIRequest.h"
#include "APIResponse.h"
#include "DataModels/Observation.h"

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

		APIResponse<DataModels::Observation> getObservations() {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("/observations")
				.build()
				.execute<DataModels::Observation>();
			return response;
		}

		APIResponse<DataModels::Observation> getObservationsOfDataStream(const std::string& dataStreamId) {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("/datastreams/" + dataStreamId + "/observations")
				.build()
				.execute<DataModels::Observation>();
			return response;
		}

		APIResponse<void> createObservation(const std::string& dataStreamId, const DataModels::Observation& observation) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, observation);
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("POST")
				.setAuthHeader(authHeader)
				.addHeader("Content-Type", "application/json")
				.setResourcePath("/datastreams/" + dataStreamId + "/observations")
				.setBody(j.dump())
				.build()
				.execute<void>();
			return response;
		}
	};
}