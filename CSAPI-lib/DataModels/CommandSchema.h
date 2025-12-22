#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Component/DataComponent.h"
#include "Component/DataComponentRegistry.h"
#include "Component/DataRecord.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class CommandSchema {
		private:
			std::string commandFormat;
			std::unique_ptr<Component::DataComponent> parametersSchema;
			std::unique_ptr<Component::DataComponent> resultSchema;
			//todo: feasibilityResultSchema

		public:
			CommandSchema() = default;
			CommandSchema(const std::string& commandFormat,
				std::unique_ptr<Component::DataComponent> parametersSchema,
				std::unique_ptr<Component::DataComponent> resultSchema)
				: commandFormat(commandFormat), resultSchema(std::move(resultSchema)) {
			}

			CommandSchema(const CommandSchema&) = delete;
			CommandSchema& operator=(const CommandSchema&) = delete;
			CommandSchema(CommandSchema&&) noexcept = default;
			CommandSchema& operator=(CommandSchema&&) noexcept = default;

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

			friend void from_json(const nlohmann::json& j, CommandSchema& v);
			friend void to_json(nlohmann::ordered_json& j, const CommandSchema& v);
			friend std::ostream& operator<<(std::ostream& os, const CommandSchema& v);
		};

		inline void from_json(const nlohmann::json& j, CommandSchema& v) {
			// Print the json for debugging
			std::cout << "Deserializing CommandSchema from JSON: " << j.dump(2) << std::endl;
			v.commandFormat = j.at("commandFormat").get<std::string>();
			v.parametersSchema = Component::DataComponentRegistry::createDataComponent(j.at("paramsSchema"));
			if (j.contains("resultSchema"))
				v.resultSchema = Component::DataComponentRegistry::createDataComponent(j.at("resultSchema"));
		}

		inline void to_json(nlohmann::ordered_json& j, const CommandSchema& v) {
			j = nlohmann::ordered_json::object();

			j["commandFormat"] = v.commandFormat;
			if (v.parametersSchema) j["paramsSchema"] = v.getParametersSchema()->toJson();
			if (v.resultSchema) j["resultSchema"] = v.getResultSchema()->toJson();
		}

		inline std::ostream& operator<<(std::ostream& os, const CommandSchema& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			return os << j.dump(2);
		}
	}
}