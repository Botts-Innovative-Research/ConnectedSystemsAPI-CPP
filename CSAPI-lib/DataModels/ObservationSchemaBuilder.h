#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <memory>
#include <utility>

#include "ObservationSchema.h"
#include "Link.h"
#include "Component/DataRecord.h"
#include "Component/DataComponent.h"
#include "ValidationException.h"

namespace ConnectedSystemsAPI::DataModels {
	/// <summary>
	/// Builder for ObservationSchema instances following the fluent builder pattern.
	/// This class is NOT thread-safe. Each thread should use its own builder instance.
	/// </summary>
	class ObservationSchemaBuilder {
	private:
		std::optional<std::string> observationFormat;
		std::optional<Component::DataRecord> parametersSchema;
		std::unique_ptr<Component::DataComponent> resultSchema;
		std::optional<Link> resultLink;

		static std::unique_ptr<Component::DataComponent> cloneResultSchema(const Component::DataComponent* source) {
			if (!source) return nullptr;
			nlohmann::ordered_json j = source->toJson();
			return Component::DataComponentRegistry::createDataComponent(j);
		}

	public:
		ObservationSchemaBuilder() = default;

		/// <summary>
		/// Create a builder initialized with values from an existing ObservationSchema.
		/// </summary>
		explicit ObservationSchemaBuilder(const ObservationSchema& os) { withObservationSchema(os); }

		ObservationSchemaBuilder(const ObservationSchemaBuilder& other)
			: observationFormat(other.observationFormat),
			parametersSchema(other.parametersSchema),
			resultSchema(cloneResultSchema(other.resultSchema.get())),
			resultLink(other.resultLink)
		{
		}

		ObservationSchemaBuilder& operator=(const ObservationSchemaBuilder& other) {
			if (this != &other) {
				observationFormat = other.observationFormat;
				parametersSchema = other.parametersSchema;
				resultLink = other.resultLink;
				resultSchema = cloneResultSchema(other.resultSchema.get());
			}
			return *this;
		}

		ObservationSchemaBuilder(ObservationSchemaBuilder&&) noexcept = default;
		ObservationSchemaBuilder& operator=(ObservationSchemaBuilder&&) noexcept = default;
		~ObservationSchemaBuilder() = default;

		/// <summary>
		/// Static factory method to create a builder from an existing ObservationSchema.
		/// </summary>
		static ObservationSchemaBuilder from(const ObservationSchema& os) {
			return ObservationSchemaBuilder(os);
		}

		/// <summary>
		/// Initialize the builder with values from an existing ObservationSchema instance.
		/// Note: This will perform a deep copy of all fields; use before modifying any fields or they will be overwritten.
		/// </summary>
		ObservationSchemaBuilder& withObservationSchema(const ObservationSchema& os) {
			observationFormat = os.getObservationFormat();
			parametersSchema = os.getParametersSchema();
			resultSchema = cloneResultSchema(os.getResultSchema());
			resultLink = os.getResultLink();
			return *this;
		}

		/// <summary>
		/// Reset the builder to its default state, clearing all fields.
		/// </summary>
		ObservationSchemaBuilder& reset() noexcept {
			observationFormat.reset();
			parametersSchema.reset();
			resultSchema.reset();
			resultLink.reset();
			return *this;
		}

		/// <summary>
		/// Encoding format of the observation.
		/// </summary>
		ObservationSchemaBuilder& withObservationFormat(std::string_view v) noexcept {
			observationFormat = std::string(v);
			return *this;
		}

		/// <summary>
		/// Record schema for the observation parameters property. If omitted, parameters are not included in the datastream.
		/// </summary>
		ObservationSchemaBuilder& withParametersSchema(const Component::DataRecord& v) {
			parametersSchema = v;
			return *this;
		}

		/// <summary>
		/// Record schema for the observation parameters property. If omitted, parameters are not included in the datastream.
		/// </summary>
		ObservationSchemaBuilder& withParametersSchema(Component::DataRecord&& v) {
			parametersSchema = std::move(v);
			return *this;
		}

		/// <summary>
		/// Encoding information in case the result is provided out-of-band via the result@link property.
		/// </summary>
		ObservationSchemaBuilder& withResultLink(const Link& v) noexcept {
			resultLink = v;
			return *this;
		}

		/// <summary>
		/// Encoding information in case the result is provided out-of-band via the result@link property.
		/// </summary>
		ObservationSchemaBuilder& withResultLink(Link&& v) noexcept {
			resultLink = std::move(v);
			return *this;
		}

		/// <summary>
		/// Set the result schema by transferring ownership of a unique_ptr.
		/// Schema for the observation result property describing the observed properties and their structure.
		/// </summary>
		ObservationSchemaBuilder& withResultSchema(std::unique_ptr<Component::DataComponent> v) noexcept {
			resultSchema = std::move(v);
			return *this;
		}

		/// <summary>
		/// Set the result schema by copying from an existing DataComponent instance.
		/// Schema for the observation result property describing the observed properties and their structure.
		/// Note: This performs a deep copy via JSON serialization.
		/// </summary>
		ObservationSchemaBuilder& withResultSchema(const Component::DataComponent& v) {
			resultSchema = cloneResultSchema(&v);
			return *this;
		}

		/// <summary>
		/// Clear the parameters schema.
		/// </summary>
		ObservationSchemaBuilder& clearParametersSchema() noexcept {
			parametersSchema.reset();
			return *this;
		}

		/// <summary>
		/// Clear the result link.
		/// </summary>
		ObservationSchemaBuilder& clearResultLink() noexcept {
			resultLink.reset();
			return *this;
		}

		const std::optional<std::string>& getObservationFormat() const noexcept { return observationFormat; }
		const std::optional<Component::DataRecord>& getParametersSchema() const noexcept { return parametersSchema; }
		const Component::DataComponent* getResultSchema() const noexcept { return resultSchema.get(); }
		const std::optional<Link>& getResultLink() const noexcept { return resultLink; }

		/// <summary>
		/// Check if the current builder state is valid. Optionally populate a vector with validation errors.
		/// </summary>
		/// <param name="outErrors">Optional pointer to a vector to populate with validation errors. If nullptr, only validation result is returned.</param>
		/// <returns>True if validation passes, false otherwise.</returns>
		bool isValid(std::vector<ConnectedSystemsAPI::ValidationException::ValidationError>* outErrors = nullptr) const {
			std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors;

			if (!observationFormat || observationFormat->empty())
				errors.emplace_back("observationFormat", "Field is required and cannot be empty");

			if (!resultSchema)
				errors.emplace_back("resultSchema", "Field is required");

			bool valid = errors.empty();

			if (outErrors)
				*outErrors = std::move(errors);

			return valid;
		}

		/// <summary>
		/// Build an ObservationSchema instance from the current builder state.
		/// Note: This method clones the resultSchema, so the builder can be reused for multiple builds.
		/// </summary>
		/// <returns>A new ObservationSchema instance.</returns>
		/// <exception cref="ValidationException">Thrown if validation fails.</exception>
		ObservationSchema build() const {
			if (std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors; !isValid(&errors)) {
				throw ConnectedSystemsAPI::ValidationException("ObservationSchemaBuilder", errors);
			}

			return ObservationSchema(
				observationFormat.value(),
				parametersSchema,
				cloneResultSchema(resultSchema.get()),
				resultLink
			);
		}
	};
}