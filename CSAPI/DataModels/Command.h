#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Link.h"
#include "TimeInstant.h"
#include "TimeExtent.h"

namespace ConnectedSystemsAPI::DataModels {
	class Command;
	void to_json(nlohmann::ordered_json& j, const Command& v);

	class Command {
	private:
		std::optional<std::string> id;
		std::optional<std::string> controlStreamId;
		std::optional<std::string> samplingFeatureId;
		std::optional<Link> procedureLink;
		std::optional<TimeInstant> issueTime;
		std::optional<TimeExtent> executionTime;
		std::optional<std::string> sender;
		std::optional<std::string> currentStatus;
		Data::DataBlockMixed parameters;
		std::optional<std::vector<Link>> links;

	public:
		Command() = default;
		Command(const std::optional<std::string>& id,
			const std::optional<std::string>& controlStreamId,
			const std::optional<std::string>& samplingFeatureId,
			const std::optional<Link>& procedureLink,
			const std::optional<TimeInstant>& issueTime,
			const std::optional<TimeExtent>& executionTime,
			const std::optional<std::string>& sender,
			const std::optional<std::string>& currentStatus,
			const Data::DataBlockMixed& parameters,
			const std::optional<std::vector<Link>>& links)
			: id(id), controlStreamId(controlStreamId), samplingFeatureId(samplingFeatureId),
			procedureLink(procedureLink), issueTime(issueTime), executionTime(executionTime),
			sender(sender), currentStatus(currentStatus), parameters(parameters), links(links) {
		}

		nlohmann::ordered_json toJson() const {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// Local ID of the command.
		/// </summary>
		const std::optional<std::string>& getId() const { return id; }
		/// <summary>
		/// Local ID of the control stream that the command is part of.
		/// </summary>
		const std::optional<std::string>& getControlStreamId() const { return controlStreamId; }
		/// <summary>
		/// Local ID of the sampling feature that is the target of the command.
		/// </summary>
		const std::optional<std::string>& getSamplingFeatureId() const { return samplingFeatureId; }
		/// <summary>
		/// Link to the procedure/method used to process the command.
		/// </summary>
		const std::optional<Link>& getProcedureLink() const { return procedureLink; }
		/// <summary>
		/// Time at which the command was issued.
		/// If omitted on creation, the server sets it to the time the request was received.
		/// </summary>
		const std::optional<TimeInstant>& getIssueTime() const { return issueTime; }
		/// <summary>
		/// Time period during which the command was executed.
		/// </summary>
		const std::optional<TimeExtent>& getExecutionTime() const { return executionTime; }
		/// <summary>
		/// Identifier of the person or entity who submitted the command.
		/// </summary>
		const std::optional<std::string>& getSender() const { return sender; }
		/// <summary>
		/// Current status of the command.
		/// </summary>
		const std::optional<std::string>& getCurrentStatus() const { return currentStatus; }
		/// <summary>
		/// Command parameters.
		/// Must be valid according to the schema provided in the control stream metadata
		/// </summary>
		const Data::DataBlockMixed& getParameters() const { return parameters; }
		/// <summary>
		/// Links to related resources.
		/// </summary>
		const std::optional<std::vector<Link>>& getLinks() const { return links; }

		friend void from_json(const nlohmann::ordered_json& j, Command& v);
		friend void to_json(nlohmann::ordered_json& j, const Command& v);

		friend std::ostream& operator<<(std::ostream& os, const Command& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			os << j.dump(2);
			return os;
		}
	};

	inline void from_json(const nlohmann::ordered_json& j, Command& v) {
		v.id = (j.contains("id") && !j["id"].is_null()) ? std::optional<std::string>{ j["id"].get<std::string>() } : std::optional<std::string>{};
		v.controlStreamId = (j.contains("controlstream@id") && !j["controlstream@id"].is_null()) ? std::optional<std::string>{ j["controlstream@id"].get<std::string>() } : std::optional<std::string>{};
		v.samplingFeatureId = (j.contains("samplingFeature@id") && !j["samplingFeature@id"].is_null()) ? std::optional<std::string>{ j["samplingFeature@id"].get<std::string>() } : std::optional<std::string>{};
		v.procedureLink = j.value("procedure@link", std::optional<Link>{});
		v.issueTime = j.value("issueTime", std::optional<TimeInstant>{});
		v.executionTime = j.value("executionTime", std::optional<TimeExtent>{});
		v.sender = j.value("sender", std::optional<std::string>{});
		v.currentStatus = j.value("currentStatus", std::optional<std::string>{});
		v.parameters = (j.contains("parameters") && !j["parameters"].is_null()) ? j["parameters"].get<Data::DataBlockMixed>() : Data::DataBlockMixed{};
		v.links = j.value("links", std::optional<std::vector<Link>>{});
	}

	inline void to_json(nlohmann::ordered_json& j, const Command& v) {
		j = nlohmann::ordered_json::object();

		if (v.id) j["id"] = v.id.value();
		if (v.controlStreamId) j["controlstream@id"] = v.controlStreamId.value();
		if (v.samplingFeatureId) j["samplingFeature@id"] = v.samplingFeatureId.value();
		if (v.procedureLink) j["procedure@link"] = v.procedureLink.value();
		if (v.issueTime) j["issueTime"] = v.issueTime.value();
		if (v.executionTime) j["executionTime"] = v.executionTime.value();
		if (v.sender) j["sender"] = v.sender.value();
		if (v.currentStatus) j["currentStatus"] = v.currentStatus.value();
		j["parameters"] = v.parameters;
		if (v.links) j["links"] = v.links.value();
	}
}