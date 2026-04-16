#pragma once

#include <string>
#include "QueryParameters.h"
#include "../DataModels/TimeExtent.h"

namespace ConnectedSystemsAPI::Query {
	class CommandsQuery : public QueryParameters {
	public:
		/// <summary>
		/// List of resource local IDs or unique IDs (URI).
		/// Only resources that have one of the provided identifiers are selected.
		/// </summary>
		CommandsQuery& setId(const std::string& id) {
			addParameter("id", id);
			return *this;
		}

		/// <summary>
		/// Either a date-time or an interval.
		/// Date and time expressions adhere to RFC 3339.
		/// Intervals may be bounded or half-bounded (double-dots at start or end).
		/// </summary>
		CommandsQuery& setIssueTime(DataModels::TimeExtent issueTime) {
			addParameter("issueTime", issueTime);
			return *this;
		}

		/// <summary>
		/// Either a date-time or an interval.
		/// Date and time expressions adhere to RFC 3339.
		/// Intervals may be bounded or half-bounded (double-dots at start or end).
		/// </summary>
		CommandsQuery& setExecutionTime(DataModels::TimeExtent executionTime) {
			addParameter("executionTime", executionTime);
			return *this;
		}

		/// <summary>
		/// List of command status codes.
		/// Only command resources whose current status matches one of the provided status codes are selected.
		/// Items Enum: "PENDING" "ACCEPTED" "REJECTED" "SCHEDULED" "UPDATED" "CANCELED" "EXECUTING" "FAILED" "COMPLETED"
		/// </summary>
		CommandsQuery& setStatusCode(const std::string& statusCode) {
			addParameter("statusCode", statusCode);
			return *this;
		}

		/// <summary>
		/// List of sender IDs.
		/// Only command resources issued by one of the specified senders are selected.
		/// </summary>
		CommandsQuery& setSender(const std::string& sender) {
			addParameter("sender", sender);
			return *this;
		}

		/// <summary>
		/// List of control stream local IDs or unique IDs (URI).
		/// Only resources that are associated to a control stream that has one of the provided identifiers are selected.
		/// </summary>
		CommandsQuery& setControlStream(std::vector<std::string>& controlStream) {
			addParameter("controlStream", controlStream);
			return *this;
		}

		/// <summary>
		/// List of system local IDs or unique IDs (URI).
		/// Only resources that are associated with a System that has one of the provided identifiers are selected.
		/// </summary>
		CommandsQuery& setSystem(std::vector<std::string>& system) {
			addParameter("system", system);
			return *this;
		}

		/// <summary>
		/// List of feature local IDs or unique IDs (URI).
		/// Only resources that are associated with a feature of interest that has one of the provided identifiers are selected.
		/// </summary>
		CommandsQuery& setFoi(const std::string& foi) {
			addParameter("foi", foi);
			return *this;
		}

		/// <summary>
		/// List of property local IDs or unique IDs (URI).
		/// Only resources that are associated with an observable property that has one of the provided identifiers are selected.
		/// </summary>
		CommandsQuery& setControlledProperty(const std::string& controlledProperty) {
			addParameter("controlledProperty", controlledProperty);
			return *this;
		}

		/// <summary>
		/// Limits the number of items that are presented in the response document.
		/// Default: 100
		/// </summary>
		CommandsQuery& setLimit(int limit) {
			addParameter("limit", limit);
			return *this;
		}
	};
}