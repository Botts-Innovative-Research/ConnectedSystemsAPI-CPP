#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "Link.h"
#include "TimeExtent.h"
#include "ObservedProperty.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class DataStream {
		private:
			std::string id;
			std::string name;
			std::string description;
			std::optional<TimeExtent> validTime;
			std::optional<std::vector<std::string>> formats;
			std::optional<Link> systemLink;
			std::string outputName;
			std::optional<Link> procedureLink;
			std::optional<Link> deploymentLink;
			std::optional<Link> featureOfInterestLink;
			std::optional<Link> samplingFeatureLink;
			std::optional<std::vector<ObservedProperty>> observedProperties;
			std::optional<TimeExtent> phenomenonTime;
			std::string phenomenonTimeInterval;
			std::optional<TimeExtent> resultTime;
			std::string resultTimeInterval;
			std::string dataStreamType;
			std::string resultType;
			std::optional<bool> live;
			std::optional<std::vector<Link>> links;
			//std::optional<ObservationSchema> schema;

		public:
			DataStream() = default;
			DataStream(const std::string& id,
				const std::string& name,
				const std::string& description,
				const std::optional<TimeExtent>& validTime,
				const std::optional<std::vector<std::string>>& formats,
				const std::optional<Link>& systemLink,
				const std::string& outputName,
				const std::optional<Link>& procedureLink,
				const std::optional<Link>& deploymentLink,
				const std::optional<Link>& featureOfInterestLink,
				const std::optional<Link>& samplingFeatureLink,
				const std::optional<std::vector<ObservedProperty>>& observedProperties,
				const std::optional<TimeExtent>& phenomenonTime,
				const std::string& phenomenonTimeInterval,
				const std::optional<TimeExtent>& resultTime,
				const std::string& resultTimeInterval,
				const std::string& dataStreamType,
				const std::string& resultType,
				const std::optional<bool> live,
				const std::optional<std::vector<Link>>& links)
				: id(id), name(name), description(description), validTime(validTime), formats(formats),
				systemLink(systemLink), outputName(outputName), procedureLink(procedureLink),
				deploymentLink(deploymentLink), featureOfInterestLink(featureOfInterestLink),
				samplingFeatureLink(samplingFeatureLink), observedProperties(observedProperties),
				phenomenonTime(phenomenonTime), phenomenonTimeInterval(phenomenonTimeInterval),
				resultTime(resultTime), resultTimeInterval(resultTimeInterval),
				dataStreamType(dataStreamType), resultType(resultType), live(live), links(links) {
			}

			/// <returns>Local resource ID. If set on creation, the server may ignore it.</returns>
			const std::string& getId() const { return id; }
			/// <returns>Human-readable name of the resource.</returns>
			const std::string& getName() const { return name; }
			/// <returns>Human-readable description of the resource.</returns>
			const std::string& getDescription() const { return description; }
			/// <returns>The validity period of the data stream’s description.</returns>
			const std::optional<TimeExtent>& getValidTime() const { return validTime; }
			/// <returns>The list of formats that the observations in the datastream can be encoded to.</returns>
			const std::optional<std::vector<std::string>>& getFormats() const { return formats; }
			/// <returns>Link to the system producing the observations.</returns>
			const std::optional<Link>& getSystemLink() const { return systemLink; }
			/// <returns>Name of the system output feeding this datastream.</returns>
			const std::string& getOutputName() const { return outputName; }
			/// <returns>Link to the procedure used to acquire observations
			/// (only provided if all observations in the datastream share the same procedure).</returns>
			const std::optional<Link>& getProcedureLink() const { return procedureLink; }
			/// <returns>Link to the deployment during which the observations are/were collected
			/// (only provided if all observations in the datastream share the same deployment).</returns>
			const std::optional<Link>& getDeploymentLink() const { return deploymentLink; }
			/// <returns>Link to the ultimate feature of interest
			/// (only provided if all observations in the datastream share the same feature of interest).</returns>
			const std::optional<Link>& getFeatureOfInterestLink() const { return featureOfInterestLink; }
			/// <returns>Link to the sampling feature
			/// (only provided if all observations in the datastream share the same sampling feature).</returns>
			const std::optional<Link>& getSamplingFeatureLink() const { return samplingFeatureLink; }
			/// <returns>Properties for which the observations in the datastream provide measurements.</returns>
			const std::optional<std::vector<ObservedProperty>>& getObservedProperties() const { return observedProperties; }
			/// <returns>The time period spanned by the phenomenon times of all observations in the datastream.</returns>
			const std::optional<TimeExtent>& getPhenomenonTime() const { return phenomenonTime; }
			/// <returns>An indication of how often feature of interest properties are observed.</returns>
			const std::string& getPhenomenonTimeInterval() const { return phenomenonTimeInterval; }
			/// <returns>The time period spanned by the result times of all observations in the datastream.</returns>
			const std::optional<TimeExtent>& getResultTime() const { return resultTime; }
			/// <returns>An indication of how often observation results are produced.</returns>
			const std::string& getResultTimeInterval() const { return resultTimeInterval; }
			/// <returns>Type of the data stream.</returns>
			const std::string& getDataStreamType() const { return dataStreamType; }
			/// <returns>The type of result for observations in the datastream.</returns>
			const std::string& getResultType() const { return resultType; }
			/// <returns>Indicates whether live data is available from the datastream.</returns>
			const std::optional<bool>& isLive() const { return live; }
			/// <returns>List of links associated with the data stream.</returns>
			const std::optional<std::vector<Link>>& getLinks() const { return links; }
		};

		inline void from_json(const nlohmann::json& j, DataStream& ds) {
			ds = DataStream(
				j.value("id", ""),
				j.at("name").get<std::string>(),
				j.value("description", ""),
				j.value("validTime", std::optional<TimeExtent>{}),
				j.value("formats", std::optional<std::vector<std::string>>{}),
				j.value("system@link", std::optional<Link>{}),
				j.value("outputName", ""),
				j.value("procedure@link", std::optional<Link>{}),
				j.value("deployment@link", std::optional<Link>{}),
				j.value("featureOfInterest@link", std::optional<Link>{}),
				j.value("samplingFeature@link", std::optional<Link>{}),
				j.value("observedProperties", std::optional<std::vector<ObservedProperty>>{}),
				j.value("phenomenonTime", std::optional<TimeExtent>{}),
				j.value("phenomenonTimeInterval", ""),
				j.value("resultTime", std::optional<TimeExtent>{}),
				j.value("resultTimeInterval", ""),
				j.value("type", ""),
				j.value("resultType", ""),
				j.value("live", std::optional<bool>{}),
				j.value("links", std::optional<std::vector<Link>>{})
			);
		}

		inline void to_json(nlohmann::ordered_json& j, const DataStream& ds) {
			j = nlohmann::ordered_json::object();
			if (!ds.getId().empty())
				j["id"] = ds.getId();
			j["name"] = ds.getName();
			if (!ds.getDescription().empty())
				j["description"] = ds.getDescription();
			if (ds.getValidTime())
				j["validTime"] = ds.getValidTime().value();
			if (ds.getFormats())
				j["formats"] = ds.getFormats().value();
			if (ds.getSystemLink())
				j["system@link"] = ds.getSystemLink().value();
			if (!ds.getOutputName().empty())
				j["outputName"] = ds.getOutputName();
			if (ds.getProcedureLink())
				j["procedure@link"] = ds.getProcedureLink().value();
			if (ds.getDeploymentLink())
				j["deployment@link"] = ds.getDeploymentLink().value();
			if (ds.getFeatureOfInterestLink())
				j["featureOfInterest@link"] = ds.getFeatureOfInterestLink().value();
			if (ds.getSamplingFeatureLink())
				j["samplingFeature@link"] = ds.getSamplingFeatureLink().value();
			if (ds.getObservedProperties())
				j["observedProperties"] = ds.getObservedProperties().value();
			if (ds.getPhenomenonTime())
				j["phenomenonTime"] = ds.getPhenomenonTime().value();
			if (!ds.getPhenomenonTimeInterval().empty())
				j["phenomenonTimeInterval"] = ds.getPhenomenonTimeInterval();
			if (ds.getResultTime())
				j["resultTime"] = ds.getResultTime().value();
			if (!ds.getResultTimeInterval().empty())
				j["resultTimeInterval"] = ds.getResultTimeInterval();
			if (!ds.getDataStreamType().empty())
				j["type"] = ds.getDataStreamType();
			if (!ds.getResultType().empty())
				j["resultType"] = ds.getResultType();
			if (ds.isLive())
				j["live"] = ds.isLive().value();
			if (ds.getLinks())
				j["links"] = ds.getLinks().value();
		}

		inline std::ostream& operator<<(std::ostream& os, const DataStream& ds) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, ds);
			os << j.dump(2);
			return os;
		}
	}
}