#pragma once

#include "SimpleComponent.h"

namespace ConnectedSystemsAPI::DataModels::Component {
	/// <summary>
	/// A simple (non-aggregate) component representing a single scalar value.
	/// </summary>
	class ScalarComponent : public SimpleComponent {
	public:
		~ScalarComponent() override = default;
	};
}