#pragma once

#include <string>

#include "APIRequest.h"
#include "APIResponse.h"
#include "DataModels/ControlStream.h"
#include "DataModels/CommandSchema.h"

namespace ConnectedSystemsAPI {
	class ControlStreamsAPI {
	private:
		std::string apiRoot;
		std::string authHeader;

	public:
		ControlStreamsAPI() = default;
		ControlStreamsAPI(const std::string& apiRoot, const std::string& authHeader)
			: apiRoot(apiRoot), authHeader(authHeader) {
		}

		APIResponse<DataModels::ControlStream> getControlStreams(std::string queryString = "") {
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

		APIResponse<DataModels::ControlStream> getControlStreamsOfSystem(const std::string& systemId, std::string queryString = "") {
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

		APIResponse<DataModels::ControlStream> getControlStreamById(const std::string& controlStreamId) {
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

		APIResponse<DataModels::CommandSchema> getControlStreamSchema(const std::string& controlStreamId) {
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
	};
}