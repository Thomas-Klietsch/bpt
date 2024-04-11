#pragma once

#include <tuple>
#include <utility>

#include "../bxdf/common.h"
#include "../colour/colour.h"
#include "../mathematics/double3.h"
#include "../ray/intersection.h"
#include "../random/polymorphic.h"

namespace BxDF
{

	class Polymorphic
	{

	public:

		virtual std::tuple<Colour, Double3, BxDF::Event> sample(
			Ray::Intersection const& idata,
			Random::Polymorphic &random
		) const = 0;

		virtual Colour evaluate(
			Double3 const& evaluate_direction,
			Ray::Intersection const& idata
		) const = 0;

	};

};
