#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "Link.h"
#include "Component/DataComponent.h"
#include "Component/DataComponentRegistry.h"
#include "Component/DataRecord.h"

namespace ConnectedSystemsAPI::DataModels {
	class ObservationSchema {
	private:
		std::string observationFormat;
		std::optional<Component::DataRecord> parametersSchema;
		std::unique_ptr<Component::DataComponent> resultSchema;
		std::optional<Link> resultLink;

	public:
		ObservationSchema() = default;
		ObservationSchema(const std::string& observationFormat,
			const std::optional<Component::DataRecord>& parametersSchema,
			std::unique_ptr<Component::DataComponent> resultSchema,
			const std::optional<Link>& resultLink)
			: observationFormat(observationFormat), parametersSchema(parametersSchema),
			resultSchema(std::move(resultSchema)), resultLink(resultLink) {
		}

		ObservationSchema(const ObservationSchema& other)
			: observationFormat(other.observationFormat),
			parametersSchema(other.parametersSchema),
			resultLink(other.resultLink) {
			if (other.resultSchema) {
				nlohmann::ordered_json j = other.resultSchema->toJson();
				resultSchema = Component::DataComponentRegistry::createDataComponent(j);
			}
		}

		ObservationSchema& operator=(const ObservationSchema& other) {
			if (this != &other) {
				observationFormat = other.observationFormat;
				parametersSchema = other.parametersSchema;
				resultLink = other.resultLink;

				if (other.resultSchema) {
					nlohmann::ordered_json j = other.resultSchema->toJson();
					resultSchema = Component::DataComponentRegistry::createDataComponent(j);
				}
				else {
					resultSchema.reset();
				}
			}
			return *this;
		}

		ObservationSchema(ObservationSchema&&) noexcept = default;
		ObservationSchema& operator=(ObservationSchema&&) noexcept = default;

		/// <summary>
		/// Encoding format of the observation.
		/// </summary>
		const std::string& getObservationFormat() const { return observationFormat; }
		/// <summary>
		/// Record schema for the observation parameters property. If omitted, parameters are not included in the datastream.
		/// </summary>
		const std::optional<Component::DataRecord>& getParametersSchema() const { return parametersSchema; }
		/// <summary>
		/// Schema for the observation result property.
		/// this describes the observed properties included in the result
		/// and how they are structured if the result is a record, a vector quantity or a coverage.
		/// </summary>
		const Component::DataComponent* getResultSchema() const { return resultSchema.get(); }
		/// <summary>
		/// Encoding information in case the result is provided out-of-band via the result@link property.
		/// </summary>
		const std::optional<Link>& getResultLink() const { return resultLink; }
	};

	inline void from_json(const nlohmann::json& j, ObservationSchema& o) {
		o = ObservationSchema(
			j.at("obsFormat").get<std::string>(),
			j.value("parametersSchema", std::optional<Component::DataRecord>{}),
			Component::DataComponentRegistry::createDataComponent(j.at("resultSchema")),
			j.value("resultLink", std::optional<Link>{})
		);
	}

	inline void to_json(nlohmann::ordered_json& j, const ObservationSchema& o) {
		j = nlohmann::ordered_json::object();
		j["obsFormat"] = o.getObservationFormat();

		if (o.getParametersSchema())
			j["parametersSchema"] = o.getParametersSchema().value().toJson();

		if (o.getResultSchema())
			j["resultSchema"] = o.getResultSchema()->toJson();

		if (o.getResultLink())
			j["resultLink"] = o.getResultLink().value();
	}

	inline std::ostream& operator<<(std::ostream& os, const ObservationSchema& o) {
		nlohmann::ordered_json j;
		to_json(j, o);
		return os << j.dump(2);
	}
}