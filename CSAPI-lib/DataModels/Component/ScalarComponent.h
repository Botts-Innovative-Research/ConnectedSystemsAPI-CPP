#pragma once

#include "SimpleComponent.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Component {
			/// <summary>
			/// A simple (non-aggregate) component representing a single scalar value.
			/// </summary>
			class ScalarComponent : public SimpleComponent {
			public:
				virtual ~ScalarComponent() = default;

				void validate() const {
					SimpleComponent::validate();
				}
			};
		}
	}
}