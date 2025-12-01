#pragma once

#include <string>
#include <optional>
#include <stdexcept>

#include "DataComponent.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			template<typename Derived, typename Target>
			class DataComponentBuilder {
			protected:
				std::string type;
				std::optional<std::string> id;
				std::optional<std::string> name;
				std::optional<std::string> label;
				std::optional<std::string> description;
				std::optional<bool> updatable;
				std::optional<bool> optional;
				std::optional<std::string> definition;

				Derived& self() noexcept { return *reinterpret_cast<Derived*>(this); }
				const Derived& self() const noexcept { return *reinterpret_cast<const Derived*>(this); }

			public:
				DataComponentBuilder() = default;

				// Fluent setters for DataComponent base properties
				Derived& withType(const std::string& v) { type = v; return self(); }
				Derived& withId(const std::string& v) { id = v; return self(); }
				Derived& withName(const std::string& v) { name = v; return self(); }
				Derived& withLabel(const std::string& v) { label = v; return self(); }
				Derived& withDescription(const std::string& v) { description = v; return self(); }
				Derived& withUpdatable(bool v) { updatable = v; return self(); }
				Derived& withOptional(bool v) { optional = v; return self(); }
				Derived& withDefinition(const std::string& v) { definition = v; return self(); }

			protected:
				void validateBase() const {
					if (type.empty())
						throw std::runtime_error("DataComponentBuilder: type is required.");
				}

				void applyBase(Target& target) const {
					if (!type.empty()) target.setType(type);
					if (name) target.setName(name);
					if (id) target.setId(id);
					if (label) target.setLabel(label);
					if (description) target.setDescription(description);
					if (updatable) target.setUpdatable(updatable);
					if (optional) target.setOptional(optional);
					if (definition) target.setDefinition(definition);
				}
			};
		}
	}
}