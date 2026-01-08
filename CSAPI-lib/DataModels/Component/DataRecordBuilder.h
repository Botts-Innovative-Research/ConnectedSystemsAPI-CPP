#pragma once

#include <vector>
#include <memory>
#include <stdexcept>

#include "DataRecord.h"
#include "DataComponentBuilder.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class DataRecordBuilder : public DataComponentBuilder<DataRecordBuilder, DataRecord> {
	private:
		std::vector<std::unique_ptr<DataComponent>> fields;

		void validate() const {
			this->validateBase();

			for (const auto& field : fields) {
				if (field)
					field->validate();
				else
					throw std::runtime_error("DataRecordBuilder: fields cannot contain null pointers.");
			}
		}

	public:
		DataRecordBuilder() = default;

		DataRecordBuilder& withFields(std::vector<std::unique_ptr<DataComponent>> f) { fields = std::move(f); return *this; }
		DataRecordBuilder& addField(std::unique_ptr<DataComponent> field) { if (field) fields.push_back(std::move(field)); return *this; }
		DataRecordBuilder& clearFields() { fields.clear(); return *this; }

		DataRecord build() {
			validate();

			DataRecord r;
			this->applyBase(r);
			r.setFields(std::move(fields));

			return r;
		}
	};
}