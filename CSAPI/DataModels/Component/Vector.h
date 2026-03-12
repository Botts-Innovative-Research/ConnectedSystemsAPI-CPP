#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <vector>
#include <memory>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "DataComponent.h"
#include "DataComponentRegistry.h"
#include "Util/JsonUtils.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class Vector;
	void to_json(nlohmann::ordered_json& j, const Vector& r);

	class Vector : public DataComponent {
	private:
		std::optional<std::string> referenceFrame;
		std::optional<std::string> localFrame;
		std::vector<std::unique_ptr<DataComponent>> coordinates;

	public:
		Vector() = default;
		Vector(const Vector&) = delete;
		Vector(Vector&&) noexcept = default;
		Vector& operator=(const Vector&) = delete;
		Vector& operator=(Vector&&) noexcept = default;
		~Vector() override = default;

		void validate() const override {
			DataComponent::validate();
			for (const auto& coordPtr : coordinates) {
				if (coordPtr) {
					coordPtr->validate();
				}
			}
		}

		nlohmann::ordered_json toJson() const override {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		/// <summary>
		/// Frame of reference (usually spatial) with respect to which the coordinates of this vector are expressed.
		/// A reference frame anchors a vector value to a real world datum.
		/// </summary>
		std::optional<std::string> getReferenceFrame() const noexcept { return referenceFrame; }
		void setReferenceFrame(const std::optional<std::string>& referenceFrame) { this->referenceFrame = referenceFrame; }
		void setReferenceFrame(std::string&& referenceFrame) noexcept { this->referenceFrame = std::move(referenceFrame); }
		bool hasReferenceFrame() const noexcept { return referenceFrame.has_value(); }
		void clearReferenceFrame() noexcept { referenceFrame.reset(); }

		/// <summary>
		/// Frame of reference whose origin is located by the coordinates of this vector.
		/// </summary>
		std::optional<std::string> getLocalFrame() const noexcept { return localFrame; }
		void setLocalFrame(const std::optional<std::string>& localFrame) { this->localFrame = localFrame; }
		void setLocalFrame(std::string&& localFrame) noexcept { this->localFrame = std::move(localFrame); }
		bool hasLocalFrame() const noexcept { return localFrame.has_value(); }
		void clearLocalFrame() noexcept { localFrame.reset(); }

		/// <summary>
		/// Definition of the coordinate provided as a data component with a numerical representation.
		/// </summary>
		const std::vector<std::unique_ptr<DataComponent>>& getCoordinates() const noexcept { return coordinates; }
		void setCoordinates(std::vector<std::unique_ptr<DataComponent>> f) { coordinates = std::move(f); }
		bool hasCoordinates() const noexcept { return !coordinates.empty(); }
		void clearCoordinates() noexcept { coordinates.clear(); }

		friend void from_json(const nlohmann::json& j, Vector& v);
		friend void to_json(nlohmann::ordered_json& j, const Vector& v);

		friend bool operator==(const Vector& a, const Vector& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const Vector& a, const Vector& b) { return !(a == b); }

		friend std::ostream& operator<<(std::ostream& os, const Vector& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			return os << j.dump(2);
		}
	};

	const inline DataComponent::Registrar<Vector> registerVector{ "Vector" };

	inline void from_json(const nlohmann::json& j, Vector& v) {
		from_json(j, static_cast<DataComponent&>(v));

		v.referenceFrame = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "referenceFrame");
		v.localFrame = ConnectedSystemsAPI::JsonUtils::tryParseString(j, "localFrame");

		if (j.contains("coordinates") && j["coordinates"].is_array()) {
			std::vector<std::unique_ptr<DataComponent>> tempFields;
			for (const auto& fieldJson : j["coordinates"]) {
				if (auto ptr = DataComponentRegistry::createDataComponent(fieldJson)) {
					tempFields.push_back(std::move(ptr));
				}
			}
			v.coordinates = std::move(tempFields);
		}
		else {
			v.coordinates.clear();
		}
	}

	inline void to_json(nlohmann::ordered_json& j, const Vector& v) {
		to_json(j, static_cast<const DataComponent&>(v));

		if (v.referenceFrame) j["referenceFrame"] = v.referenceFrame.value();
		if (v.localFrame) j["localFrame"] = v.localFrame.value();

		j["coordinates"] = nlohmann::ordered_json::array();
		for (const auto& coordPtr : v.getCoordinates()) {
			if (coordPtr) {
				j["coordinates"].push_back(coordPtr->toJson());
			}
		}
	}
}