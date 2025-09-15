#pragma once

#include <string>
#include "APIRequest.h"
#include "APIResponse.h"
#include "System.h"

namespace ConnectedSystemsAPI {
	class SystemsAPI {
	private:
		std::string apiRoot;
		std::string authHeader;

	public:
		SystemsAPI() {}
		SystemsAPI(const std::string& apiRoot, const std::string& authHeader)
			: apiRoot(apiRoot), authHeader(authHeader) {
		}

		APIResponse <DataModels::System> getSystems() {
			auto response = APIRequest::Builder()
				.setApiRoot(apiRoot)
				.setMethod("GET")
				.setAuthHeader(authHeader)
				.setResourcePath("/systems")
				.build()
				.execute<DataModels::System>();

			return response;
		}
	};
}