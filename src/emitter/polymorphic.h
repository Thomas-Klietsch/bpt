#pragma once

#include "../ray/section.h"
#include <utility>
#include <tuple>
#include "../colour/colour.h"
#include "../mathematics/double3.h"
#include "../random/polymorphic.h"

namespace Emitter
{

	class Polymorphic
	{

	public:

		// Energy, Direction to point on emitter, Distance
		virtual std::tuple<Colour, Double3, double> evaluate(
			Double3 const& point_illuminated,
			Random::Polymorphic& random
		) const = 0;

		// Energy, Point on surface, Direction from surface, Normal at point on surface
		virtual std::tuple <Colour, Double3, Double3, Double3> emit(
			Random::Polymorphic& random
		) const = 0;

	};

};
