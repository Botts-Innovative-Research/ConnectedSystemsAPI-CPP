#pragma once

#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "DataComponent.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			/// <summary>
			/// Registry for DataComponent types to support polymorphic deserialization from JSON.
			/// </summary>
			class DataComponentRegistry {
			public:
				using FactoryFunc = std::function<std::unique_ptr<DataComponent>(const nlohmann::json&)>;

				// Registers a factory for a given type string
				static void registerType(const std::string& type, FactoryFunc factory) {
					getRegistry()[type] = std::move(factory);
				}

				// Creates a DataComponent (or derived) from JSON
				static std::unique_ptr<DataComponent> createDataComponent(const nlohmann::json& j) {
					std::string type = j.at("type").get<std::string>();
					auto& registry = getRegistry();
					auto it = registry.find(type);
					if (it != registry.end()) {
						return it->second(j);
					}
					std::cout << "Unknown component type during deserialization: " << type << std::endl;
					throw std::runtime_error("DataComponentRegistry: Unknown component type: " + type);
				}

			private:
				static std::unordered_map<std::string, FactoryFunc>& getRegistry() {
					static std::unordered_map<std::string, FactoryFunc> registry;
					return registry;
				}
			};
		}
	}
}