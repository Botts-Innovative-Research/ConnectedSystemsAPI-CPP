#pragma once

// These includes are necessary to ensure that the DataComponent types are registered.
// Including this header in ConnectedSystemsAPI.h ensures that the registration happens
// before any API calls that might require deserialization of these types.
#include "DataModels/Component/Boolean.h"
#include "DataModels/Component/Category.h"
#include "DataModels/Component/CategoryRange.h"
#include "DataModels/Component/Count.h"
#include "DataModels/Component/CountRange.h"
#include "DataModels/Component/DataRecord.h"
#include "DataModels/Component/Quantity.h"
#include "DataModels/Component/QuantityRange.h"
#include "DataModels/Component/Text.h"
#include "DataModels/Component/Time.h"
#include "DataModels/Component/TimeRange.h"
#include "DataModels/Component/Vector.h"
