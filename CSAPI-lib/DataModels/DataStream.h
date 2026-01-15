#pragma once

#include <string>
#include <optional>
#include <vector>
#include <memory>
#include <ostream>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Link.h"
#include "TimeExtent.h"
#include "ObservedProperty.h"
#include "ObservationSchema.h"

namespace ConnectedSystemsAPI::DataModels {
	class DataStream;
	void to_json(nlohmann::ordered_json& j, const DataStream& v);

	class DataStream {
	private:
		std::optional<std::string> id;
		std::optional<std::string> name;
		std::optional<std::string> description;
		std::optional<TimeExtent> validTime;
		std::optional<std::vector<std::string>> formats;
		std::optional<Link> systemLink;
		std::optional<std::string> outputName;
		std::optional<Link> procedureLink;
		std::optional<Link> deploymentLink;
		std::optional<Link> featureOfInterestLink;
		std::optional<Link> samplingFeatureLink;
		std::optional<std::vector<ObservedProperty>> observedProperties;
		std::optional<TimeExtent> phenomenonTime;
		std::optional<std::string> phenomenonTimeInterval;
		std::optional<TimeExtent> resultTime;
		std::optional<std::string> resultTimeInterval;
		std::optional<std::string> dataStreamType;
		std::optional<std::string> resultType;
		std::optional<bool> live;
		std::optional<std::vector<Link>> links;
		std::unique_ptr<ObservationSchema> schema;

	public:
		DataStream() = default;

		DataStream(const std::optional<std::string>& id,
			const std::optional<std::string>& name,
			const std::optional<std::string>& description,
			const std::optional<TimeExtent>& validTime,
			const std::optional<std::vector<std::string>>& formats,
			const std::optional<Link>& systemLink,
			const std::optional<std::string>& outputName,
			const std::optional<Link>& procedureLink,
			const std::optional<Link>& deploymentLink,
			const std::optional<Link>& featureOfInterestLink,
			const std::optional<Link>& samplingFeatureLink,
			const std::optional<std::vector<ObservedProperty>>& observedProperties,
			const std::optional<TimeExtent>& phenomenonTime,
			const std::optional<std::string>& phenomenonTimeInterval,
			const std::optional<TimeExtent>& resultTime,
			const std::optional<std::string>& resultTimeInterval,
			const std::optional<std::string>& dataStreamType,
			const std::optional<std::string>& resultType,
			const std::optional<bool> live,
			const std::optional<std::vector<Link>>& links,
			std::unique_ptr<ObservationSchema> schema
		)
			: id(id), name(name), description(description), validTime(validTime), formats(formats),
			systemLink(systemLink), outputName(outputName), procedureLink(procedureLink),
			deploymentLink(deploymentLink), featureOfInterestLink(featureOfInterestLink),
			samplingFeatureLink(samplingFeatureLink), observedProperties(observedProperties),
			phenomenonTime(phenomenonTime), phenomenonTimeInterval(phenomenonTimeInterval),
			resultTime(resultTime), resultTimeInterval(resultTimeInterval),
			dataStreamType(dataStreamType), resultType(resultType), live(live), links(links),
			schema(std::move(schema))
		{
		}

		DataStream(const DataStream& other)
			: id(other.id), name(other.name), description(other.description),
			validTime(other.validTime), formats(other.formats),
			systemLink(other.systemLink), outputName(other.outputName),
			procedureLink(other.procedureLink), deploymentLink(other.deploymentLink),
			featureOfInterestLink(other.featureOfInterestLink),
			samplingFeatureLink(other.samplingFeatureLink),
			observedProperties(other.observedProperties),
			phenomenonTime(other.phenomenonTime),
			phenomenonTimeInterval(other.phenomenonTimeInterval),
			resultTime(other.resultTime), resultTimeInterval(other.resultTimeInterval),
			dataStreamType(other.dataStreamType), resultType(other.resultType),
			live(other.live), links(other.links)
		{
			if (other.schema) {
				schema = std::make_unique<ObservationSchema>(*other.schema);
			}
		}

		DataStream(DataStream&&) noexcept = default;

		DataStream& operator=(const DataStream& other) {
			if (this != &other) {
				id = other.id;
				name = other.name;
				description = other.description;
				validTime = other.validTime;
				formats = other.formats;
				systemLink = other.systemLink;
				outputName = other.outputName;
				procedureLink = other.procedureLink;
				deploymentLink = other.deploymentLink;
				featureOfInterestLink = other.featureOfInterestLink;
				samplingFeatureLink = other.samplingFeatureLink;
				observedProperties = other.observedProperties;
				phenomenonTime = other.phenomenonTime;
				phenomenonTimeInterval = other.phenomenonTimeInterval;
				resultTime = other.resultTime;
				resultTimeInterval = other.resultTimeInterval;
				dataStreamType = other.dataStreamType;
				resultType = other.resultType;
				live = other.live;
				links = other.links;

				if (other.schema) {
					schema = std::make_unique<ObservationSchema>(*other.schema);
				}
				else {
					schema.reset();
				}
			}
			return *this;
		}

		DataStream& operator=(DataStream&&) noexcept = default;

		/// <summary>
		/// Local resource ID. If set on creation, the server may ignore it.
		/// </summary>
		const std::optional<std::string>& getId() const { return id; }
		/// <summary>
		/// Human-readable name of the resource.
		/// </summary>
		const std::optional<std::string>& getName() const { return name; }
		/// <summary>
		/// Human-readable description of the resource.
		/// </summary>
		const std::optional<std::string>& getDescription() const { return description; }
		/// <summary>
		/// The validity period of the data stream's description.
		/// </summary>
		const std::optional<TimeExtent>& getValidTime() const { return validTime; }
		/// <summary>
		/// The list of formats that the observations in the datastream can be encoded to.
		/// </summary>
		const std::optional<std::vector<std::string>>& getFormats() const { return formats; }
		/// <summary>
		/// Link to the system producing the observations.
		/// </summary>
		const std::optional<Link>& getSystemLink() const { return systemLink; }
		/// <summary>
		/// Name of the system output feeding this datastream.
		/// </summary>
		const std::optional<std::string>& getOutputName() const { return outputName; }
		/// <summary>Link to the procedure used to acquire observations
		/// (only provided if all observations in the datastream share the same procedure).
		/// </summary>
		const std::optional<Link>& getProcedureLink() const { return procedureLink; }
		/// <summary>
		/// Link to the deployment during which the observations are/were collected
		/// (only provided if all observations in the datastream share the same deployment).
		/// </summary>
		const std::optional<Link>& getDeploymentLink() const { return deploymentLink; }
		/// <summary>
		/// Link to the ultimate feature of interest
		/// (only provided if all observations in the datastream share the same feature of interest).
		/// </summary>
		const std::optional<Link>& getFeatureOfInterestLink() const { return featureOfInterestLink; }
		/// <summary>
		/// Link to the sampling feature
		/// (only provided if all observations in the datastream share the same sampling feature).
		/// </summary>
		const std::optional<Link>& getSamplingFeatureLink() const { return samplingFeatureLink; }
		/// <summary>
		/// Properties for which the observations in the datastream provide measurements.
		/// </summary>
		const std::optional<std::vector<ObservedProperty>>& getObservedProperties() const { return observedProperties; }
		/// <summary>
		/// The time period spanned by the phenomenon times of all observations in the datastream.
		/// </summary>
		const std::optional<TimeExtent>& getPhenomenonTime() const { return phenomenonTime; }
		/// <summary>
		/// An indication of how often feature of interest properties are observed.
		/// </summary>
		const std::optional<std::string>& getPhenomenonTimeInterval() const { return phenomenonTimeInterval; }
		/// <summary>
		/// The time period spanned by the result times of all observations in the datastream.
		/// </summary>
		const std::optional<TimeExtent>& getResultTime() const { return resultTime; }
		/// <summary>
		/// An indication of how often observation results are produced.
		/// </summary>
		const std::optional<std::string>& getResultTimeInterval() const { return resultTimeInterval; }
		/// <summary>
		/// Type of the data stream.
		/// </summary>
		const std::optional<std::string>& getDataStreamType() const { return dataStreamType; }
		/// <summary>
		/// The type of result for observations in the datastream.
		/// </summary>
		const std::optional<std::string>& getResultType() const { return resultType; }
		/// <summary>
		/// Indicates whether live data is available from the datastream.
		/// </summary>
		const std::optional<bool>& isLive() const { return live; }
		/// <summary>
		/// List of links associated with the data stream.
		/// </summary>
		const std::optional<std::vector<Link>>& getLinks() const { return links; }
		/// <summary>
		/// The observation schema for the data stream, if available.
		/// </summary>
		const ObservationSchema* getSchema() const { return schema.get(); }

		friend void from_json(const nlohmann::json& j, DataStream& ds);
		friend void to_json(nlohmann::ordered_json& j, const DataStream& ds);

		friend std::ostream& operator<<(std::ostream& os, const DataStream& ds) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, ds);
			os << j.dump(2);
			return os;
		}
	};

	inline void from_json(const nlohmann::json& j, DataStream& ds) {
		ds.id = j.value("id", std::optional<std::string>{});
		ds.name = j.value("name", std::optional<std::string>{});
		ds.description = j.value("description", std::optional<std::string>{});
		ds.validTime = j.value("validTime", std::optional<TimeExtent>{});
		ds.formats = j.value("formats", std::optional<std::vector<std::string>>{});
		ds.systemLink = j.value("system@link", std::optional<Link>{});
		ds.outputName = j.value("outputName", std::optional<std::string>{});
		ds.procedureLink = j.value("procedure@link", std::optional<Link>{});
		ds.deploymentLink = j.value("deployment@link", std::optional<Link>{});
		ds.featureOfInterestLink = j.value("featureOfInterest@link", std::optional<Link>{});
		ds.samplingFeatureLink = j.value("samplingFeature@link", std::optional<Link>{});
		ds.observedProperties = j.value("observedProperties", std::optional<std::vector<ObservedProperty>>{});
		ds.phenomenonTime = j.value("phenomenonTime", std::optional<TimeExtent>{});
		ds.phenomenonTimeInterval = j.value("phenomenonTimeInterval", std::optional<std::string>{});
		ds.resultTime = j.value("resultTime", std::optional<TimeExtent>{});
		ds.resultTimeInterval = j.value("resultTimeInterval", std::optional<std::string>{});
		ds.dataStreamType = j.value("type", std::optional<std::string>{});
		ds.resultType = j.value("resultType", std::optional<std::string>{});
		ds.live = j.value("live", std::optional<bool>{});
		ds.links = j.value("links", std::optional<std::vector<Link>>{});

		if (j.contains("schema") && !j["schema"].is_null()) {
			ds.schema = std::make_unique<ObservationSchema>(j["schema"].get<ObservationSchema>());
		}
	}

	inline void to_json(nlohmann::ordered_json& j, const DataStream& ds) {
		j = nlohmann::ordered_json::object();

		if (ds.id.has_value()) j["id"] = ds.id.value();
		if (ds.name.has_value()) j["name"] = ds.name.value();
		if (ds.description.has_value()) j["description"] = ds.description.value();
		if (ds.validTime.has_value()) j["validTime"] = ds.validTime.value();
		if (ds.formats.has_value()) j["formats"] = ds.formats.value();
		if (ds.systemLink.has_value()) j["system@link"] = ds.systemLink.value();
		if (ds.outputName.has_value()) j["outputName"] = ds.outputName.value();
		if (ds.procedureLink.has_value()) j["procedure@link"] = ds.procedureLink.value();
		if (ds.deploymentLink.has_value()) j["deployment@link"] = ds.deploymentLink.value();
		if (ds.featureOfInterestLink.has_value()) j["featureOfInterest@link"] = ds.featureOfInterestLink.value();
		if (ds.samplingFeatureLink.has_value()) j["samplingFeature@link"] = ds.samplingFeatureLink.value();
		if (ds.observedProperties.has_value()) j["observedProperties"] = ds.observedProperties.value();
		if (ds.phenomenonTime.has_value()) j["phenomenonTime"] = ds.phenomenonTime.value();
		if (ds.phenomenonTimeInterval.has_value()) j["phenomenonTimeInterval"] = ds.phenomenonTimeInterval.value();
		if (ds.resultTime.has_value()) j["resultTime"] = ds.resultTime.value();
		if (ds.resultTimeInterval.has_value()) j["resultTimeInterval"] = ds.resultTimeInterval.value();
		if (ds.dataStreamType.has_value()) j["type"] = ds.dataStreamType.value();
		if (ds.resultType.has_value()) j["resultType"] = ds.resultType.value();
		if (ds.live.has_value()) j["live"] = ds.live.value();
		if (ds.links.has_value()) j["links"] = ds.links.value();
		if (ds.schema) j["schema"] = *ds.schema;
	}
}