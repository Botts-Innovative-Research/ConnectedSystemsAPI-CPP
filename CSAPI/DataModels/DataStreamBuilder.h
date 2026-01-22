#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <memory>
#include <utility>

#include "DataStream.h"
#include "Link.h"
#include "TimeExtent.h"
#include "ObservedProperty.h"
#include "ObservationSchema.h"
#include "ValidationException.h"

namespace ConnectedSystemsAPI::DataModels {
	/// <summary>
	/// Builder for DataStream instances following the fluent builder pattern.
	/// This class is NOT thread-safe. Each thread should use its own builder instance.
	/// </summary>
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

		static std::unique_ptr<ObservationSchema> cloneSchema(const ObservationSchema* source) {
			return source ? std::make_unique<ObservationSchema>(*source) : nullptr;
		}

	public:
		DataStreamBuilder() = default;

		/// <summary>
		/// Create a builder initialized with values from an existing DataStream.
		/// </summary>
		explicit DataStreamBuilder(const DataStream& ds) { withDataStream(ds); }

		DataStreamBuilder(const DataStreamBuilder& other)
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
			live(other.live), links(other.links),
			schema(cloneSchema(other.schema.get()))
		{
		}

		DataStreamBuilder& operator=(const DataStreamBuilder& other) {
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
				schema = cloneSchema(other.schema.get());
			}
			return *this;
		}

		DataStreamBuilder(DataStreamBuilder&&) noexcept = default;
		DataStreamBuilder& operator=(DataStreamBuilder&&) noexcept = default;
		~DataStreamBuilder() = default;

		/// <summary>
		/// Static factory method to create a builder from an existing DataStream.
		/// </summary>
		static DataStreamBuilder from(const DataStream& ds) {
			return DataStreamBuilder(ds);
		}

		/// <summary>
		/// Initialize the builder with values from an existing DataStream instance.
		/// Note: This will perform a deep copy of all fields; use before modifying any fields or they will be overwritten.
		/// </summary>
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
			schema = cloneSchema(ds.getSchema());
			return *this;
		}

		/// <summary>
		/// Reset the builder to its default state, clearing all fields.
		/// </summary>
		DataStreamBuilder& reset() noexcept {
			id.reset();
			name.reset();
			description.reset();
			validTime.reset();
			formats.reset();
			systemLink.reset();
			outputName.reset();
			procedureLink.reset();
			deploymentLink.reset();
			featureOfInterestLink.reset();
			samplingFeatureLink.reset();
			observedProperties.reset();
			phenomenonTime.reset();
			phenomenonTimeInterval.reset();
			resultTime.reset();
			resultTimeInterval.reset();
			dataStreamType.reset();
			resultType.reset();
			live.reset();
			links.reset();
			schema.reset();
			return *this;
		}

		/// <summary>
		/// Local resource ID. If set on creation, the server may ignore it.
		/// </summary>
		DataStreamBuilder& withId(std::string_view v) noexcept { id = std::string(v); return *this; }

		/// <summary>
		/// Human-readable name of the resource.
		/// </summary>
		DataStreamBuilder& withName(std::string_view v) noexcept { name = std::string(v); return *this; }

		/// <summary>
		/// Human-readable description of the resource.
		/// </summary>
		DataStreamBuilder& withDescription(std::string_view v) noexcept { description = std::string(v); return *this; }

		/// <summary>
		/// Name of the system output feeding this datastream.
		/// </summary>
		DataStreamBuilder& withOutputName(std::string_view v) noexcept { outputName = std::string(v); return *this; }

		/// <summary>
		/// An indication of how often feature of interest properties are observed.
		/// </summary>
		DataStreamBuilder& withPhenomenonTimeInterval(std::string_view v) noexcept { phenomenonTimeInterval = std::string(v); return *this; }

		/// <summary>
		/// An indication of how often observation results are produced.
		/// </summary>
		DataStreamBuilder& withResultTimeInterval(std::string_view v) noexcept { resultTimeInterval = std::string(v); return *this; }

		/// <summary>
		/// Type of the data stream.
		/// </summary>
		DataStreamBuilder& withDataStreamType(std::string_view v) noexcept { dataStreamType = std::string(v); return *this; }

		/// <summary>
		/// The type of result for observations in the datastream.
		/// </summary>
		DataStreamBuilder& withResultType(std::string_view v) noexcept { resultType = std::string(v); return *this; }

		/// <summary>
		/// The validity period of the data stream's description.
		/// </summary>
		DataStreamBuilder& withValidTime(const TimeExtent& v) noexcept { validTime = v; return *this; }
		/// <summary>
		/// The validity period of the data stream's description.
		/// </summary>
		DataStreamBuilder& withValidTime(TimeExtent&& v) noexcept { validTime = std::move(v); return *this; }

		/// <summary>
		/// The list of formats that the observations in the datastream can be encoded to.
		/// </summary>
		DataStreamBuilder& withFormats(const std::vector<std::string>& v) noexcept { formats = v; return *this; }
		/// <summary>
		/// The list of formats that the observations in the datastream can be encoded to.
		/// </summary>
		DataStreamBuilder& withFormats(std::vector<std::string>&& v) noexcept { formats = std::move(v); return *this; }

		/// <summary>
		/// Link to the system producing the observations.
		/// </summary>
		DataStreamBuilder& withSystemLink(const Link& v) noexcept { systemLink = v; return *this; }
		/// <summary>
		/// Link to the system producing the observations.
		/// </summary>
		DataStreamBuilder& withSystemLink(Link&& v) noexcept { systemLink = std::move(v); return *this; }

		/// <summary>
		/// Link to the procedure used to acquire observations
		/// (only provided if all observations in the datastream share the same procedure).
		/// </summary>
		DataStreamBuilder& withProcedureLink(const Link& v) noexcept { procedureLink = v; return *this; }
		/// <summary>
		/// Link to the procedure used to acquire observations
		/// (only provided if all observations in the datastream share the same procedure).
		/// </summary>
		DataStreamBuilder& withProcedureLink(Link&& v) noexcept { procedureLink = std::move(v); return *this; }

		/// <summary>
		/// Link to the deployment during which the observations are/were collected
		/// (only provided if all observations in the datastream share the same deployment).
		/// </summary>
		DataStreamBuilder& withDeploymentLink(const Link& v) noexcept { deploymentLink = v; return *this; }
		/// <summary>
		/// Link to the deployment during which the observations are/were collected
		/// (only provided if all observations in the datastream share the same deployment).
		/// </summary>
		DataStreamBuilder& withDeploymentLink(Link&& v) noexcept { deploymentLink = std::move(v); return *this; }

		/// <summary>
		/// Link to the ultimate feature of interest
		/// (only provided if all observations in the datastream share the same feature of interest).
		/// </summary>
		DataStreamBuilder& withFeatureOfInterestLink(const Link& v) noexcept { featureOfInterestLink = v; return *this; }
		/// <summary>
		/// Link to the ultimate feature of interest
		/// (only provided if all observations in the datastream share the same feature of interest).
		/// </summary>
		DataStreamBuilder& withFeatureOfInterestLink(Link&& v) noexcept { featureOfInterestLink = std::move(v); return *this; }

		/// <summary>
		/// Link to the sampling feature
		/// (only provided if all observations in the datastream share the same sampling feature).
		/// </summary>
		DataStreamBuilder& withSamplingFeatureLink(const Link& v) noexcept { samplingFeatureLink = v; return *this; }
		/// <summary>
		/// Link to the sampling feature
		/// (only provided if all observations in the datastream share the same sampling feature).
		/// </summary>
		DataStreamBuilder& withSamplingFeatureLink(Link&& v) noexcept { samplingFeatureLink = std::move(v); return *this; }

		/// <summary>
		/// Properties for which the observations in the datastream provide measurements.
		/// </summary>
		DataStreamBuilder& withObservedProperties(const std::vector<ObservedProperty>& v) noexcept { observedProperties = v; return *this; }
		/// <summary>
		/// Properties for which the observations in the datastream provide measurements.
		/// </summary>
		DataStreamBuilder& withObservedProperties(std::vector<ObservedProperty>&& v) noexcept { observedProperties = std::move(v); return *this; }

		/// <summary>
		/// The time period spanned by the phenomenon times of all observations in the datastream.
		/// </summary>
		DataStreamBuilder& withPhenomenonTime(const TimeExtent& v) noexcept { phenomenonTime = v; return *this; }
		/// <summary>
		/// The time period spanned by the phenomenon times of all observations in the datastream.
		/// </summary>
		DataStreamBuilder& withPhenomenonTime(TimeExtent&& v) noexcept { phenomenonTime = std::move(v); return *this; }

		/// <summary>
		/// The time period spanned by the result times of all observations in the datastream.
		/// </summary>
		DataStreamBuilder& withResultTime(const TimeExtent& v) noexcept { resultTime = v; return *this; }
		/// <summary>
		/// The time period spanned by the result times of all observations in the datastream.
		/// </summary>
		DataStreamBuilder& withResultTime(TimeExtent&& v) noexcept { resultTime = std::move(v); return *this; }

		/// <summary>
		/// Indicates whether live data is available from the datastream.
		/// </summary>
		DataStreamBuilder& withLive(bool v) noexcept { live = v; return *this; }

		/// <summary>
		/// List of links associated with the data stream.
		/// </summary>
		DataStreamBuilder& withLinks(const std::vector<Link>& v) noexcept { links = v; return *this; }
		/// <summary>
		/// List of links associated with the data stream.
		/// </summary>
		DataStreamBuilder& withLinks(std::vector<Link>&& v) noexcept { links = std::move(v); return *this; }

		/// <summary>
		/// Add a format that the observations in the datastream can be encoded to.
		/// </summary>
		DataStreamBuilder& addFormat(std::string_view f) {
			if (!formats) formats = std::vector<std::string>{};
			formats->emplace_back(f);
			return *this;
		}

		/// <summary>
		/// Add a format that the observations in the datastream can be encoded to.
		/// </summary>
		DataStreamBuilder& addFormat(std::string&& f) {
			if (!formats) formats = std::vector<std::string>{};
			formats->emplace_back(std::move(f));
			return *this;
		}

		/// <summary>
		/// Add an observed property for which the observations in the datastream provide measurements.
		/// </summary>
		DataStreamBuilder& addObservedProperty(const ObservedProperty& p) {
			if (!observedProperties) observedProperties = std::vector<ObservedProperty>{};
			observedProperties->push_back(p);
			return *this;
		}

		/// <summary>
		/// Add an observed property for which the observations in the datastream provide measurements.
		/// </summary>
		DataStreamBuilder& addObservedProperty(ObservedProperty&& p) {
			if (!observedProperties) observedProperties = std::vector<ObservedProperty>{};
			observedProperties->emplace_back(std::move(p));
			return *this;
		}

		/// <summary>
		/// Add a link associated with the data stream.
		/// </summary>
		DataStreamBuilder& addLink(const Link& l) {
			if (!links) links = std::vector<Link>{};
			links->push_back(l);
			return *this;
		}

		/// <summary>
		/// Add a link associated with the data stream.
		/// </summary>
		DataStreamBuilder& addLink(Link&& l) {
			if (!links) links = std::vector<Link>{};
			links->emplace_back(std::move(l));
			return *this;
		}

		/// <summary>
		/// Clear all formats.
		/// </summary>
		DataStreamBuilder& clearFormats() noexcept {
			formats.reset();
			return *this;
		}

		/// <summary>
		/// Clear all observed properties.
		/// </summary>
		DataStreamBuilder& clearObservedProperties() noexcept {
			observedProperties.reset();
			return *this;
		}

		/// <summary>
		/// Clear all links.
		/// </summary>
		DataStreamBuilder& clearLinks() noexcept {
			links.reset();
			return *this;
		}

		/// <summary>
		/// Set the observation schema by transferring ownership of a unique_ptr.
		/// </summary>
		DataStreamBuilder& withSchema(std::unique_ptr<ObservationSchema> v) noexcept {
			schema = std::move(v);
			return *this;
		}

		/// <summary>
		/// Set the observation schema by copying from an existing ObservationSchema instance.
		/// Note: This performs a deep copy via the ObservationSchema copy constructor.
		/// </summary>
		DataStreamBuilder& withSchema(const ObservationSchema& v) {
			schema = std::make_unique<ObservationSchema>(v);
			return *this;
		}

		// Getters for inspection
		const std::optional<std::string>& getId() const noexcept { return id; }
		const std::optional<std::string>& getName() const noexcept { return name; }
		const std::optional<std::string>& getDescription() const noexcept { return description; }
		const std::optional<TimeExtent>& getValidTime() const noexcept { return validTime; }
		const std::optional<std::vector<std::string>>& getFormats() const noexcept { return formats; }
		const std::optional<Link>& getSystemLink() const noexcept { return systemLink; }
		const std::optional<std::string>& getOutputName() const noexcept { return outputName; }
		const std::optional<Link>& getProcedureLink() const noexcept { return procedureLink; }
		const std::optional<Link>& getDeploymentLink() const noexcept { return deploymentLink; }
		const std::optional<Link>& getFeatureOfInterestLink() const noexcept { return featureOfInterestLink; }
		const std::optional<Link>& getSamplingFeatureLink() const noexcept { return samplingFeatureLink; }
		const std::optional<std::vector<ObservedProperty>>& getObservedProperties() const noexcept { return observedProperties; }
		const std::optional<TimeExtent>& getPhenomenonTime() const noexcept { return phenomenonTime; }
		const std::optional<std::string>& getPhenomenonTimeInterval() const noexcept { return phenomenonTimeInterval; }
		const std::optional<TimeExtent>& getResultTime() const noexcept { return resultTime; }
		const std::optional<std::string>& getResultTimeInterval() const noexcept { return resultTimeInterval; }
		const std::optional<std::string>& getDataStreamType() const noexcept { return dataStreamType; }
		const std::optional<std::string>& getResultType() const noexcept { return resultType; }
		const std::optional<bool>& isLive() const noexcept { return live; }
		const std::optional<std::vector<Link>>& getLinks() const noexcept { return links; }
		const ObservationSchema* getSchema() const noexcept { return schema.get(); }

		/// <summary>
		/// Check if the current builder state is valid. Optionally populate a vector with validation errors.
		/// </summary>
		/// <param name="outErrors">Optional pointer to a vector to populate with validation errors. If nullptr, only validation result is returned.</param>
		/// <returns>True if validation passes, false otherwise.</returns>
		bool isValid(std::vector<ConnectedSystemsAPI::ValidationException::ValidationError>* outErrors = nullptr) const {
			std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors;

			if (!name || name->empty())
				errors.emplace_back("name", "Field is required and cannot be empty");

			if (!outputName || outputName->empty())
				errors.emplace_back("outputName", "Field is required and cannot be empty");

			if (!schema)
				errors.emplace_back("schema", "Field is required");

			bool valid = errors.empty();

			if (outErrors)
				*outErrors = std::move(errors);

			return valid;
		}

		/// <summary>
		/// Build a DataStream instance from the current builder state.
		/// Note: This method clones the schema, so the builder can be reused for multiple builds.
		/// </summary>
		/// <returns>A new DataStream instance.</returns>
		/// <exception cref="ValidationException">Thrown if validation fails.</exception>
		DataStream build() const {
			if (std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors; !isValid(&errors)) {
				throw ConnectedSystemsAPI::ValidationException("DataStreamBuilder", errors);
			}

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
				schema ? std::make_unique<ObservationSchema>(*schema) : nullptr
			);
		}
	};
}