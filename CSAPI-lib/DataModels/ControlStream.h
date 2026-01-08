#pragma once

#include <string>
#include <optional>
#include <vector>
#include <memory>
#include <ostream>
#include <nlohmann/json.hpp>

#include "Link.h"
#include "TimeExtent.h"
#include "ControlledProperty.h"
#include "CommandSchema.h"

namespace ConnectedSystemsAPI::DataModels {
	class ControlStream {
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
		std::unique_ptr<CommandSchema> schema;

	public:
		ControlStream() = default;
		ControlStream(const std::optional<std::string>& id,
			const std::optional<std::string>& name,
			const std::optional<std::string>& description,
			const std::optional<TimeExtent>& validTime,
			const std::optional<std::vector<std::string>>& formats,
			const std::optional<Link>& systemLink,
			const std::optional<std::string>& inputName,
			const std::optional<Link>& procedureLink,
			const std::optional<Link>& deploymentLink,
			const std::optional<Link>& featureOfInterestLink,
			const std::optional<Link>& samplingFeatureLink,
			const std::optional<std::vector<ControlledProperty>>& controlledProperties,
			const std::optional<TimeExtent>& issueTime,
			const std::optional<TimeExtent>& executionTime,
			const std::optional<bool>& live,
			const std::optional<bool>& async,
			const std::optional<std::vector<Link>>& links,
			std::unique_ptr<CommandSchema> schema)
			: id(id), name(name), description(description), validTime(validTime), formats(formats),
			systemLink(systemLink), inputName(inputName), procedureLink(procedureLink),
			deploymentLink(deploymentLink), featureOfInterestLink(featureOfInterestLink),
			samplingFeatureLink(samplingFeatureLink),
			controlledProperties(controlledProperties),
			issueTime(issueTime), executionTime(executionTime),
			live(live), async(async), links(links), schema(std::move(schema))
		{
		}

		ControlStream(const ControlStream& other)
			: id(other.id), name(other.name), description(other.description),
			validTime(other.validTime), formats(other.formats),
			systemLink(other.systemLink), inputName(other.inputName),
			procedureLink(other.procedureLink), deploymentLink(other.deploymentLink),
			featureOfInterestLink(other.featureOfInterestLink),
			samplingFeatureLink(other.samplingFeatureLink),
			controlledProperties(other.controlledProperties),
			issueTime(other.issueTime), executionTime(other.executionTime),
			live(other.live), async(other.async), links(other.links)
		{
			if (other.schema) {
				schema = std::make_unique<CommandSchema>(
					other.schema->getCommandFormat(),
					cloneComponent(other.schema->getParametersSchema()),
					cloneComponent(other.schema->getResultSchema()),
					cloneComponent(other.schema->getFeasibilityResultSchema())
				);
			}
		}

		ControlStream(ControlStream&&) noexcept = default;

		ControlStream& operator=(const ControlStream& other) {
			if (this != &other) {
				id = other.id;
				name = other.name;
				description = other.description;
				validTime = other.validTime;
				formats = other.formats;
				systemLink = other.systemLink;
				inputName = other.inputName;
				procedureLink = other.procedureLink;
				deploymentLink = other.deploymentLink;
				featureOfInterestLink = other.featureOfInterestLink;
				samplingFeatureLink = other.samplingFeatureLink;
				controlledProperties = other.controlledProperties;
				issueTime = other.issueTime;
				executionTime = other.executionTime;
				live = other.live;
				async = other.async;
				links = other.links;

				if (other.schema) {
					schema = std::make_unique<CommandSchema>(
						other.schema->getCommandFormat(),
						cloneComponent(other.schema->getParametersSchema()),
						cloneComponent(other.schema->getResultSchema()),
						cloneComponent(other.schema->getFeasibilityResultSchema())
					);
				}
				else {
					schema.reset();
				}
			}
			return *this;
		}

		ControlStream& operator=(ControlStream&&) noexcept = default;

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
		/// The validity period of the control stream's description.
		/// </summary>
		const std::optional<TimeExtent>& getValidTime() const { return validTime; }
		/// <summary>
		/// The list of formats that the observations in the control stream can be encoded to.
		/// </summary>
		const std::optional<std::vector<std::string>>& getFormats() const { return formats; }
		/// <summary>
		/// Link to the system producing the observations.
		/// </summary>
		const std::optional<Link>& getSystemLink() const { return systemLink; }
		/// <summary>
		/// Name of the system input feeding this control stream.
		/// </summary>
		const std::optional<std::string>& getInputName() const { return inputName; }
		/// <summary>Link to the procedure used to acquire observations
		/// (only provided if all observations in the control stream share the same procedure).
		/// </summary>
		const std::optional<Link>& getProcedureLink() const { return procedureLink; }
		/// <summary>
		/// Link to the deployment during which the observations are/were collected
		/// (only provided if all observations in the control stream share the same deployment).
		/// </summary>
		const std::optional<Link>& getDeploymentLink() const { return deploymentLink; }
		/// <summary>
		/// Link to the ultimate feature of interest
		/// (only provided if all observations in the control stream share the same feature of interest).
		/// </summary>
		const std::optional<Link>& getFeatureOfInterestLink() const { return featureOfInterestLink; }
		/// <summary>
		/// Link to the sampling feature
		/// (only provided if all observations in the control stream share the same sampling feature).
		/// </summary>
		const std::optional<Link>& getSamplingFeatureLink() const { return samplingFeatureLink; }
		/// <summary>
		/// Properties for which the observations in the control stream provide measurements.
		/// </summary>
		const std::optional<std::vector<ControlledProperty>>& getControlledProperties() const { return controlledProperties; }
		/// <summary>
		/// The time period spanned by the issue times of all observations in the control stream.
		/// </summary>
		const std::optional<TimeExtent>& getIssueTime() const { return issueTime; }
		/// <summary>
		/// The time period spanned by the result times of all observations in the control stream.
		/// </summary>
		const std::optional<TimeExtent>& getExecutionTime() const { return executionTime; }
		/// <summary>
		/// Flag indicating if the command channel can currently accept commands.
		/// </summary>
		const std::optional<bool>& isLive() const { return live; }
		/// <summary>
		/// Flag indicating if the control stream processes commands asynchronously.
		/// </summary>
		const std::optional<bool>& isAsync() const { return async; }
		/// <summary>
		/// Links to related resources.
		/// </summary>
		const std::optional<std::vector<Link>>& getLinks() const { return links; }
		/// <summary>
		/// The command schema for the control stream, if available.
		/// </summary>
		const CommandSchema* getSchema() const { return schema.get(); }

		friend void from_json(const nlohmann::json& j, ControlStream& o);
		friend void to_json(nlohmann::ordered_json& j, const ControlStream& o);
		friend bool operator==(const ControlStream& a, const ControlStream& b);
		friend bool operator!=(const ControlStream& a, const ControlStream& b);

	private:
		static std::unique_ptr<Component::DataComponent> cloneComponent(const Component::DataComponent* source) {
			if (!source) return nullptr;
			nlohmann::ordered_json j = source->toJson();
			return Component::DataComponentRegistry::createDataComponent(j);
		}
	};

	inline void from_json(const nlohmann::json& j, ControlStream& v) {
		std::unique_ptr<CommandSchema> schemaPtr;
		if (j.contains("schema") && !j["schema"].is_null()) {
			schemaPtr = std::make_unique<CommandSchema>(j["schema"].get<CommandSchema>());
		}

		v.id = j.value("id", std::optional<std::string>{});
		v.name = j.value("name", std::optional<std::string>{});
		v.description = j.value("description", std::optional<std::string>{});
		v.validTime = j.value("validTime", std::optional<TimeExtent>{});
		v.formats = j.value("formats", std::optional<std::vector<std::string>>{});
		v.systemLink = j.value("systemLink", std::optional<Link>{});
		v.inputName = j.value("inputName", std::optional<std::string>{});
		v.procedureLink = j.value("procedureLink", std::optional<Link>{});
		v.deploymentLink = j.value("deploymentLink", std::optional<Link>{});
		v.featureOfInterestLink = j.value("featureOfInterestLink", std::optional<Link>{});
		v.samplingFeatureLink = j.value("samplingFeatureLink", std::optional<Link>{});
		v.controlledProperties = j.value("controlledProperties", std::optional<std::vector<ControlledProperty>>{});
		v.issueTime = j.value("issueTime", std::optional<TimeExtent>{});
		v.executionTime = j.value("executionTime", std::optional<TimeExtent>{});
		v.live = j.value("live", std::optional<bool>{});
		v.async = j.value("async", std::optional<bool>{});
		v.links = j.value("links", std::optional<std::vector<Link>>{});
		v.schema = std::move(schemaPtr);
	}

	inline void to_json(nlohmann::ordered_json& j, const ControlStream& v) {
		j = nlohmann::ordered_json::object();

		if (v.id.has_value()) j["id"] = v.id.value();
		if (v.name.has_value()) j["name"] = v.name.value();
		if (v.description.has_value()) j["description"] = v.description.value();
		if (v.validTime.has_value()) j["validTime"] = v.validTime.value();
		if (v.formats.has_value()) j["formats"] = v.formats.value();
		if (v.systemLink.has_value()) j["systemLink"] = v.systemLink.value();
		if (v.inputName.has_value()) j["inputName"] = v.inputName.value();
		if (v.procedureLink.has_value()) j["procedureLink"] = v.procedureLink.value();
		if (v.deploymentLink.has_value()) j["deploymentLink"] = v.deploymentLink.value();
		if (v.featureOfInterestLink.has_value()) j["featureOfInterestLink"] = v.featureOfInterestLink.value();
		if (v.samplingFeatureLink.has_value()) j["samplingFeatureLink"] = v.samplingFeatureLink.value();
		if (v.controlledProperties.has_value()) j["controlledProperties"] = v.controlledProperties.value();
		if (v.issueTime.has_value()) j["issueTime"] = v.issueTime.value();
		if (v.executionTime.has_value()) j["executionTime"] = v.executionTime.value();
		if (v.live.has_value()) j["live"] = v.live.value();
		if (v.async.has_value()) j["async"] = v.async.value();
		if (v.links.has_value()) j["links"] = v.links.value();
		if (v.schema) j["schema"] = *v.schema;
	}

	inline std::ostream& operator<<(std::ostream& os, const ControlStream& v) {
		nlohmann::ordered_json j;
		ConnectedSystemsAPI::DataModels::to_json(j, v);
		os << j.dump(2);
		return os;
	}

	inline bool operator==(const ControlStream& a, const ControlStream& b) {
		// Note: Schema comparison not implemented as CommandSchema doesn't have operator==
		return a.id == b.id
			&& a.name == b.name
			&& a.description == b.description
			&& a.validTime == b.validTime
			&& a.formats == b.formats
			&& a.systemLink == b.systemLink
			&& a.inputName == b.inputName
			&& a.procedureLink == b.procedureLink
			&& a.deploymentLink == b.deploymentLink
			&& a.featureOfInterestLink == b.featureOfInterestLink
			&& a.samplingFeatureLink == b.samplingFeatureLink
			&& a.controlledProperties == b.controlledProperties
			&& a.issueTime == b.issueTime
			&& a.executionTime == b.executionTime
			&& a.live == b.live
			&& a.async == b.async
			&& a.links == b.links;
	}

	inline bool operator!=(const ControlStream& a, const ControlStream& b) {
		return !(a == b);
	}
}