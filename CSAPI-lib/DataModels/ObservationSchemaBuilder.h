#pragma once

#include <string>
#include <optional>
#include <memory>
#include <stdexcept>

#include "ObservationSchema.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class ObservationSchemaBuilder {
		private:
			std::string observationFormat;
			std::optional<Component::DataRecord> parametersSchema;
			std::unique_ptr<Component::DataComponent> resultSchema;
			std::optional<Link> resultLink;

			void validate() const {
				if (observationFormat.empty())
					throw std::runtime_error("ObservationSchemaBuilder: observationFormat must be set.");
				if (!resultSchema)
					throw std::runtime_error("ObservationSchemaBuilder: resultSchema must be set.");
			}

		public:
			ObservationSchemaBuilder() = default;

			ObservationSchemaBuilder& withObservationFormat(const std::string& fmt) { observationFormat = fmt; return *this; }
			ObservationSchemaBuilder& withObservationFormat(std::string&& fmt) { observationFormat = std::move(fmt); return *this; }
			ObservationSchemaBuilder& withParametersSchema(const Component::DataRecord& rec) { parametersSchema = rec; return *this; }
			ObservationSchemaBuilder& withoutParametersSchema() { parametersSchema.reset(); return *this; }
			ObservationSchemaBuilder& withResultSchema(std::unique_ptr<Component::DataComponent> v) { resultSchema = std::move(v); return *this; }
			ObservationSchemaBuilder& withResultLink(const Link& link) { resultLink = link; return *this; }

			ObservationSchema build() {
				validate();

				return ObservationSchema(
					observationFormat,
					parametersSchema,
					std::move(resultSchema),
					resultLink
				);
			}
		};
	}
}