#pragma once

#include <string>
#include "QueryParameters.h"
#include "..\DataModels\TimeExtent.h"

namespace ConnectedSystemsAPI {
	namespace Query {
		class ObservationsOfDataStreamQuery : public QueryParameters {
		public:
			/// <summary>
			/// List of resource local IDs or unique IDs (URI).
			/// Only resources that have one of the provided identifiers are selected.
			/// </summary>
			ObservationsOfDataStreamQuery& setId(const std::string& id) {
				addParameter("id", id);
				return *this;
			}

			/// <summary>
			/// Either a date-time or an interval.
			/// Date and time expressions adhere to RFC 3339.
			/// Intervals may be bounded or half-bounded (double-dots at start or end).
			/// </summary>
			ObservationsOfDataStreamQuery& setPhenomenonTime(DataModels::TimeExtent phenomenonTime) {
				addParameter("phenomenonTime", phenomenonTime);
				return *this;
			}

			/// <summary>
			/// Either a date-time or an interval.
			/// Date and time expressions adhere to RFC 3339.
			/// Intervals may be bounded or half-bounded (double-dots at start or end).
			/// </summary>
			ObservationsOfDataStreamQuery& setResultTime(DataModels::TimeExtent resultTime) {
				addParameter("resultTime", resultTime);
				return *this;
			}

			/// <summary>
			/// List of feature local IDs or unique IDs (URI).
			/// Only resources that are associated with a feature of interest that has one of the provided identifiers are selected.
			/// </summary>
			ObservationsOfDataStreamQuery& setFoi(const std::string& foi) {
				addParameter("foi", foi);
				return *this;
			}

			/// <summary>
			/// List of property local IDs or unique IDs (URI).
			/// Only resources that are associated with an observable property that has one of the provided identifiers are selected.
			/// </summary>
			ObservationsOfDataStreamQuery& setObservedProperty(const std::string& observedProperty) {
				addParameter("observedProperty", observedProperty);
				return *this;
			}

			/// <summary>
			/// Limits the number of items that are presented in the response document.
			/// Default: 100
			/// </summary>
			ObservationsOfDataStreamQuery& setLimit(int limit) {
				addParameter("limit", limit);
				return *this;
			}
		};
	}
}