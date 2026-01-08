#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <utility>

#include "Properties.h"
#include "TimeExtent.h"
#include "ValidationException.h"

namespace ConnectedSystemsAPI::DataModels {
	/// <summary>
	/// Builder for Properties instances following the fluent builder pattern.
	/// This class is NOT thread-safe. Each thread should use its own builder instance.
	/// </summary>
	class PropertiesBuilder {
	private:
		std::optional<std::string> featureType;
		std::optional<std::string> uid;
		std::optional<std::string> name;
		std::optional<std::string> description;
		std::optional<std::string> assetType;
		std::optional<TimeExtent> validTime;

	public:
		PropertiesBuilder() = default;

		/// <summary>
		/// Create a builder initialized with values from existing Properties.
		/// </summary>
		explicit PropertiesBuilder(const Properties& props) { withProperties(props); }

		PropertiesBuilder(const PropertiesBuilder& other) = default;
		PropertiesBuilder& operator=(const PropertiesBuilder& other) = default;
		PropertiesBuilder(PropertiesBuilder&&) noexcept = default;
		PropertiesBuilder& operator=(PropertiesBuilder&&) noexcept = default;
		~PropertiesBuilder() = default;

		/// <summary>
		/// Static factory method to create a builder from existing Properties.
		/// </summary>
		static PropertiesBuilder from(const Properties& props) {
			return PropertiesBuilder(props);
		}

		/// <summary>
		/// Initialize the builder with values from an existing Properties instance.
		/// Note: This will perform a deep copy of all fields; use before modifying any fields or they will be overwritten.
		/// </summary>
		PropertiesBuilder& withProperties(const Properties& props) {
			featureType = props.getFeatureType();
			uid = props.getUid();
			name = props.getName();
			description = props.getDescription();
			assetType = props.getAssetType();
			validTime = props.getValidTime();
			return *this;
		}

		/// <summary>
		/// Reset the builder to its default state, clearing all fields.
		/// </summary>
		PropertiesBuilder& reset() noexcept {
			featureType.reset();
			uid.reset();
			name.reset();
			description.reset();
			assetType.reset();
			validTime.reset();
			return *this;
		}

		// Setters for string parameters
		/// <summary>
		/// Identifier of the feature, either a URI, a CURIE, or a simple token.
		/// </summary>
		PropertiesBuilder& withFeatureType(std::string_view v) noexcept { featureType = std::string(v); return *this; }

		/// <summary>
		/// Globally unique identifier of the feature.
		/// </summary>
		PropertiesBuilder& withUid(std::string_view v) noexcept { uid = std::string(v); return *this; }

		/// <summary>
		/// Human-readable name of the feature.
		/// </summary>
		PropertiesBuilder& withName(std::string_view v) noexcept { name = std::string(v); return *this; }

		/// <summary>
		/// Human-readable description of the feature.
		/// </summary>
		PropertiesBuilder& withDescription(std::string_view v) noexcept { description = std::string(v); return *this; }

		/// <summary>
		/// Type of asset represented by this system.
		/// </summary>
		PropertiesBuilder& withAssetType(std::string_view v) noexcept { assetType = std::string(v); return *this; }

		// Setters with copy and move semantics for non-string types
		/// <summary>
		/// Time period during which the system description is valid.
		/// </summary>
		PropertiesBuilder& withValidTime(const TimeExtent& v) noexcept { validTime = v; return *this; }
		/// <summary>
		/// Time period during which the system description is valid.
		/// </summary>
		PropertiesBuilder& withValidTime(TimeExtent&& v) noexcept { validTime = std::move(v); return *this; }

		// Clear methods
		/// <summary>
		/// Clear the valid time.
		/// </summary>
		PropertiesBuilder& clearValidTime() noexcept {
			validTime.reset();
			return *this;
		}

		// Getters for inspection
		const std::optional<std::string>& getFeatureType() const noexcept { return featureType; }
		const std::optional<std::string>& getUid() const noexcept { return uid; }
		const std::optional<std::string>& getName() const noexcept { return name; }
		const std::optional<std::string>& getDescription() const noexcept { return description; }
		const std::optional<std::string>& getAssetType() const noexcept { return assetType; }
		const std::optional<TimeExtent>& getValidTime() const noexcept { return validTime; }

		/// <summary>
		/// Check if the current builder state is valid. Optionally populate a vector with validation errors.
		/// </summary>
		/// <param name="outErrors">Optional pointer to a vector to populate with validation errors. If nullptr, only validation result is returned.</param>
		/// <returns>True if validation passes, false otherwise.</returns>
		bool isValid(std::vector<ConnectedSystemsAPI::ValidationException::ValidationError>* outErrors = nullptr) const {
			std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors;

			if (!featureType || featureType->empty())
				errors.emplace_back("featureType", "Field is required and cannot be empty");

			if (!uid || uid->empty())
				errors.emplace_back("uid", "Field is required and cannot be empty");

			if (!name || name->empty())
				errors.emplace_back("name", "Field is required and cannot be empty");

			bool valid = errors.empty();

			if (outErrors)
				*outErrors = std::move(errors);

			return valid;
		}

		/// <summary>
		/// Build a Properties instance from the current builder state.
		/// </summary>
		/// <returns>A new Properties instance.</returns>
		/// <exception cref="ValidationException">Thrown if validation fails.</exception>
		Properties build() const {
			if (std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors; !isValid(&errors)) {
				throw ConnectedSystemsAPI::ValidationException("PropertiesBuilder", errors);
			}

			return Properties(
				featureType.value(),
				uid.value(),
				name.value(),
				description.value_or(""),
				assetType.value_or(""),
				validTime
			);
		}
	};
}