#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <vector>
#include <utility>

#include "Observation.h"
#include "Link.h"
#include "TimeInstant.h"
#include "DataModels/Data/DataBlockMixed.h"
#include "ValidationException.h"

namespace ConnectedSystemsAPI::DataModels {
	/// <summary>
	/// Builder for Observation instances following the fluent builder pattern.
	/// This class is NOT thread-safe. Each thread should use its own builder instance.
	/// </summary>
	class ObservationBuilder {
	private:
		std::optional<std::string> id;
		std::optional<std::string> dataStreamId;
		std::optional<std::string> samplingFeatureId;
		std::optional<Link> procedureLink;
		std::optional<TimeInstant> phenomenonTime;
		std::optional<TimeInstant> resultTime;
		std::optional<Data::DataBlockMixed> result;
		std::optional<Link> resultLink;
		std::optional<std::vector<Link>> links;

	public:
		ObservationBuilder() = default;

		/// <summary>
		/// Create a builder initialized with values from an existing Observation.
		/// </summary>
		explicit ObservationBuilder(const Observation& obs) { withObservation(obs); }

		ObservationBuilder(const ObservationBuilder& other) = default;
		ObservationBuilder& operator=(const ObservationBuilder& other) = default;
		ObservationBuilder(ObservationBuilder&&) noexcept = default;
		ObservationBuilder& operator=(ObservationBuilder&&) noexcept = default;
		~ObservationBuilder() = default;

		/// <summary>
		/// Static factory method to create a builder from an existing Observation.
		/// </summary>
		static ObservationBuilder from(const Observation& obs) {
			return ObservationBuilder(obs);
		}

		/// <summary>
		/// Initialize the builder with values from an existing Observation instance.
		/// Note: This will perform a deep copy of all fields; use before modifying any fields or they will be overwritten.
		/// </summary>
		ObservationBuilder& withObservation(const Observation& obs) {
			id = obs.getId();
			dataStreamId = obs.getDataStreamId();
			samplingFeatureId = obs.getSamplingFeatureId();
			procedureLink = obs.getProcedureLink();
			phenomenonTime = obs.getPhenomenonTime();
			resultTime = obs.getResultTime();
			result = obs.getResult();
			resultLink = obs.getResultLink();
			links = obs.getLinks();
			return *this;
		}

		/// <summary>
		/// Reset the builder to its default state, clearing all fields.
		/// </summary>
		ObservationBuilder& reset() noexcept {
			id.reset();
			dataStreamId.reset();
			samplingFeatureId.reset();
			procedureLink.reset();
			phenomenonTime.reset();
			resultTime.reset();
			result.reset();
			resultLink.reset();
			links.reset();
			return *this;
		}

		/// <summary>
		/// Local ID of the observation.
		/// </summary>
		ObservationBuilder& withId(std::string_view v) noexcept { id = std::string(v); return *this; }

		/// <summary>
		/// Local ID of the datastream that the observation is part of.
		/// </summary>
		ObservationBuilder& withDataStreamId(std::string_view v) noexcept { dataStreamId = std::string(v); return *this; }

		/// <summary>
		/// Local ID of the sampling feature that is the target of the observation.
		/// </summary>
		ObservationBuilder& withSamplingFeatureId(std::string_view v) noexcept { samplingFeatureId = std::string(v); return *this; }

		/// <summary>
		/// Link to the procedure/method used to make the observation.
		/// </summary>
		ObservationBuilder& withProcedureLink(const Link& v) noexcept { procedureLink = v; return *this; }
		/// <summary>
		/// Link to the procedure/method used to make the observation.
		/// </summary>
		ObservationBuilder& withProcedureLink(Link&& v) noexcept { procedureLink = std::move(v); return *this; }

		/// <summary>
		/// Time at which the observation result is a valid estimate of the sampling feature property(ies).
		/// Defaults to the same value as resultTime.
		/// </summary>
		ObservationBuilder& withPhenomenonTime(const TimeInstant& v) noexcept { phenomenonTime = v; return *this; }
		/// <summary>
		/// Time at which the observation result is a valid estimate of the sampling feature property(ies).
		/// Defaults to the same value as resultTime.
		/// </summary>
		ObservationBuilder& withPhenomenonTime(TimeInstant&& v) noexcept { phenomenonTime = std::move(v); return *this; }

		/// <summary>
		/// Time at which the observation result was generated.
		/// </summary>
		ObservationBuilder& withResultTime(const TimeInstant& v) noexcept { resultTime = v; return *this; }
		/// <summary>
		/// Time at which the observation result was generated.
		/// </summary>
		ObservationBuilder& withResultTime(TimeInstant&& v) noexcept { resultTime = std::move(v); return *this; }

		/// <summary>
		/// Result of the observation.
		/// Must be valid according to the result schema provided in the datastream metadata.
		/// </summary>
		ObservationBuilder& withResult(const Data::DataBlockMixed& v) { result = v; return *this; }
		/// <summary>
		/// Result of the observation.
		/// Must be valid according to the result schema provided in the datastream metadata.
		/// </summary>
		ObservationBuilder& withResult(Data::DataBlockMixed&& v) { result = std::move(v); return *this; }

		/// <summary>
		/// Link to external result data (e.g., large raster dataset served by a tiling service)
		/// </summary>
		ObservationBuilder& withResultLink(const Link& v) noexcept { resultLink = v; return *this; }
		/// <summary>
		/// Link to external result data (e.g., large raster dataset served by a tiling service)
		/// </summary>
		ObservationBuilder& withResultLink(Link&& v) noexcept { resultLink = std::move(v); return *this; }

		/// <summary>
		/// Links to related resources.
		/// </summary>
		ObservationBuilder& withLinks(const std::vector<Link>& v) noexcept { links = v; return *this; }
		/// <summary>
		/// Links to related resources.
		/// </summary>
		ObservationBuilder& withLinks(std::vector<Link>&& v) noexcept { links = std::move(v); return *this; }

		/// <summary>
		/// Add a link to related resources.
		/// </summary>
		ObservationBuilder& addLink(const Link& l) {
			if (!links) links = std::vector<Link>{};
			links->push_back(l);
			return *this;
		}

		/// <summary>
		/// Add a link to related resources.
		/// </summary>
		ObservationBuilder& addLink(Link&& l) {
			if (!links) links = std::vector<Link>{};
			links->emplace_back(std::move(l));
			return *this;
		}

		/// <summary>
		/// Clear all links.
		/// </summary>
		ObservationBuilder& clearLinks() noexcept {
			links.reset();
			return *this;
		}

		// Getters for inspection
		const std::optional<std::string>& getId() const noexcept { return id; }
		const std::optional<std::string>& getDataStreamId() const noexcept { return dataStreamId; }
		const std::optional<std::string>& getSamplingFeatureId() const noexcept { return samplingFeatureId; }
		const std::optional<Link>& getProcedureLink() const noexcept { return procedureLink; }
		const std::optional<TimeInstant>& getPhenomenonTime() const noexcept { return phenomenonTime; }
		const std::optional<TimeInstant>& getResultTime() const noexcept { return resultTime; }
		const std::optional<Data::DataBlockMixed>& getResult() const noexcept { return result; }
		const std::optional<Link>& getResultLink() const noexcept { return resultLink; }
		const std::optional<std::vector<Link>>& getLinks() const noexcept { return links; }

		/// <summary>
		/// Check if the current builder state is valid. Optionally populate a vector with validation errors.
		/// </summary>
		/// <param name="outErrors">Optional pointer to a vector to populate with validation errors. If nullptr, only validation result is returned.</param>
		/// <returns>True if validation passes, false otherwise.</returns>
		bool isValid(std::vector<ConnectedSystemsAPI::ValidationException::ValidationError>* outErrors = nullptr) const {
			std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors;

			if (!result.has_value())
				errors.emplace_back("result", "Field is required");

			bool valid = errors.empty();

			if (outErrors)
				*outErrors = std::move(errors);

			return valid;
		}

		/// <summary>
		/// Build an Observation instance from the current builder state.
		/// If either phenomenonTime or resultTime is set and the other is not, the missing one will default to the provided value.
		/// </summary>
		/// <returns>A new Observation instance.</returns>
		/// <exception cref="ValidationException">Thrown if validation fails.</exception>
		Observation build() const {
			std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors;
			if (!isValid(&errors)) {
				throw ConnectedSystemsAPI::ValidationException("ObservationBuilder", errors);
			}

			// If either phenomenonTime or resultTime is set and the other is not, set the missing one to the same value
			std::optional<TimeInstant> localPhenomenonTime = phenomenonTime;
			std::optional<TimeInstant> localResultTime = resultTime;

			if (localPhenomenonTime && !localResultTime) {
				localResultTime = localPhenomenonTime;
			}
			else if (localResultTime && !localPhenomenonTime) {
				localPhenomenonTime = localResultTime;
			}

			return Observation(
				id,
				dataStreamId,
				samplingFeatureId,
				procedureLink,
				localPhenomenonTime,
				localResultTime,
				result.value(),
				resultLink,
				links
			);
		}
	};
}