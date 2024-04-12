#pragma once

#include <tuple>
#include <utility>

#include "../colour/colour.h"
#include "../mathematics/double3.h"
#include "../random/polymorphic.h"

namespace Emitter
{

	class Polymorphic
	{

	public:

		// Energy, Point on surface, Direction from surface, Normal at point on surface
		virtual std::tuple <Colour, Double3, Double3, Double3> emit(
			Random::Polymorphic& random
		) const = 0;

	};

};
