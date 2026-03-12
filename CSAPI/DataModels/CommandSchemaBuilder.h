#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <memory>
#include <utility>
#include <vector>
#include <nlohmann/json_fwd.hpp>

#include "CommandSchema.h"
#include "Component/DataComponent.h"
#include "Component/DataComponentRegistry.h"
#include "ValidationException.h"

namespace ConnectedSystemsAPI::DataModels {
	/// <summary>
	/// Builder for CommandSchema instances following the fluent builder pattern.
	/// This class is NOT thread-safe. Each thread should use its own builder instance.
	/// </summary>
	class CommandSchemaBuilder {
	private:
		std::optional<std::string> commandFormat;
		std::unique_ptr<Component::DataComponent> parametersSchema;
		std::unique_ptr<Component::DataComponent> resultSchema;
		std::unique_ptr<Component::DataComponent> feasibilityResultSchema;

		static std::unique_ptr<Component::DataComponent> cloneSchema(const Component::DataComponent* source) {
			if (!source) return nullptr;
			nlohmann::ordered_json j = source->toJson();
			return Component::DataComponentRegistry::createDataComponent(j);
		}

	public:
		CommandSchemaBuilder() = default;

		/// <summary>
		/// Create a builder initialized with values from an existing CommandSchema.
		/// </summary>
		explicit CommandSchemaBuilder(const CommandSchema& cs) { withCommandSchema(cs); }

		CommandSchemaBuilder(const CommandSchemaBuilder& other)
			: commandFormat(other.commandFormat),
			parametersSchema(cloneSchema(other.parametersSchema.get())),
			resultSchema(cloneSchema(other.resultSchema.get())),
			feasibilityResultSchema(cloneSchema(other.feasibilityResultSchema.get()))
		{
		}

		CommandSchemaBuilder& operator=(const CommandSchemaBuilder& other) {
			if (this != &other) {
				commandFormat = other.commandFormat;
				parametersSchema = cloneSchema(other.parametersSchema.get());
				resultSchema = cloneSchema(other.resultSchema.get());
				feasibilityResultSchema = cloneSchema(other.feasibilityResultSchema.get());
			}
			return *this;
		}

		CommandSchemaBuilder(CommandSchemaBuilder&&) noexcept = default;
		CommandSchemaBuilder& operator=(CommandSchemaBuilder&&) noexcept = default;
		~CommandSchemaBuilder() = default;

		/// <summary>
		/// Static factory method to create a builder from an existing CommandSchema.
		/// </summary>
		static CommandSchemaBuilder from(const CommandSchema& cs) {
			return CommandSchemaBuilder(cs);
		}

		/// <summary>
		/// Initialize the builder with values from an existing CommandSchema instance.
		/// Note: This will perform a deep copy of all fields; use before modifying any fields or they will be overwritten.
		/// </summary>
		CommandSchemaBuilder& withCommandSchema(const CommandSchema& cs) {
			commandFormat = cs.getCommandFormat();
			parametersSchema = cloneSchema(cs.getParametersSchema());
			resultSchema = cloneSchema(cs.getResultSchema());
			feasibilityResultSchema = cloneSchema(cs.getFeasibilityResultSchema());
			return *this;
		}

		/// <summary>
		/// Reset the builder to its default state, clearing all fields.
		/// </summary>
		CommandSchemaBuilder& reset() noexcept {
			commandFormat.reset();
			parametersSchema.reset();
			resultSchema.reset();
			feasibilityResultSchema.reset();
			return *this;
		}

		/// <summary>
		/// Encoding format of the command.
		/// </summary>
		CommandSchemaBuilder& withCommandFormat(std::string_view v) noexcept {
			commandFormat = std::string(v);
			return *this;
		}

		/// <summary>
		/// Set the parameters schema by transferring ownership of a unique_ptr.
		/// Schema for the command parameters property. If omitted, parameters are not included in the control stream.
		/// </summary>
		CommandSchemaBuilder& withParametersSchema(std::unique_ptr<Component::DataComponent> v) noexcept {
			parametersSchema = std::move(v);
			return *this;
		}

		/// <summary>
		/// Set the parameters schema by copying from an existing DataComponent instance.
		/// Schema for the command parameters property. If omitted, parameters are not included in the control stream.
		/// Note: This performs a deep copy via JSON serialization.
		/// </summary>
		CommandSchemaBuilder& withParametersSchema(const Component::DataComponent& v) {
			parametersSchema = cloneSchema(&v);
			return *this;
		}

		/// <summary>
		/// Set the result schema by transferring ownership of a unique_ptr.
		/// Schema for the command result property describing the observed properties and their structure.
		/// </summary>
		CommandSchemaBuilder& withResultSchema(std::unique_ptr<Component::DataComponent> v) noexcept {
			resultSchema = std::move(v);
			return *this;
		}

		/// <summary>
		/// Set the result schema by copying from an existing DataComponent instance.
		/// Schema for the command result property describing the observed properties and their structure.
		/// Note: This performs a deep copy via JSON serialization.
		/// </summary>
		CommandSchemaBuilder& withResultSchema(const Component::DataComponent& v) {
			resultSchema = cloneSchema(&v);
			return *this;
		}

		/// <summary>
		/// Set the feasibility result schema by transferring ownership of a unique_ptr.
		/// Schema for the feasibility result property describing the structure of the feasibility assessment result.
		/// </summary>
		CommandSchemaBuilder& withFeasibilityResultSchema(std::unique_ptr<Component::DataComponent> v) noexcept {
			feasibilityResultSchema = std::move(v);
			return *this;
		}

		/// <summary>
		/// Set the feasibility result schema by copying from an existing DataComponent instance.
		/// Schema for the feasibility result property describing the structure of the feasibility assessment result.
		/// Note: This performs a deep copy via JSON serialization.
		/// </summary>
		CommandSchemaBuilder& withFeasibilityResultSchema(const Component::DataComponent& v) {
			feasibilityResultSchema = cloneSchema(&v);
			return *this;
		}

		/// <summary>
		/// Clear the parameters schema.
		/// </summary>
		CommandSchemaBuilder& clearParametersSchema() noexcept {
			parametersSchema.reset();
			return *this;
		}

		/// <summary>
		/// Clear the result schema.
		/// </summary>
		CommandSchemaBuilder& clearResultSchema() noexcept {
			resultSchema.reset();
			return *this;
		}

		/// <summary>
		/// Clear the feasibility result schema.
		/// </summary>
		CommandSchemaBuilder& clearFeasibilityResultSchema() noexcept {
			feasibilityResultSchema.reset();
			return *this;
		}

		const std::optional<std::string>& getCommandFormat() const noexcept { return commandFormat; }
		const Component::DataComponent* getParametersSchema() const noexcept { return parametersSchema.get(); }
		const Component::DataComponent* getResultSchema() const noexcept { return resultSchema.get(); }
		const Component::DataComponent* getFeasibilityResultSchema() const noexcept { return feasibilityResultSchema.get(); }

		/// <summary>
		/// Check if the current builder state is valid. Optionally populate a vector with validation errors.
		/// </summary>
		/// <param name="outErrors">Optional pointer to a vector to populate with validation errors. If nullptr, only validation result is returned.</param>
		/// <returns>True if validation passes, false otherwise.</returns>
		bool isValid(std::vector<ConnectedSystemsAPI::ValidationException::ValidationError>* outErrors = nullptr) const {
			std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors;

			if (!commandFormat || commandFormat->empty())
				errors.emplace_back("commandFormat", "Field is required and cannot be empty");

			if (!parametersSchema)
				errors.emplace_back("parametersSchema", "Field is required");

			bool valid = errors.empty();

			if (outErrors)
				*outErrors = std::move(errors);

			return valid;
		}

		/// <summary>
		/// Build a CommandSchema instance from the current builder state.
		/// Note: This method clones the schemas, so the builder can be reused for multiple builds.
		/// </summary>
		/// <returns>A new CommandSchema instance.</returns>
		/// <exception cref="ValidationException">Thrown if validation fails.</exception>
		CommandSchema build() const {
			if (std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors; !isValid(&errors)) {
				throw ConnectedSystemsAPI::ValidationException("CommandSchemaBuilder", errors);
			}

			return CommandSchema(
				commandFormat.value(),
				cloneSchema(parametersSchema.get()),
				cloneSchema(resultSchema.get()),
				cloneSchema(feasibilityResultSchema.get())
			);
		}
	};
}
