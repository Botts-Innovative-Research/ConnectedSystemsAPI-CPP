#pragma once

#include "DataModels/Data/DataType.h"

namespace ConnectedSystemsAPI {
	namespace DataModels {
		namespace Data {
			class DataBlock {
			public:
				virtual ~DataBlock() = default;

				virtual DataType getDataType() const noexcept = 0;
			};
		}
	}
}