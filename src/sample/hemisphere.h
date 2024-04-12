#pragma once

#include <cmath>
#include <utility>

#include "../mathematics/constant.h"
#include "../mathematics/double3.h"
#include "../random/polymorphic.h"

namespace Sample
{

	Double3 HemiSphere( Random::Polymorphic& random )
	{
		auto const [e1, e2] = random.get_float2();
		float const phi = e1 * two_pi;
		float radius = std::sqrt( std::max( 0.f, 1.f - e2 * e2 ) );
		return { std::cos( phi ) * radius, std::sin( phi ) * radius, e2 };
	};

};

