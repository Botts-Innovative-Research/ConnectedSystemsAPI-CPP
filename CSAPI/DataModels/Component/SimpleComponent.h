#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <utility>
#include <memory>
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
		std::vector<std::unique_ptr<DataComponent>> m_quality;
		std::optional<std::vector<NilValue>> m_nilValues;

		static std::vector<std::unique_ptr<DataComponent>> cloneQuality(const std::vector<std::unique_ptr<DataComponent>>& quality) {
			std::vector<std::unique_ptr<DataComponent>> copy;
			copy.reserve(quality.size());
			for (const auto& q : quality) {
				if (q) copy.push_back(DataComponentRegistry::createDataComponent(q->toJson()));
			}
			return copy;
		}

	public:
		SimpleComponent() = default;
		SimpleComponent(const SimpleComponent& other)
			: DataComponent(other), m_referenceFrame(other.m_referenceFrame), m_axisId(other.m_axisId),
			m_quality(cloneQuality(other.m_quality)), m_nilValues(other.m_nilValues) {
		}
		SimpleComponent& operator=(const SimpleComponent& other) {
			if (this != &other) {
				DataComponent::operator=(other);
				m_referenceFrame = other.m_referenceFrame;
				m_axisId = other.m_axisId;
				m_quality = cloneQuality(other.m_quality);
				m_nilValues = other.m_nilValues;
			}
			return *this;
		}
		SimpleComponent(SimpleComponent&&) noexcept = default;
		SimpleComponent& operator=(SimpleComponent&&) noexcept = default;
		~SimpleComponent() override = default;

		nlohmann::ordered_json toJson() const override {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		void validate() const override {
			DataComponent::validate();

			for (const auto& q : m_quality) {
				if (q) q->validate();
			}

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
		/// Information regarding the precision, accuracy, tolerance, etc. of the value,
		/// given as Quantity, QuantityRange, Category, or Text components.
		/// </summary>
		const std::vector<std::unique_ptr<DataComponent>>& getQuality() const noexcept { return m_quality; }
		void setQuality(std::vector<std::unique_ptr<DataComponent>> quality) noexcept { m_quality = std::move(quality); }
		void addQuality(std::unique_ptr<DataComponent> quality) { if (quality) m_quality.push_back(std::move(quality)); }
		void clearQuality() noexcept { m_quality.clear(); }

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

		v.m_referenceFrame = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "referenceFrame");
		v.m_axisId = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "axisID");

		v.m_quality.clear();
		if (j.contains("quality") && j["quality"].is_array()) {
			for (const auto& q : j["quality"]) {
				v.m_quality.push_back(DataComponentRegistry::createDataComponentOrUnparsed(q));
			}
		}

		if (j.contains("nilValues") && j["nilValues"].is_array())
			v.m_nilValues = j.at("nilValues").get<std::vector<NilValue>>();
		else
			v.m_nilValues = std::nullopt;
	}

	inline void to_json(nlohmann::ordered_json& j, const SimpleComponent& s) {
		to_json(j, static_cast<const DataComponent&>(s));

		if (s.m_referenceFrame) j["referenceFrame"] = s.m_referenceFrame.value();
		if (s.m_axisId) j["axisID"] = s.m_axisId.value();
		if (!s.m_quality.empty()) {
			j["quality"] = nlohmann::ordered_json::array();
			for (const auto& q : s.m_quality) {
				if (q) j["quality"].push_back(q->toJson());
			}
		}
		if (s.m_nilValues) j["nilValues"] = s.m_nilValues.value();
	}
}