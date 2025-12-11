#pragma once

#include <string>
#include "QueryParameters.h"
#include "..\DataModels\TimeExtent.h"

namespace ConnectedSystemsAPI {
	namespace Query {
		class ObservationsQuery : public QueryParameters {
		public:
			/// <summary>
			/// List of resource local IDs or unique IDs (URI).
			/// Only resources that have one of the provided identifiers are selected.
			/// </summary>
			ObservationsQuery& setId(const std::string& id) {
				addParameter("id", id);
				return *this;
			}

			/// <summary>
			/// Either a date-time or an interval.
			/// Date and time expressions adhere to RFC 3339.
			/// Intervals may be bounded or half-bounded (double-dots at start or end).
			/// </summary>
			ObservationsQuery& setPhenomenonTime(DataModels::TimeExtent phenomenonTime) {
				addParameter("phenomenonTime", phenomenonTime);
				return *this;
			}

			/// <summary>
			/// Either a date-time or an interval.
			/// Date and time expressions adhere to RFC 3339.
			/// Intervals may be bounded or half-bounded (double-dots at start or end).
			/// </summary>
			ObservationsQuery& setResultTime(DataModels::TimeExtent resultTime) {
				addParameter("resultTime", resultTime);
				return *this;
			}

			/// <summary>
			/// List of system local IDs or unique IDs (URI).
			/// Only resources that are associated with a data stream that has one of the provided identifiers are selected.
			/// </summary>
			ObservationsQuery& setDataStream(std::vector<std::string>& dataStream) {
				addParameter("dataStream", dataStream);
				return *this;
			}

			/// <summary>
			/// List of system local IDs or unique IDs (URI).
			/// Only resources that are associated with a System that has one of the provided identifiers are selected.
			/// </summary>
			ObservationsQuery& setSystem(std::vector<std::string>& system) {
				addParameter("system", system);
				return *this;
			}

			/// <summary>
			/// List of feature local IDs or unique IDs (URI).
			/// Only resources that are associated with a feature of interest that has one of the provided identifiers are selected.
			/// </summary>
			ObservationsQuery& setFoi(const std::string& foi) {
				addParameter("foi", foi);
				return *this;
			}

			/// <summary>
			/// List of property local IDs or unique IDs (URI).
			/// Only resources that are associated with an observable property that has one of the provided identifiers are selected.
			/// </summary>
			ObservationsQuery& setObservedProperty(const std::string& observedProperty) {
				addParameter("observedProperty", observedProperty);
				return *this;
			}

			/// <summary>
			/// Limits the number of items that are presented in the response document.
			/// Default: 100
			/// </summary>
			ObservationsQuery& setLimit(int limit) {
				addParameter("limit", limit);
				return *this;
			}
		};
	}
}