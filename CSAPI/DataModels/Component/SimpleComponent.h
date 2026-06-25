#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <utility>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "DataComponent.h"
#include "NilValue.h"
#include "../../Util/JsonUtils.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class SimpleComponent : public DataComponent {
	private:
		std::optional<std::string> m_referenceFrame;
		std::optional<std::string> m_axisId;
		std::optional<std::vector<NilValue>> m_nilValues;

	public:
		SimpleComponent() = default;

		nlohmann::ordered_json toJson() const {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		void validate() const override {
			DataComponent::validate();

			if (m_nilValues) {
				for (const auto& nv : m_nilValues.value()) {
					nv.validate();
				}
			}
		}

		/// <summary>
		/// Frame of reference (usually temporal or spatial) with respect to which the value of the component is expressed.
		/// A reference frame anchors a value to a real world datum.
		/// </summary>
		std::optional<std::string> getReferenceFrame() const noexcept { return m_referenceFrame; }
		void setReferenceFrame(const std::optional<std::string> referenceFrame) { m_referenceFrame = referenceFrame; }
		void clearReferenceFrame() noexcept { m_referenceFrame.reset(); }

		/// <summary>
		/// Specifies the reference axis (refer to CRS axisID).
		/// The reference frame URI should also be specified unless it is inherited from parent Vector.
		/// </summary>
		std::optional<std::string> getAxisId() const noexcept { return m_axisId; }
		void setAxisId(const std::optional<std::string> axisId) { m_axisId = axisId; }
		void clearAxisId() noexcept { m_axisId.reset(); }

		/// <summary>
		/// Defines reserved values with special meaning (e.g., missing, out-of-range, etc.)
		/// </summary>		
		const std::optional<std::vector<NilValue>>& getNilValues() const noexcept { return m_nilValues; }
		void setNilValues(const std::optional<std::vector<NilValue>>& nilValues) { m_nilValues = nilValues; }
		void setNilValues(std::optional<std::vector<NilValue>>&& nilValues) noexcept { m_nilValues = std::move(nilValues); }
		void clearNilValues() noexcept { m_nilValues.reset(); }

		friend void from_json(const nlohmann::json& j, SimpleComponent& v);
		friend void to_json(nlohmann::ordered_json& j, const SimpleComponent& v);

		friend bool operator==(const SimpleComponent& a, const SimpleComponent& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const SimpleComponent& a, const SimpleComponent& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const SimpleComponent& s) {
			nlohmann::ordered_json j;
			to_json(j, s);
			return os << j.dump(2);
		}
	};

	inline void from_json(const nlohmann::json& j, SimpleComponent& v) {
		from_json(j, static_cast<DataComponent&>(v));

		v.m_referenceFrame = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "refFrame");
		v.m_axisId = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "axisID");

		if (j.contains("nilValues") && j["nilValues"].is_array())
			v.m_nilValues = j.at("nilValues").get<std::vector<NilValue>>();
		else
			v.m_nilValues = std::nullopt;
	}

	inline void to_json(nlohmann::ordered_json& j, const SimpleComponent& s) {
		to_json(j, static_cast<const DataComponent&>(s));

		if (s.m_referenceFrame) j["refFrame"] = s.m_referenceFrame;
		if (s.m_axisId) j["axisID"] = s.m_axisId;
		if (s.m_nilValues) j["nilValues"] = s.m_nilValues.value();
	}
}