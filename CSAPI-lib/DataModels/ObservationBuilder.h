#pragma once

#include <string>
#include <optional>
#include <nlohmann/json.hpp>

#include "Link.h"
#include "TimeInstant.h"
#include "Observation.h"
#include "DataModels/Data/DataBlockMixed.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class ObservationBuilder {
		private:
			std::optional<std::string> id;
			std::optional<std::string> dataStreamId;
			std::optional<std::string> samplingFeatureId;
			std::optional<Link> procedureLink;
			std::optional<TimeInstant> phenomenonTime;
			std::optional<TimeInstant> resultTime;
			// todo: parameters
			Data::DataBlockMixed result;
			std::optional<Link> resultLink;
			std::optional<std::vector<Link>> links;

		public:
			ObservationBuilder() = default;

			ObservationBuilder& from(const Observation& o) {
				id = o.getId();
				dataStreamId = o.getDataStreamId();
				samplingFeatureId = o.getSamplingFeatureId();
				procedureLink = o.getProcedureLink();
				phenomenonTime = o.getPhenomenonTime();
				resultTime = o.getResultTime();
				result = o.getResult();
				resultLink = o.getResultLink();
				links = o.getLinks();
				return *this;
			}

			ObservationBuilder& withId(const std::string& v) { id = v; return *this; }
			ObservationBuilder& withDataStreamId(const std::string& v) { dataStreamId = v; return *this; }
			ObservationBuilder& withSamplingFeatureId(const std::string& v) { samplingFeatureId = v; return *this; }
			ObservationBuilder& withProcedureLink(const Link& v) { procedureLink = v; return *this; }
			ObservationBuilder& withPhenomenonTime(const TimeInstant& v) { phenomenonTime = v; return *this; }
			ObservationBuilder& withResultTime(const TimeInstant& v) { resultTime = v; return *this; }
			ObservationBuilder& withResult(const Data::DataBlockMixed& v) { result = v; return *this; }
			ObservationBuilder& withResultLink(const Link& v) { resultLink = v; return *this; }
			ObservationBuilder& withLinks(const std::vector<Link>& v) { links = v; return *this; }
			Observation build() const {
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
					result,
					resultLink,
					links
				);
			}
		};
	}
}