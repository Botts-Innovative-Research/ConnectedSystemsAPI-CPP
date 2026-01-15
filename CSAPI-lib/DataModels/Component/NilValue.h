#pragma once

#include <string>
#include <ostream>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

namespace ConnectedSystemsAPI::DataModels::Component {
	class NilValue;
	void to_json(nlohmann::ordered_json& j, const NilValue& v);

	class NilValue {
		std::string reason;
		std::string value;

	public:
		NilValue() = default;
		NilValue(const std::string& reason, const std::string& value)
			: reason(reason), value(value) {
		}
		NilValue(const NilValue&) = default;
		NilValue(NilValue&&) noexcept = default;
		NilValue& operator=(const NilValue&) = default;
		NilValue& operator=(NilValue&&) noexcept = default;
		~NilValue() = default;

		nlohmann::ordered_json toJson() const {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		void validate() const {
			if (reason.empty())
				throw std::invalid_argument("NilValue.reason is required.");
			if (value.empty())
				throw std::invalid_argument("NilValue.value is required.");
		}

		/// <summary>
		/// The reason for using the reserved value.
		/// </summary>
		const std::string& getReason() const noexcept { return reason; }
		void setReason(const std::string& reason) { this->reason = reason; }

		/// <summary>
		/// The reserved value itself.
		/// </summary>
		const std::string& getValue() const noexcept { return value; }
		void setValue(const std::string& value) { this->value = value; }

		friend void from_json(const nlohmann::json& j, NilValue& v);
		friend void to_json(nlohmann::ordered_json& j, const NilValue& v);

		friend bool operator==(const NilValue& a, const NilValue& b) {
			return a.getReason() == b.getReason() && a.getValue() == b.getValue();
		}
		friend bool operator!=(const NilValue& a, const NilValue& b) {
			return !(a == b);
		}

		friend std::ostream& operator<<(std::ostream& os, const NilValue& v) {
			nlohmann::ordered_json j;
			to_json(j, v);
			return os << j.dump(2);
		}
	};

	inline void from_json(const nlohmann::json& j, NilValue& v) {
		v.reason = j["reason"].get<std::string>();
		v.value = j["value"].get<std::string>();
	}

	inline void to_json(nlohmann::ordered_json& j, const NilValue& v) {
		j = nlohmann::ordered_json::object();
		j["reason"] = v.reason;
		j["value"] = v.value;
	}
}