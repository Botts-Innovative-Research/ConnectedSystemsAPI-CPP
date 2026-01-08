#pragma once

namespace ConnectedSystemsAPI::DataModels::Data {
	enum class DataType {
		Boolean,
		Byte,
		Short,
		Int,
		Long,
		UByte,
		UShort,
		UInt,
		ULong,
		Float,
		Double,
		String,
		Instant,
		DateTime,
		Other,
		Discard,
		Mixed
	};
}