#pragma once

#include <cstdint>

namespace BxDF
{

	enum class Event : uint8_t
	{
		None,
		Diffuse,
		Emission,
		Reflect,
		Transmit
	};

};
