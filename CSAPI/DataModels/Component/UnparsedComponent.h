#pragma once

#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "DataComponent.h"
#include "DataComponentRegistry.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	/// <summary>
	/// A component that could not be parsed, e.g., because its type is unknown to this library or its JSON is invalid.
	/// It keeps the original JSON, so serializing it reproduces the input, and the reason parsing failed.
	/// Not registered with the DataComponentRegistry; it is only created by createDataComponentOrUnparsed.
	/// </summary>
	class UnparsedComponent : public DataComponent {
	private:
		nlohmann::ordered_json m_json;
		std::string m_error;

	public:
		UnparsedComponent() = default;
		UnparsedComponent(nlohmann::ordered_json json, std::string error)
			: m_json(std::move(json)), m_error(std::move(error)) {
			// Keep the common properties available, if present
			if (m_json.is_object()) {
				if (m_json.contains("type") && m_json["type"].is_string()) setType(m_json["type"].get<std::string>());
				if (m_json.contains("name") && m_json["name"].is_string()) setName(m_json["name"].get<std::string>());
				if (m_json.contains("id") && m_json["id"].is_string()) setId(m_json["id"].get<std::string>());
				if (m_json.contains("definition") && m_json["definition"].is_string()) setDefinition(m_json["definition"].get<std::string>());
			}
		}

		nlohmann::ordered_json toJson() const override { return m_json; }

		/// <summary>
		/// The original JSON of the component.
		/// </summary>
		const nlohmann::ordered_json& getJson() const noexcept { return m_json; }

		/// <summary>
		/// Why the component could not be parsed.
		/// </summary>
		const std::string& getError() const noexcept { return m_error; }
	};

	inline std::unique_ptr<DataComponent> DataComponentRegistry::createDataComponentOrUnparsed(const nlohmann::json& j) {
		try {
			return createDataComponent(j);
		}
		catch (const std::exception& e) {
			return std::make_unique<UnparsedComponent>(nlohmann::ordered_json(j), e.what());
		}
	}
}
