#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace ConnectedSystemsAPI {
	namespace DataModels {
		class Link {
		private:
			std::string href;
			std::string relationType;
			std::string mediaType;
			std::string hrefLanguage;
			std::string title;
			std::string uid;
			std::string resourceType;
			std::string interfaceUri;

		public:
			Link() = default;
			Link(const std::string& href,
				const std::string& relationType,
				const std::string& mediaType,
				const std::string& hrefLanguage,
				const std::string& title,
				const std::string& uid,
				const std::string& resourceType,
				const std::string& interfaceUri)
				: href(href), relationType(relationType), mediaType(mediaType), hrefLanguage(hrefLanguage),
				title(title), uid(uid), resourceType(resourceType), interfaceUri(interfaceUri) {
			}

			/// <returns>URL of the target resource.</returns>
			const std::string& getHref() const { return href; }
			/// <returns>Link relation type.</returns>
			const std::string& getRelationType() const { return relationType; }
			/// <returns>Media type of the target resource.</returns>
			const std::string& getMediaType() const { return mediaType; }
			/// <returns>Language tag of the target resource (2-letter language code, followed by optional 2-letter region code).</returns>
			const std::string& getHrefLanguage() const { return hrefLanguage; }
			/// <returns>Title of the target resource.</returns>
			const std::string& getTitle() const { return title; }
			/// <returns>Unique identifier of the target resource</returns>
			const std::string& getUid() const { return uid; }
			/// <returns>Semantic type of the target resource (RFC 6690).</returns>
			const std::string& getResourceType() const { return resourceType; }
			/// <returns>Interface used to access the target resource (RFC 6690).</returns>
			const std::string& getInterfaceUri() const { return interfaceUri; }
		};

		inline void from_json(const nlohmann::json& j, Link& l) {
			l = Link(
				j.at("href").get<std::string>(),
				j.value("rel", ""),
				j.value("type", ""),
				j.value("hreflang", ""),
				j.value("title", ""),
				j.value("uid", ""),
				j.value("rt", ""),
				j.value("if", "")
			);
		}

		inline void to_json(nlohmann::ordered_json& j, const Link& l) {
			j = nlohmann::ordered_json::object();
			j["href"] = l.getHref();

			if (!l.getRelationType().empty()) {
				j["rel"] = l.getRelationType();
			}
			if (!l.getMediaType().empty()) {
				j["type"] = l.getMediaType();
			}
			if (!l.getHrefLanguage().empty()) {
				j["hreflang"] = l.getHrefLanguage();
			}
			if (!l.getTitle().empty()) {
				j["title"] = l.getTitle();
			}
			if (!l.getUid().empty()) {
				j["uid"] = l.getUid();
			}
			if (!l.getResourceType().empty()) {
				j["rt"] = l.getResourceType();
			}
			if (!l.getInterfaceUri().empty()) {
				j["if"] = l.getInterfaceUri();
			}
		}

		inline std::ostream& operator<<(std::ostream& os, const Link& l) {
			nlohmann::ordered_json j;
			ConnectedSystemsAPI::DataModels::to_json(j, l);
			return os << j.dump(4);
		}
	}
}