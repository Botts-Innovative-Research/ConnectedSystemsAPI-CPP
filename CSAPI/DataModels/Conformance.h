#pragma once

#include <algorithm>
#include <ostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

namespace ConnectedSystemsAPI::DataModels {
	class Conformance;
	void to_json(nlohmann::ordered_json& j, const Conformance& v);

	/// <summary>
	/// The conformance declaration of an OGC API server: the list of conformance classes it implements.
	/// </summary>
	class Conformance {
	private:
		std::vector<std::string> conformsTo;

	public:
		/// <summary>
		/// Conformance class of the Connected Systems API Part 1 core, which every Connected Systems API server implements.
		/// </summary>
		static constexpr const char* CONNECTED_SYSTEMS_CORE = "http://www.opengis.net/spec/ogcapi-connectedsystems-1/1.0/conf/core";

		Conformance() = default;
		explicit Conformance(std::vector<std::string> conformsTo) : conformsTo(std::move(conformsTo)) {}

		/// <summary>
		/// URIs of the conformance classes the server implements.
		/// </summary>
		const std::vector<std::string>& getConformsTo() const { return conformsTo; }

		/// <summary>
		/// True if the server implements the given conformance class.
		/// </summary>
		bool conformsToClass(const std::string& conformanceClass) const {
			return std::find(conformsTo.begin(), conformsTo.end(), conformanceClass) != conformsTo.end();
		}

		/// <summary>
		/// True if the server implements the Connected Systems API.
		/// </summary>
		bool isConnectedSystemsAPI() const { return conformsToClass(CONNECTED_SYSTEMS_CORE); }

		friend void from_json(const nlohmann::json& j, Conformance& v);
		friend void to_json(nlohmann::ordered_json& j, const Conformance& v);

		friend std::ostream& operator<<(std::ostream& os, const Conformance& v) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, v);
			return os << j.dump(2);
		}
	};

	inline void from_json(const nlohmann::json& j, Conformance& v) {
		v.conformsTo = j.at("conformsTo").get<std::vector<std::string>>();
	}

	inline void to_json(nlohmann::ordered_json& j, const Conformance& v) {
		j = nlohmann::ordered_json::object();
		j["conformsTo"] = v.conformsTo;
	}
}
