#pragma once

#include <string>
#include "APIRequest.h"
#include "APIResponse.h"
#include "DataModels/DataStream.h"

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
		/// <returns>A response object containing a list of data streams.</returns>
		APIResponse<DataModels::DataStream> getSystems() {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("/datastreams")
				.build()
				.execute<DataModels::DataStream>();
			return response;
		}
	};
}