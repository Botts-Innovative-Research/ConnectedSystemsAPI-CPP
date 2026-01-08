#pragma once

#include <string>
#include "QueryParameters.h"
#include "DataModels/TimeExtent.h"

namespace ConnectedSystemsAPI::Query {
	class DataStreamsOfSystemQuery : public QueryParameters {
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
		DataStreamsOfSystemQuery& setQ(const std::string& q) {
			addParameter("q", q);
			return *this;
		}

		/// <summary>
		/// Either a date-time or an interval.
		/// Date and time expressions adhere to RFC 3339.
		/// Intervals may be bounded or half-bounded (double-dots at start or end).
		/// </summary>
		DataStreamsOfSystemQuery& setPhenomenonTime(DataModels::TimeExtent phenomenonTime) {
			addParameter("phenomenonTime", phenomenonTime);
			return *this;
		}

		/// <summary>
		/// Either a date-time or an interval.
		/// Date and time expressions adhere to RFC 3339.
		/// Intervals may be bounded or half-bounded (double-dots at start or end).
		/// </summary>
		DataStreamsOfSystemQuery& setResultTime(DataModels::TimeExtent resultTime) {
			addParameter("resultTime", resultTime);
			return *this;
		}

		/// <summary>
		/// Limits the number of items that are presented in the response document.
		/// Default: 100
		/// </summary>
		DataStreamsOfSystemQuery& setLimit(int limit) {
			addParameter("limit", limit);
			return *this;
		}
	};
}