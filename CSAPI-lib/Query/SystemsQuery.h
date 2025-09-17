#pragma once

#include <string>
#include "QueryParameters.h"
#include "..\DataModels\TimeExtent.h"

namespace ConnectedSystemsAPI {
	namespace Query {
		class SystemsQuery : public QueryParameters {
		public:
			/// <summary>
			/// List of resource local IDs or unique IDs (URI).
			/// Only resources that have one of the provided identifiers are selected.
			/// </summary>
			SystemsQuery& setId(const std::string& id) {
				addParameter("id", id);
				return *this;
			}

			/// <summary>
			/// The bounding box is provided as four or six numbers,
			/// depending on whether the coordinate reference system includes a vertical axis (height or depth).
			/// </summary>
			/// <param name="bbox"></param>
			/// <returns></returns>
			SystemsQuery& setBbox(const std::string& bbox) {
				addParameter("bbox", bbox);
				return *this;
			}

			/// <summary>
			/// Only features that have a validTime property that intersects the value of datetime are selected.
			/// If history is supported for a feature type, the following also applies:
			/// - If datetime is a time instant or now, only the description valid at the specified time is selected.
			/// - If datetime is a time period, only the latest description valid during the period is selected.
			/// - The response can never include more than one description of the same feature.
			/// </summary>
			SystemsQuery& setDatetime(DataModels::TimeExtent datetime) {
				addParameter("datetime", datetime);
				return *this;
			}

			/// <summary>
			/// WKT geometry and operator to filter resources on their location or geometry.
			/// Only features that have a geometry that intersects the value of geom are selected.
			/// Examples:
			/// - geom=LINESTRING((-86.53 12.45), (-86.54 12.46), (-86.55 12.47))
			/// - geom=POLYGON((0 0,4 0,4 4,0 4,0 0))
			/// </summary>
			SystemsQuery& setGeom(const std::string& geom) {
				addParameter("geom", geom);
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
			SystemsQuery& setQ(const std::string& q) {
				addParameter("q", q);
				return *this;
			}

			/// <summary>
			/// List of resource local IDs or unique IDs (URI).
			/// Only resources that have a parent that has one of the provided identifiers are selected.
			/// </summary>
			SystemsQuery& setParent(const std::string& parent) {
				addParameter("parent", parent);
				return *this;
			}

			/// <summary>
			/// List of procedure local IDs or unique IDs (URI).
			/// Only systems that implement a procedure that has one of the provided identifiers are selected.
			/// </summary>
			/// <param name="procedure"></param>
			/// <returns></returns>
			SystemsQuery& setProcedure(const std::string& procedure) {
				addParameter("procedure", procedure);
				return *this;
			}

			/// <summary>
			/// List of feature local IDs or unique IDs (URI).
			/// Only resources that are associated with a feature of interest that has one of the provided identifiers are selected.
			/// </summary>
			SystemsQuery& setFoi(const std::string& foi) {
				addParameter("foi", foi);
				return *this;
			}

			/// <summary>
			/// List of property local IDs or unique IDs (URI).
			/// Only resources that are associated with an observable property that has one of the provided identifiers are selected.
			/// </summary>
			SystemsQuery& setObservedProperty(const std::string& observedProperty) {
				addParameter("observedProperty", observedProperty);
				return *this;
			}

			/// <summary>
			/// List of property local IDs or unique IDs (URI).
			/// Only resources that are associated with a controllable property that has one of the provided identifiers are selected.
			/// </summary>
			SystemsQuery& setControlledProperty(const std::string& controlledProperty) {
				addParameter("controlledProperty", controlledProperty);
				return *this;
			}

			/// <summary>
			/// If true, instructs the server to include subsystems in the search results.
			/// Default: false
			/// </summary>
			SystemsQuery& setRecursive(bool recursive) {
				addParameter("recursive", recursive);
				return *this;
			}

			/// <summary>
			/// Limits the number of items that are presented in the response document.
			/// Default: 100
			/// </summary>
			SystemsQuery& setLimit(int limit) {
				addParameter("limit", limit);
				return *this;
			}
		};
	}
}
