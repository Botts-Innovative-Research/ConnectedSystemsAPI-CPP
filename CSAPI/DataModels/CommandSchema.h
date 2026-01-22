#pragma once

#include <string>
#include <memory>
#include <ostream>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Component/DataComponent.h"
#include "Component/DataComponentRegistry.h"

namespace ConnectedSystemsAPI::DataModels {
	class CommandSchema;
	void to_json(nlohmann::ordered_json& j, const CommandSchema& v);

	class CommandSchema {
	private:
		std::string commandFormat;
		std::unique_ptr<Component::DataComponent> parametersSchema;
		std::unique_ptr<Component::DataComponent> resultSchema;
		std::unique_ptr<Component::DataComponent> feasibilityResultSchema;

	public:
		CommandSchema() = default;
		CommandSchema(const std::string& commandFormat,
			std::unique_ptr<Component::DataComponent> parametersSchema,
			std::unique_ptr<Component::DataComponent> resultSchema,
			std::unique_ptr<Component::DataComponent> feasibilityResultSchema = nullptr)
			: commandFormat(commandFormat),
			parametersSchema(std::move(parametersSchema)),
			resultSchema(std::move(resultSchema)),
			feasibilityResultSchema(std::move(feasibilityResultSchema)) {
		}

		CommandSchema(const CommandSchema&) = delete;
		CommandSchema& operator=(const CommandSchema&) = delete;
		CommandSchema(CommandSchema&&) noexcept = default;
		CommandSchema& operator=(CommandSchema&&) noexcept = default;
		~CommandSchema() = default;

		nlohmann::ordered_json toJson() const {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// Encoding format of the command.
		/// </summary>
		const std::string& getCommandFormat() const { return commandFormat; }
		/// <summary>
		/// Record schema for the command parameters property. If omitted, parameters are not included in the datastream.
		/// </summary>
		const Component::DataComponent* getParametersSchema() const { return parametersSchema.get(); }
		/// <summary>
		/// Schema for the command result property.
		/// this describes the observed properties included in the result
		/// and how they are structured if the result is a record, a vector quantity or a coverage.
		/// </summary>
		const Component::DataComponent* getResultSchema() const { return resultSchema.get(); }
		/// <summary>
		/// Schema for the feasibility result property.
		/// This describes the structure of the feasibility assessment result.
		/// </summary>
		const Component::DataComponent* getFeasibilityResultSchema() const { return feasibilityResultSchema.get(); }

		friend void from_json(const nlohmann::json& j, CommandSchema& v);
		friend void to_json(nlohmann::ordered_json& j, const CommandSchema& v);

		friend std::ostream& operator<<(std::ostream& os, const CommandSchema& v) {
			return os << v.toJson().dump(2);
		}
	};

	inline void from_json(const nlohmann::json& j, CommandSchema& v) {
		v.commandFormat = j.at("commandFormat").get<std::string>();
		v.parametersSchema = Component::DataComponentRegistry::createDataComponent(j.at("paramsSchema"));
		if (j.contains("resultSchema"))
			v.resultSchema = Component::DataComponentRegistry::createDataComponent(j.at("resultSchema"));
		if (j.contains("feasibilityResultSchema"))
			v.feasibilityResultSchema = Component::DataComponentRegistry::createDataComponent(j.at("feasibilityResultSchema"));
	}

	inline void to_json(nlohmann::ordered_json& j, const CommandSchema& v) {
		j = nlohmann::ordered_json::object();

		j["commandFormat"] = v.commandFormat;
		if (v.parametersSchema) j["paramsSchema"] = v.getParametersSchema()->toJson();
		if (v.resultSchema) j["resultSchema"] = v.getResultSchema()->toJson();
		if (v.feasibilityResultSchema) j["feasibilityResultSchema"] = v.getFeasibilityResultSchema()->toJson();
	}
}