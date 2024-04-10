#pragma once

#include <cstdint>

#include "../mathematics/double3.h"
#include "../mathematics/orthogonal.h"

namespace Ray
{

	struct Intersection
	{
		// post intersection
		Double3 normal{ 0, 0, 0 };
		Double3 point{ 0, 0, 0 };
		Double3 local_wray{ 0, 0, 0 };
		Orthogonal orthogonal;
		uint32_t material_id{ 0 };
	};

};

