#pragma once

#include <string>
#include <ostream>
#include <nlohmann/json.hpp>

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
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
			};

			inline bool operator==(const NilValue& a, const NilValue& b) {
				return a.getReason() == b.getReason() && a.getValue() == b.getValue();
			}
			inline bool operator!=(const NilValue& a, const NilValue& b) {
				return !(a == b);
			}

			inline void from_json(const nlohmann::json& j, NilValue& n) {
				n = NilValue();
				n.setReason(j["reason"].get<std::string>());
				n.setValue(j["value"].get<std::string>());
			}

			inline void to_json(nlohmann::ordered_json& j, const NilValue& n) {
				j = nlohmann::ordered_json::object();
				j["reason"] = n.getReason();
				j["value"] = n.getValue();
			}

			inline std::ostream& operator<<(std::ostream& os, const NilValue& n) {
				nlohmann::ordered_json j;
				to_json(j, n);
				return os << j.dump(2);
			}
		}
	}
}