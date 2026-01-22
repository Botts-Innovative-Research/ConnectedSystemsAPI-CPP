#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "Count.h"
#include "DataComponent.h"
#include "DataComponentRegistry.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class DataArray;
	void to_json(nlohmann::ordered_json& j, const DataArray& v);

	class DataArray : public DataComponent {
	private:
		Count elementCount;
		std::unique_ptr<DataComponent> elementType;

	public:
		DataArray() = default;
		DataArray(const DataArray& other) {
			nlohmann::ordered_json j = other.toJson();
			from_json(j, *this);
		}
		DataArray& operator=(const DataArray& other) {
			if (this != &other) {
				nlohmann::ordered_json j = other.toJson();
				from_json(j, *this);
			}
			return *this;
		}
		DataArray(DataArray&&) noexcept = default;
		DataArray& operator=(DataArray&&) noexcept = default;
		~DataArray() override = default;

		nlohmann::ordered_json toJson() const override {
			nlohmann::ordered_json j;
			to_json(j, *this);
			return j;
		}

		void validate() const override {
			DataComponent::validate();
			elementCount.validate();
			if (!elementType)
				throw std::invalid_argument("DataArray.elementType is required");
			elementType->validate();
		}

		/// <summary>
		/// Specifies the size of the array (i.e., the number of elements of the defined type it contains).
		/// </summary>
		const Count& getElementCount() const { return elementCount; }
		void setElementCount(const Count& elementCount) { this->elementCount = elementCount; }

		/// <summary>
		/// Defines the structure of the element that will be repeated in the array.
		/// </summary>
		const DataComponent* getElementType() const { return elementType.get(); }
		void setElementType(std::unique_ptr<DataComponent> elementType) { this->elementType = std::move(elementType); }

		friend void from_json(const nlohmann::json& j, DataArray& v);
		friend void to_json(nlohmann::ordered_json& j, const DataArray& v);

		friend bool operator==(const DataArray& a, const DataArray& b) { return a.toJson() == b.toJson(); }
		friend bool operator!=(const DataArray& a, const DataArray& b) { return !(a == b); }
	};

	const inline DataComponent::Registrar<DataArray> registerDataArray{ "DataArray" };

	inline void from_json(const nlohmann::json& j, DataArray& v) {
		from_json(j, static_cast<DataComponent&>(v));

		v.elementCount = j.at("elementCount").get<Count>();
		v.elementType = DataComponentRegistry::createDataComponent(j.at("elementType"));
	}

	inline void to_json(nlohmann::ordered_json& j, const DataArray& v) {
		to_json(j, static_cast<const DataComponent&>(v));

		j["elementCount"] = v.elementCount;
		j["elementType"] = v.elementType ? v.elementType->toJson() : nullptr;
	}
}