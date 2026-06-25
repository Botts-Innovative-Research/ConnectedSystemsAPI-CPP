#pragma once

#include <optional>
#include <utility>

#include "Boolean.h"
#include "DataComponentBuilder.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	class BooleanBuilder : public DataComponentBuilder<BooleanBuilder, Boolean> {
	private:
		std::optional<bool> m_value;

		void validate() const {
			this->validateBase();
		}

	public:
		BooleanBuilder() = default;

		BooleanBuilder& withValue(std::optional<bool> value) { m_value = std::move(value); return *this; }
		BooleanBuilder& withValue(bool value) { m_value = value; return *this; }
		BooleanBuilder& clearValue() { m_value.reset(); return *this; }

		Boolean build() {
			validate();

			Boolean b;
			this->applyBase(b);

			if (m_value) b.setValue(m_value);

			return b;
		}
	};
}