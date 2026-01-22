#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include <utility>

#include "System.h"
#include "Link.h"
#include "Properties.h"
#include "ValidationException.h"

namespace ConnectedSystemsAPI::DataModels {
	/// <summary>
	/// Builder for System instances following the fluent builder pattern.
	/// This class is NOT thread-safe. Each thread should use its own builder instance.
	/// </summary>
	class SystemBuilder {
	private:
		std::optional<std::string> type;
		std::optional<std::string> id;
		std::optional<Properties> properties;
		std::optional<std::vector<double>> bbox;
		std::optional<std::vector<Link>> links;

	public:
		SystemBuilder() = default;

		/// <summary>
		/// Create a builder initialized with values from an existing System.
		/// </summary>
		explicit SystemBuilder(const System& sys) { withSystem(sys); }

		SystemBuilder(const SystemBuilder& other) = default;
		SystemBuilder& operator=(const SystemBuilder& other) = default;
		SystemBuilder(SystemBuilder&&) noexcept = default;
		SystemBuilder& operator=(SystemBuilder&&) noexcept = default;
		~SystemBuilder() = default;

		/// <summary>
		/// Static factory method to create a builder from an existing System.
		/// </summary>
		static SystemBuilder from(const System& sys) {
			return SystemBuilder(sys);
		}

		/// <summary>
		/// Initialize the builder with values from an existing System instance.
		/// Note: This will perform a deep copy of all fields; use before modifying any fields or they will be overwritten.
		/// </summary>
		SystemBuilder& withSystem(const System& sys) {
			type = sys.getType();
			id = sys.getId();
			properties = sys.getProperties();
			bbox = sys.getBbox();
			links = sys.getLinks();
			return *this;
		}

		/// <summary>
		/// Reset the builder to its default state, clearing all fields.
		/// </summary>
		SystemBuilder& reset() noexcept {
			type.reset();
			id.reset();
			properties.reset();
			bbox.reset();
			links.reset();
			return *this;
		}

		/// <summary>
		/// Type of the feature (typically "Feature").
		/// </summary>
		SystemBuilder& withType(std::string_view v) noexcept { type = std::string(v); return *this; }

		/// <summary>
		/// Local ID of the feature (ignored on create or update).
		/// </summary>
		SystemBuilder& withId(std::string_view v) noexcept { id = std::string(v); return *this; }

		/// <summary>
		/// Feature properties.
		/// </summary>
		SystemBuilder& withProperties(const Properties& v) { properties = v; return *this; }
		/// <summary>
		/// Feature properties.
		/// </summary>
		SystemBuilder& withProperties(Properties&& v) { properties = std::move(v); return *this; }

		/// <summary>
		/// Optional bounding box for the feature.
		/// </summary>
		SystemBuilder& withBbox(const std::vector<double>& v) noexcept { bbox = v; return *this; }
		/// <summary>
		/// Optional bounding box for the feature.
		/// </summary>
		SystemBuilder& withBbox(std::vector<double>&& v) noexcept { bbox = std::move(v); return *this; }

		/// <summary>
		/// Links to related resources.
		/// </summary>
		SystemBuilder& withLinks(const std::vector<Link>& v) noexcept { links = v; return *this; }
		/// <summary>
		/// Links to related resources.
		/// </summary>
		SystemBuilder& withLinks(std::vector<Link>&& v) noexcept { links = std::move(v); return *this; }

		/// <summary>
		/// Add a link to related resources.
		/// </summary>
		SystemBuilder& addLink(const Link& l) {
			if (!links) links = std::vector<Link>{};
			links->push_back(l);
			return *this;
		}

		/// <summary>
		/// Add a link to related resources.
		/// </summary>
		SystemBuilder& addLink(Link&& l) {
			if (!links) links = std::vector<Link>{};
			links->emplace_back(std::move(l));
			return *this;
		}

		/// <summary>
		/// Clear the bounding box.
		/// </summary>
		SystemBuilder& clearBbox() noexcept {
			bbox.reset();
			return *this;
		}

		/// <summary>
		/// Clear all links.
		/// </summary>
		SystemBuilder& clearLinks() noexcept {
			links.reset();
			return *this;
		}

		// Getters for inspection
		const std::optional<std::string>& getType() const noexcept { return type; }
		const std::optional<std::string>& getId() const noexcept { return id; }
		const std::optional<Properties>& getProperties() const noexcept { return properties; }
		const std::optional<std::vector<double>>& getBbox() const noexcept { return bbox; }
		const std::optional<std::vector<Link>>& getLinks() const noexcept { return links; }

		/// <summary>
		/// Check if the current builder state is valid. Optionally populate a vector with validation errors.
		/// </summary>
		/// <param name="outErrors">Optional pointer to a vector to populate with validation errors. If nullptr, only validation result is returned.</param>
		/// <returns>True if validation passes, false otherwise.</returns>
		bool isValid(std::vector<ConnectedSystemsAPI::ValidationException::ValidationError>* outErrors = nullptr) const {
			std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors;

			if (!properties.has_value())
				errors.emplace_back("properties", "Field is required");

			bool valid = errors.empty();

			if (outErrors)
				*outErrors = std::move(errors);

			return valid;
		}

		/// <summary>
		/// Build a System instance from the current builder state.
		/// </summary>
		/// <returns>A new System instance.</returns>
		/// <exception cref="ValidationException">Thrown if validation fails.</exception>
		System build() const {
			if (std::vector<ConnectedSystemsAPI::ValidationException::ValidationError> errors; !isValid(&errors)) {
				throw ConnectedSystemsAPI::ValidationException("SystemBuilder", errors);
			}

			return System(
				type.value_or("Feature"),
				id.value_or(""),
				properties.value(),
				bbox,
				links
			);
		}
	};
}