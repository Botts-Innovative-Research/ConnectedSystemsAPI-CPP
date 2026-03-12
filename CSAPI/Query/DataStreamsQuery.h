#pragma once

#include <string>
#include "QueryParameters.h"
#include "DataModels/TimeExtent.h"

namespace ConnectedSystemsAPI::Query {
	class DataStreamsQuery : public QueryParameters {
	public:
		/// <summary>
		/// List of resource local IDs or unique IDs (URI).
		/// Only resources that have one of the provided identifiers are selected.
		/// </summary>
		DataStreamsQuery& setId(const std::string& id) {
			addParameter("id", id);
			return *this;
		}

		/// <summary>
		/// List of keywords used for full-text search.
		/// Only resources that have textual fields that contain one of the specified keywords are selected.
		/// The resource name and description properties are always searched.
		/// It is up to the server to decide which other textual fields are searched.
		/// Examples:
		/// - q=temp
		/// - q=gps,imu
		/// </summary>
		DataStreamsQuery& setQ(const std::string& q) {
			addParameter("q", q);
			return *this;
		}

		/// <summary>
		/// Either a date-time or an interval.
		/// Date and time expressions adhere to RFC 3339.
		/// Intervals may be bounded or half-bounded (double-dots at start or end).
		/// </summary>
		DataStreamsQuery& setPhenomenonTime(DataModels::TimeExtent phenomenonTime) {
			addParameter("phenomenonTime", phenomenonTime);
			return *this;
		}

		/// <summary>
		/// Either a date-time or an interval.
		/// Date and time expressions adhere to RFC 3339.
		/// Intervals may be bounded or half-bounded (double-dots at start or end).
		/// </summary>
		DataStreamsQuery& setResultTime(DataModels::TimeExtent resultTime) {
			addParameter("resultTime", resultTime);
			return *this;
		}

		/// <summary>
		/// List of system local IDs or unique IDs (URI).
		/// Only resources that are associated with a System that has one of the provided identifiers are selected.
		/// </summary>
		DataStreamsQuery& setSystem(std::vector<std::string>& system) {
			addParameter("system", system);
			return *this;
		}

		/// <summary>
		/// List of feature local IDs or unique IDs (URI).
		/// Only resources that are associated with a feature of interest that has one of the provided identifiers are selected.
		/// </summary>
		DataStreamsQuery& setFoi(const std::string& foi) {
			addParameter("foi", foi);
			return *this;
		}

		/// <summary>
		/// List of property local IDs or unique IDs (URI).
		/// Only resources that are associated with an observable property that has one of the provided identifiers are selected.
		/// </summary>
		DataStreamsQuery& setObservedProperty(const std::string& observedProperty) {
			addParameter("observedProperty", observedProperty);
			return *this;
		}

		/// <summary>
		/// Limits the number of items that are presented in the response document.
		/// Default: 100
		/// </summary>
		DataStreamsQuery& setLimit(int limit) {
			addParameter("limit", limit);
			return *this;
		}
	};
}