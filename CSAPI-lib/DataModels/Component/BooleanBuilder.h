#pragma once

#include <optional>

#include "Boolean.h"
#include "DataComponentBuilder.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class BooleanBuilder : public DataComponentBuilder<BooleanBuilder, Boolean> {
	private:
		std::optional<bool> value;

		void validate() const {
			this->validateBase();
		}

	public:
		BooleanBuilder() = default;

		BooleanBuilder& withValue(std::optional<bool> v) { value = std::move(v); return *this; }
		BooleanBuilder& withValue(bool v) { value = v; return *this; }
		BooleanBuilder& clearValue() { value.reset(); return *this; }

		Boolean build() {
			validate();

			Boolean b;
			this->applyBase(b);

			if (value) b.setValue(value);

			return b;
		}
	};
}