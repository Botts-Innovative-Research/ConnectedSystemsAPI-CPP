#pragma once

#include <string>
#include <optional>
#include <ostream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

namespace ConnectedSystemsAPI::DataModels {
	class Link;
	void to_json(nlohmann::ordered_json& j, const Link& v);

	class Link {
	private:
		std::string href;
		std::optional<std::string> relationType;
		std::optional<std::string> mediaType;
		std::optional<std::string> hrefLanguage;
		std::optional<std::string> title;
		std::optional<std::string> uid;
		std::optional<std::string> resourceType;
		std::optional<std::string> interfaceUri;

	public:
		Link() = default;
		Link(const std::string& href,
			const std::optional<std::string>& relationType = std::nullopt,
			const std::optional<std::string>& mediaType = std::nullopt,
			const std::optional<std::string>& hrefLanguage = std::nullopt,
			const std::optional<std::string>& title = std::nullopt,
			const std::optional<std::string>& uid = std::nullopt,
			const std::optional<std::string>& resourceType = std::nullopt,
			const std::optional<std::string>& interfaceUri = std::nullopt)
			: href(href), relationType(relationType), mediaType(mediaType), hrefLanguage(hrefLanguage),
			title(title), uid(uid), resourceType(resourceType), interfaceUri(interfaceUri) {
		}

		/// <summary>
		/// URL of the target resource.
		/// </summary>
		const std::string& getHref() const { return href; }
		/// <summary>
		/// Link relation type.
		/// </summary>
		const std::optional<std::string>& getRelationType() const { return relationType; }
		/// <summary>
		/// Media type of the target resource.
		/// </summary>
		const std::optional<std::string>& getMediaType() const { return mediaType; }
		/// <summary>
		/// Language tag of the target resource (2-letter language code, followed by optional 2-letter region code).
		/// </summary>
		const std::optional<std::string>& getHrefLanguage() const { return hrefLanguage; }
		/// <summary>
		/// Title of the target resource.
		/// </summary>
		const std::optional<std::string>& getTitle() const { return title; }
		/// <summary>
		/// Unique identifier of the target resource.
		/// </summary>
		const std::optional<std::string>& getUid() const { return uid; }
		/// <summary>
		/// Semantic type of the target resource (RFC 6690).
		/// </summary>
		const std::optional<std::string>& getResourceType() const { return resourceType; }
		/// <summary>
		/// Interface used to access the target resource (RFC 6690).
		/// </summary>
		const std::optional<std::string>& getInterfaceUri() const { return interfaceUri; }

		friend void from_json(const nlohmann::json& j, Link& v);
		friend void to_json(nlohmann::ordered_json& j, const Link& v);

		friend bool operator==(const Link& a, const Link& b) {
			return a.getHref() == b.getHref() &&
				a.getRelationType() == b.getRelationType() &&
				a.getMediaType() == b.getMediaType() &&
				a.getHrefLanguage() == b.getHrefLanguage() &&
				a.getTitle() == b.getTitle() &&
				a.getUid() == b.getUid() &&
				a.getResourceType() == b.getResourceType() &&
				a.getInterfaceUri() == b.getInterfaceUri();
		}

		friend bool operator!=(const Link& a, const Link& b) {
			return !(a == b);
		}

		friend std::ostream& operator<<(std::ostream& os, const Link& v) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, v);
			return os << j.dump(2);
		}
	};

	inline void from_json(const nlohmann::json& j, Link& v) {
		v.href = j.at("href").get<std::string>();
		v.relationType = j.value("rel", std::optional<std::string>{});
		v.mediaType = j.value("type", std::optional<std::string>{});
		v.hrefLanguage = j.value("hreflang", std::optional<std::string>{});
		v.title = j.value("title", std::optional<std::string>{});
		v.uid = j.value("uid", std::optional<std::string>{});
		v.resourceType = j.value("rt", std::optional<std::string>{});
		v.interfaceUri = j.value("if", std::optional<std::string>{});
	}

	inline void to_json(nlohmann::ordered_json& j, const Link& v) {
		j = nlohmann::ordered_json::object();

		j["href"] = v.href;
		if (v.relationType.has_value()) j["rel"] = v.relationType.value();
		if (v.mediaType.has_value()) j["type"] = v.mediaType.value();
		if (v.hrefLanguage.has_value()) j["hreflang"] = v.hrefLanguage.value();
		if (v.title.has_value()) j["title"] = v.title.value();
		if (v.uid.has_value()) j["uid"] = v.uid.value();
		if (v.resourceType.has_value()) j["rt"] = v.resourceType.value();
		if (v.interfaceUri.has_value()) j["if"] = v.interfaceUri.value();
	}
}