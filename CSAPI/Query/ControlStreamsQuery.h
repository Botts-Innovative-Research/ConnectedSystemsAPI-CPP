#pragma once

#include <string>
#include "QueryParameters.h"
#include "../DataModels/TimeExtent.h"

namespace ConnectedSystemsAPI::Query {
	class ControlStreamsQuery : public QueryParameters {
	public:
		/// <summary>
		/// List of keywords used for full-text search.
		/// Only resources that have textual fields that contain one of the specified keywords are selected.
		/// The resource name and description properties are always searched.
		/// It is up to the server to decide which other textual fields are searched.
		/// Examples:
		/// - q=temp
		/// - q=gps,imu
		/// </summary>
		ControlStreamsQuery& setQ(const std::string& q) {
			addParameter("q", q);
			return *this;
		}

		/// <summary>
		/// Either a date-time or an interval.
		/// Date and time expressions adhere to RFC 3339.
		/// Intervals may be bounded or half-bounded (double-dots at start or end).
		/// </summary>
		ControlStreamsQuery& setIssueTime(DataModels::TimeExtent const& issueTime) {
			addParameter("issueTime", issueTime);
			return *this;
		}

		/// <summary>
		/// Either a date-time or an interval.
		/// Date and time expressions adhere to RFC 3339.
		/// Intervals may be bounded or half-bounded (double-dots at start or end).
		/// </summary>
		ControlStreamsQuery& setExecutionTime(DataModels::TimeExtent const& executionTime) {
			addParameter("executionTime", executionTime);
			return *this;
		}

		/// <summary>
		/// List of system local IDs or unique IDs (URI).
		/// Only resources that are associated with a System that has one of the provided identifiers are selected.
		/// </summary>
		ControlStreamsQuery& setSystem(std::vector<std::string>& system) {
			addParameter("system", system);
			return *this;
		}

		/// <summary>
		/// List of feature local IDs or unique IDs (URI).
		/// Only resources that are associated with a feature of interest that has one of the provided identifiers are selected.
		/// </summary>
		ControlStreamsQuery& setFoi(const std::string& foi) {
			addParameter("foi", foi);
			return *this;
		}

		/// <summary>
		/// List of property local IDs or unique IDs (URI).
		/// Only resources that are associated with an observable property that has one of the provided identifiers are selected.
		/// </summary>
		ControlStreamsQuery& setControlledProperty(const std::string& controlledProperty) {
			addParameter("controlledProperty", controlledProperty);
			return *this;
		}


		/// <summary>
		/// Limits the number of items that are presented in the response document.
		/// Default: 100
		/// </summary>
		ControlStreamsQuery& setLimit(int limit) {
			addParameter("limit", limit);
			return *this;
		}
	};
}