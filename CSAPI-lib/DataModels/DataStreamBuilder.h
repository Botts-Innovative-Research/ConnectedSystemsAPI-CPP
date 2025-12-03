#pragma once

#include <string>
#include <vector>
#include <optional>
#include <memory>

#include "DataStream.h"
#include "Link.h"
#include "TimeExtent.h"
#include "ObservedProperty.h"
#include "ObservationSchema.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class DataStreamBuilder {
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

			void validate() const {
				if (!name)
					throw std::runtime_error("DataStreamBuilder: name is required.");
				if (!outputName)
					throw std::runtime_error("DataStreamBuilder: outputName is required.");
				if (!schema)
					throw std::runtime_error("DataStreamBuilder: schema is required.");
			}

		public:
			DataStreamBuilder() = default;
			DataStreamBuilder(const DataStream& ds) { withDataStream(ds); }

			/// <summary>
			/// Initialize the builder with values from an existing DataStream instance.
			/// Note: This will perform a deep copy of all fields; use before modifying any fields or they will be overwritten.
			/// </summary>
			/// <param name="ds">The DataStream instance to copy values from.</param>
			/// <returns>The builder instance.</returns>
			DataStreamBuilder& withDataStream(const DataStream& ds) {
				id = ds.getId();
				name = ds.getName();
				description = ds.getDescription();
				validTime = ds.getValidTime();
				formats = ds.getFormats();
				systemLink = ds.getSystemLink();
				outputName = ds.getOutputName();
				procedureLink = ds.getProcedureLink();
				deploymentLink = ds.getDeploymentLink();
				featureOfInterestLink = ds.getFeatureOfInterestLink();
				samplingFeatureLink = ds.getSamplingFeatureLink();
				observedProperties = ds.getObservedProperties();
				phenomenonTime = ds.getPhenomenonTime();
				phenomenonTimeInterval = ds.getPhenomenonTimeInterval();
				resultTime = ds.getResultTime();
				resultTimeInterval = ds.getResultTimeInterval();
				dataStreamType = ds.getDataStreamType();
				resultType = ds.getResultType();
				live = ds.isLive();
				links = ds.getLinks();

				// ObservationSchema is non-copyable; clone via JSON round-trip (to_json/from_json)
				if (ds.getSchema()) {
					nlohmann::ordered_json j;
					to_json(j, *ds.getSchema());
					auto copy = std::make_unique<ObservationSchema>();
					from_json(j, *copy);
					schema = std::move(copy);
				}
				else {
					schema.reset();
				}

				return *this;
			}

			DataStreamBuilder& withId(const std::string& v) { id = v; return *this; }
			DataStreamBuilder& withName(const std::string& v) { name = v; return *this; }
			DataStreamBuilder& withDescription(const std::string& v) { description = v; return *this; }
			DataStreamBuilder& withValidTime(const TimeExtent& v) { validTime = v; return *this; }
			DataStreamBuilder& withFormats(const std::vector<std::string>& v) { formats = v; return *this; }
			DataStreamBuilder& addFormat(const std::string& f) {
				if (!formats) formats = std::vector<std::string>{};
				formats->push_back(f);
				return *this;
			}
			DataStreamBuilder& withSystemLink(const Link& v) { systemLink = v; return *this; }
			DataStreamBuilder& withOutputName(const std::string& v) { outputName = v; return *this; }
			DataStreamBuilder& withProcedureLink(const Link& v) { procedureLink = v; return *this; }
			DataStreamBuilder& withDeploymentLink(const Link& v) { deploymentLink = v; return *this; }
			DataStreamBuilder& withFeatureOfInterestLink(const Link& v) { featureOfInterestLink = v; return *this; }
			DataStreamBuilder& withSamplingFeatureLink(const Link& v) { samplingFeatureLink = v; return *this; }
			DataStreamBuilder& withObservedProperties(const std::vector<ObservedProperty>& v) { observedProperties = v; return *this; }
			DataStreamBuilder& addObservedProperty(const ObservedProperty& p) {
				if (!observedProperties) observedProperties = std::vector<ObservedProperty>{};
				observedProperties->push_back(p);
				return *this;
			}
			DataStreamBuilder& withPhenomenonTime(const TimeExtent& v) { phenomenonTime = v; return *this; }
			DataStreamBuilder& withPhenomenonTimeInterval(const std::string& v) { phenomenonTimeInterval = v; return *this; }
			DataStreamBuilder& withResultTime(const TimeExtent& v) { resultTime = v; return *this; }
			DataStreamBuilder& withResultTimeInterval(const std::string& v) { resultTimeInterval = v; return *this; }
			DataStreamBuilder& withDataStreamType(const std::string& v) { dataStreamType = v; return *this; }
			DataStreamBuilder& withResultType(const std::string& v) { resultType = v; return *this; }
			DataStreamBuilder& withLive(bool v) { live = v; return *this; }
			DataStreamBuilder& withLinks(const std::vector<Link>& v) { links = v; return *this; }
			DataStreamBuilder& addLink(const Link& l) {
				if (!links) links = std::vector<Link>{};
				links->push_back(l);
				return *this;
			}
			DataStreamBuilder& withSchema(std::unique_ptr<ObservationSchema> v) { schema = std::move(v); return *this; }

			DataStream build() {
				validate();

				return DataStream(
					id,
					name,
					description,
					validTime,
					formats,
					systemLink,
					outputName,
					procedureLink,
					deploymentLink,
					featureOfInterestLink,
					samplingFeatureLink,
					observedProperties,
					phenomenonTime,
					phenomenonTimeInterval,
					resultTime,
					resultTimeInterval,
					dataStreamType,
					resultType,
					live,
					links,
					schema ? std::move(schema) : nullptr
				);
			}
		};
	}
}