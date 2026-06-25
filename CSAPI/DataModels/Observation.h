#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Link.h"
#include "TimeInstant.h"
#include "Data/DataBlockMixed.h"

namespace ConnectedSystemsAPI::DataModels {
	class Observation;
	void to_json(nlohmann::ordered_json& j, const Observation& v);

	class Observation {
	private:
		std::optional<std::string> id;
		std::optional<std::string> dataStreamId;
		std::optional<std::string> samplingFeatureId;
		std::optional<Link> procedureLink;
		std::optional<TimeInstant> phenomenonTime;
		std::optional<TimeInstant> resultTime;
		// todo: parameters
		Data::DataBlockMixed result;
		std::optional<Link> resultLink;
		std::optional<std::vector<Link>> links;

	public:
		Observation() = default;
		Observation(const std::optional<std::string>& id,
			const std::optional<std::string>& dataStreamId,
			const std::optional<std::string>& samplingFeatureId,
			const std::optional<Link>& procedureLink,
			const std::optional<TimeInstant>& phenomenonTime,
			const std::optional<TimeInstant>& resultTime,
			const Data::DataBlockMixed& result,
			const std::optional<Link>& resultLink,
			const std::optional<std::vector<Link>>& links)
			: id(id), dataStreamId(dataStreamId), samplingFeatureId(samplingFeatureId),
			procedureLink(procedureLink), phenomenonTime(phenomenonTime), resultTime(resultTime),
			result(result), resultLink(resultLink), links(links) {
		}

		nlohmann::ordered_json toJson() const {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// Local ID of the observation.
		/// </summary>
		const std::optional<std::string>& getId() const { return id; }
		/// <summary>
		/// Local ID of the datastream that the observation is part of.
		/// </summary>
		const std::optional<std::string>& getDataStreamId() const { return dataStreamId; }
		/// <summary>
		/// Local ID of the sampling feature that is the target of the observation.
		/// </summary>
		const std::optional<std::string>& getSamplingFeatureId() const { return samplingFeatureId; }
		/// <summary>
		/// Link to the procedure/method used to make the observation.
		/// </summary>
		const std::optional<Link>& getProcedureLink() const { return procedureLink; }
		/// <summary>
		/// Time at which the observation result is a valid estimate of the sampling feature property(ies).
		/// Defaults to the same value as resultTime.
		/// </summary>
		const std::optional<TimeInstant>& getPhenomenonTime() const { return phenomenonTime; }
		/// <summary>
		/// Time at which the observation result was generated.
		/// </summary>
		const std::optional<TimeInstant>& getResultTime() const { return resultTime; }
		/// <summary>
		/// Result of the observation.
		/// Must be valid according to the result schema provided in the datastream metadata.
		/// </summary>
		const Data::DataBlockMixed& getResult() const { return result; }
		/// <summary>
		/// Link to external result data (e.g., large raster dataset served by a tiling service)
		/// </summary>
		const std::optional<Link>& getResultLink() const { return resultLink; }
		/// <summary>
		/// Links to related resources.
		/// </summary>
		const std::optional<std::vector<Link>>& getLinks() const { return links; }

		friend void from_json(const nlohmann::ordered_json& j, Observation& o);
		friend void to_json(nlohmann::ordered_json& j, const Observation& o);

		friend std::ostream& operator<<(std::ostream& os, const Observation& o) {
			nlohmann::ordered_json j;
			to_json(j, o);
			os << j.dump(2);
			return os;
		}
	};

	inline void from_json(const nlohmann::ordered_json& j, Observation& o) {
		if (j.contains("id") && !j["id"].is_null())
			o.id = j["id"].get<std::string>();
		else
			o.id.reset();

		if (j.contains("datastream@id") && !j["datastream@id"].is_null())
			o.dataStreamId = j["datastream@id"].get<std::string>();
		else
			o.dataStreamId.reset();

		if (j.contains("samplingFeature@id") && !j["samplingFeature@id"].is_null())
			o.samplingFeatureId = j["samplingFeature@id"].get<std::string>();
		else
			o.samplingFeatureId.reset();

		if (j.contains("procedure@link") && !j["procedure@link"].is_null())
			o.procedureLink = j["procedure@link"].get<Link>();
		else
			o.procedureLink.reset();

		if (j.contains("phenomenonTime") && !j["phenomenonTime"].is_null())
			o.phenomenonTime = j["phenomenonTime"].get<TimeInstant>();
		else
			o.phenomenonTime.reset();

		if (j.contains("resultTime") && !j["resultTime"].is_null())
			o.resultTime = j["resultTime"].get<TimeInstant>();
		else
			o.resultTime.reset();

		if (j.contains("result@link") && !j["result@link"].is_null())
			o.resultLink = j["result@link"].get<Link>();
		else
			o.resultLink.reset();

		if (j.contains("links") && !j["links"].is_null())
			o.links = j["links"].get<std::vector<Link>>();
		else
			o.links.reset();

		if (j.contains("result") && !j["result"].is_null()) {
			o.result = j["result"].get<Data::DataBlockMixed>();
		}
	}

	inline void to_json(nlohmann::ordered_json& j, const Observation& o) {
		j = nlohmann::ordered_json::object();

		if (o.id) j["id"] = o.id.value();
		if (o.dataStreamId) j["datastream@id"] = o.dataStreamId.value();
		if (o.samplingFeatureId) j["samplingFeature@id"] = o.samplingFeatureId.value();
		if (o.procedureLink) j["procedure@link"] = o.procedureLink.value();
		if (o.phenomenonTime) j["phenomenonTime"] = o.phenomenonTime.value();
		if (o.resultTime) j["resultTime"] = o.resultTime.value();
		j["result"] = o.result;
		if (o.resultLink) j["result@link"] = o.resultLink.value();
		if (o.links) j["links"] = o.links.value();
	}
}