#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <utility>

#include "ControlStream.h"
#include "Link.h"
#include "TimeExtent.h"
#include "ControlledProperty.h"
#include "ValidationException.h"

namespace ConnectedSystemsAPI::DataModels {
	/// <summary>
	/// Builder for ControlStream instances following the fluent builder pattern.
	/// This class is NOT thread-safe. Each thread should use its own builder instance.
	/// </summary>
	class ControlStreamBuilder {
	private:
		std::optional<std::string> id;
		std::optional<std::string> name;
		std::optional<std::string> description;
		std::optional<TimeExtent> validTime;
		std::optional<std::vector<std::string>> formats;
		std::optional<Link> systemLink;
		std::optional<std::string> inputName;
		std::optional<Link> procedureLink;
		std::optional<Link> deploymentLink;
		std::optional<Link> featureOfInterestLink;
		std::optional<Link> samplingFeatureLink;
		std::optional<std::vector<ControlledProperty>> controlledProperties;
		std::optional<TimeExtent> issueTime;
		std::optional<TimeExtent> executionTime;
		std::optional<bool> live;
		std::optional<bool> async;
		std::optional<std::vector<Link>> links;

	public:
		ControlStreamBuilder() = default;

		/// <summary>
		/// Create a builder initialized with values from an existing ControlStream.
		/// </summary>
		explicit ControlStreamBuilder(const ControlStream& cs) { withControlStream(cs); }

		ControlStreamBuilder(const ControlStreamBuilder& other) = default;
		ControlStreamBuilder& operator=(const ControlStreamBuilder& other) = default;
		ControlStreamBuilder(ControlStreamBuilder&&) noexcept = default;
		ControlStreamBuilder& operator=(ControlStreamBuilder&&) noexcept = default;
		~ControlStreamBuilder() = default;

		/// <summary>
		/// Static factory method to create a builder from an existing ControlStream.
		/// </summary>
		static ControlStreamBuilder from(const ControlStream& cs) {
			return ControlStreamBuilder(cs);
		}

		/// <summary>
		/// Initialize the builder with values from an existing ControlStream instance.
		/// Note: This will perform a deep copy of all fields; use before modifying any fields or they will be overwritten.
		/// </summary>
		ControlStreamBuilder& withControlStream(const ControlStream& cs) {
			id = cs.getId();
			name = cs.getName();
			description = cs.getDescription();
			validTime = cs.getValidTime();
			formats = cs.getFormats();
			systemLink = cs.getSystemLink();
			inputName = cs.getInputName();
			procedureLink = cs.getProcedureLink();
			deploymentLink = cs.getDeploymentLink();
			featureOfInterestLink = cs.getFeatureOfInterestLink();
			samplingFeatureLink = cs.getSamplingFeatureLink();
			controlledProperties = cs.getControlledProperties();
			issueTime = cs.getIssueTime();
			executionTime = cs.getExecutionTime();
			live = cs.isLive();
			async = cs.isAsync();
			links = cs.getLinks();
			return *this;
		}

		/// <summary>
		/// Reset the builder to its default state, clearing all fields.
		/// </summary>
		ControlStreamBuilder& reset() noexcept {
			id.reset();
			name.reset();
			description.reset();
			validTime.reset();
			formats.reset();
			systemLink.reset();
			inputName.reset();
			procedureLink.reset();
			deploymentLink.reset();
			featureOfInterestLink.reset();
			samplingFeatureLink.reset();
			controlledProperties.reset();
			issueTime.reset();
			executionTime.reset();
			live.reset();
			async.reset();
			links.reset();
			return *this;
		}

		/// <summary>
		/// Local resource ID. If set on creation, the server may ignore it.
		/// </summary>
		ControlStreamBuilder& withId(std::string_view v) noexcept { id = std::string(v); return *this; }

		/// <summary>
		/// Human-readable name of the resource.
		/// </summary>
		ControlStreamBuilder& withName(std::string_view v) noexcept { name = std::string(v); return *this; }

		/// <summary>
		/// Human-readable description of the resource.
		/// </summary>
		ControlStreamBuilder& withDescription(std::string_view v) noexcept { description = std::string(v); return *this; }

		/// <summary>
		/// Name of the system input feeding this control stream.
		/// </summary>
		ControlStreamBuilder& withInputName(std::string_view v) noexcept { inputName = std::string(v); return *this; }

		/// <summary>
		/// The validity period of the control stream's description.
		/// </summary>
		ControlStreamBuilder& withValidTime(const TimeExtent& v) noexcept { validTime = v; return *this; }
		/// <summary>
		/// The validity period of the control stream's description.
		/// </summary>
		ControlStreamBuilder& withValidTime(TimeExtent&& v) noexcept { validTime = std::move(v); return *this; }

		/// <summary>
		/// The list of formats that the observations in the control stream can be encoded to.
		/// </summary>
		ControlStreamBuilder& withFormats(const std::vector<std::string>& v) noexcept { formats = v; return *this; }
		/// <summary>
		/// The list of formats that the observations in the control stream can be encoded to.
		/// </summary>
		ControlStreamBuilder& withFormats(std::vector<std::string>&& v) noexcept { formats = std::move(v); return *this; }

		/// <summary>
		/// Link to the system producing the observations.
		/// </summary>
		ControlStreamBuilder& withSystemLink(const Link& v) noexcept { systemLink = v; return *this; }
		/// <summary>
		/// Link to the system producing the observations.
		/// </summary>
		ControlStreamBuilder& withSystemLink(Link&& v) noexcept { systemLink = std::move(v); return *this; }

		/// <summary>
		/// Link to the procedure used to acquire observations
		/// (only provided if all observations in the control stream share the same procedure).
		/// </summary>
		ControlStreamBuilder& withProcedureLink(const Link& v) noexcept { procedureLink = v; return *this; }
		/// <summary>
		/// Link to the procedure used to acquire observations
		/// (only provided if all observations in the control stream share the same procedure).
		/// </summary>
		ControlStreamBuilder& withProcedureLink(Link&& v) noexcept { procedureLink = std::move(v); return *this; }

		/// <summary>
		/// Link to the deployment during which the observations are/were collected
		/// (only provided if all observations in the control stream share the same deployment).
		/// </summary>
		ControlStreamBuilder& withDeploymentLink(const Link& v) noexcept { deploymentLink = v; return *this; }
		/// <summary>
		/// Link to the deployment during which the observations are/were collected
		/// (only provided if all observations in the control stream share the same deployment).
		/// </summary>
		ControlStreamBuilder& withDeploymentLink(Link&& v) noexcept { deploymentLink = std::move(v); return *this; }

		/// <summary>
		/// Link to the ultimate feature of interest
		/// (only provided if all observations in the control stream share the same feature of interest).
		/// </summary>
		ControlStreamBuilder& withFeatureOfInterestLink(const Link& v) noexcept { featureOfInterestLink = v; return *this; }
		/// <summary>
		/// Link to the ultimate feature of interest
		/// (only provided if all observations in the control stream share the same feature of interest).
		/// </summary>
		ControlStreamBuilder& withFeatureOfInterestLink(Link&& v) noexcept { featureOfInterestLink = std::move(v); return *this; }

		/// <summary>
		/// Link to the sampling feature
		/// (only provided if all observations in the control stream share the same sampling feature).
		/// </summary>
		ControlStreamBuilder& withSamplingFeatureLink(const Link& v) noexcept { samplingFeatureLink = v; return *this; }
		/// <summary>
		/// Link to the sampling feature
		/// (only provided if all observations in the control stream share the same sampling feature).
		/// </summary>
		ControlStreamBuilder& withSamplingFeatureLink(Link&& v) noexcept { samplingFeatureLink = std::move(v); return *this; }

		/// <summary>
		/// Properties for which the observations in the control stream provide measurements.
		/// </summary>
		ControlStreamBuilder& withControlledProperties(const std::vector<ControlledProperty>& v) noexcept { controlledProperties = v; return *this; }
		/// <summary>
		/// Properties for which the observations in the control stream provide measurements.
		/// </summary>
		ControlStreamBuilder& withControlledProperties(std::vector<ControlledProperty>&& v) noexcept { controlledProperties = std::move(v); return *this; }

		/// <summary>
		/// The time period spanned by the issue times of all observations in the control stream.
		/// </summary>
		ControlStreamBuilder& withIssueTime(const TimeExtent& v) noexcept { issueTime = v; return *this; }
		/// <summary>
		/// The time period spanned by the issue times of all observations in the control stream.
		/// </summary>
		ControlStreamBuilder& withIssueTime(TimeExtent&& v) noexcept { issueTime = std::move(v); return *this; }

		/// <summary>
		/// The time period spanned by the result times of all observations in the control stream.
		/// </summary>
		ControlStreamBuilder& withExecutionTime(const TimeExtent& v) noexcept { executionTime = v; return *this; }
		/// <summary>
		/// The time period spanned by the result times of all observations in the control stream.
		/// </summary>
		ControlStreamBuilder& withExecutionTime(TimeExtent&& v) noexcept { executionTime = std::move(v); return *this; }

		/// <summary>
		/// Flag indicating if the command channel can currently accept commands.
		/// </summary>
		ControlStreamBuilder& withLive(bool v) noexcept { live = v; return *this; }

		/// <summary>
		/// Flag indicating if the control stream processes commands asynchronously.
		/// </summary>
		ControlStreamBuilder& withAsync(bool v) noexcept { async = v; return *this; }

		/// <summary>
		/// Links to related resources.
		/// </summary>
		ControlStreamBuilder& withLinks(const std::vector<Link>& v) noexcept { links = v; return *this; }
		/// <summary>
		/// Links to related resources.
		/// </summary>
		ControlStreamBuilder& withLinks(std::vector<Link>&& v) noexcept { links = std::move(v); return *this; }

		/// <summary>
		/// Add a format that the observations in the control stream can be encoded to.
		/// </summary>
		ControlStreamBuilder& addFormat(std::string_view f) {
			if (!formats) formats = std::vector<std::string>{};
			formats->emplace_back(f);
			return *this;
		}

		/// <summary>
		/// Add a format that the observations in the control stream can be encoded to.
		/// </summary>
		ControlStreamBuilder& addFormat(std::string&& f) {
			if (!formats) formats = std::vector<std::string>{};
			formats->emplace_back(std::move(f));
			return *this;
		}

		/// <summary>
		/// Add a controlled property for which the observations in the control stream provide measurements.
		/// </summary>
		ControlStreamBuilder& addControlledProperty(const ControlledProperty& p) {
			if (!controlledProperties) controlledProperties = std::vector<ControlledProperty>{};
			controlledProperties->push_back(p);
			return *this;
		}

		/// <summary>
		/// Add a controlled property for which the observations in the control stream provide measurements.
		/// </summary>
		ControlStreamBuilder& addControlledProperty(ControlledProperty&& p) {
			if (!controlledProperties) controlledProperties = std::vector<ControlledProperty>{};
			controlledProperties->emplace_back(std::move(p));
			return *this;
		}

		/// <summary>
		/// Add a link to related resources.
		/// </summary>
		ControlStreamBuilder& addLink(const Link& l) {
			if (!links) links = std::vector<Link>{};
			links->push_back(l);
			return *this;
		}

		/// <summary>
		/// Add a link to related resources.
		/// </summary>
		ControlStreamBuilder& addLink(Link&& l) {
			if (!links) links = std::vector<Link>{};
			links->emplace_back(std::move(l));
			return *this;
		}

		/// <summary>
		/// Clear all formats.
		/// </summary>
		ControlStreamBuilder& clearFormats() noexcept {
			formats.reset();
			return *this;
		}

		/// <summary>
		/// Clear all controlled properties.
		/// </summary>
		ControlStreamBuilder& clearControlledProperties() noexcept {
			controlledProperties.reset();
			return *this;
		}

		/// <summary>
		/// Clear all links.
		/// </summary>
		ControlStreamBuilder& clearLinks() noexcept {
			links.reset();
			return *this;
		}

		const std::optional<std::string>& getId() const noexcept { return id; }
		const std::optional<std::string>& getName() const noexcept { return name; }
		const std::optional<std::string>& getDescription() const noexcept { return description; }
		const std::optional<TimeExtent>& getValidTime() const noexcept { return validTime; }
		const std::optional<std::vector<std::string>>& getFormats() const noexcept { return formats; }
		const std::optional<Link>& getSystemLink() const noexcept { return systemLink; }
		const std::optional<std::string>& getInputName() const noexcept { return inputName; }
		const std::optional<Link>& getProcedureLink() const noexcept { return procedureLink; }
		const std::optional<Link>& getDeploymentLink() const noexcept { return deploymentLink; }
		const std::optional<Link>& getFeatureOfInterestLink() const noexcept { return featureOfInterestLink; }
		const std::optional<Link>& getSamplingFeatureLink() const noexcept { return samplingFeatureLink; }
		const std::optional<std::vector<ControlledProperty>>& getControlledProperties() const noexcept { return controlledProperties; }
		const std::optional<TimeExtent>& getIssueTime() const noexcept { return issueTime; }
		const std::optional<TimeExtent>& getExecutionTime() const noexcept { return executionTime; }
		const std::optional<bool>& isLive() const noexcept { return live; }
		const std::optional<bool>& isAsync() const noexcept { return async; }
		const std::optional<std::vector<Link>>& getLinks() const noexcept { return links; }

		/// <summary>
		/// Check if the current builder state is valid. Optionally populate a vector with validation errors.
		/// </summary>
		/// <param name="outErrors">Optional pointer to a vector to populate with validation errors. If nullptr, only validation result is returned.</param>
		/// <returns>True if validation passes, false otherwise.</returns>
		bool isValid(std::vector<ConnectedSystemsAPI::ValidationException::ValidationError>* outErrors = nullptr) const {
			std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors;

			if (!name || name->empty())
				errors.emplace_back("name", "Field is required and cannot be empty");

			if (!inputName || inputName->empty())
				errors.emplace_back("inputName", "Field is required and cannot be empty");

			bool valid = errors.empty();

			if (outErrors)
				*outErrors = std::move(errors);

			return valid;
		}

		/// <summary>
		/// Build a ControlStream instance from the current builder state.
		/// </summary>
		/// <returns>A new ControlStream instance.</returns>
		/// <exception cref="ValidationException">Thrown if validation fails.</exception>
		ControlStream build() const {
			if (std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors; !isValid(&errors)) {
				throw ConnectedSystemsAPI::ValidationException("ControlStreamBuilder", errors);
			}

			return ControlStream(
				id,
				name,
				description,
				validTime,
				formats,
				systemLink,
				inputName,
				procedureLink,
				deploymentLink,
				featureOfInterestLink,
				samplingFeatureLink,
				controlledProperties,
				issueTime,
				executionTime,
				live,
				async,
				links
			);
		}
	};
}
