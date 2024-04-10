#pragma once
#include <string>
#include <utility>

#include "../mathematics/double3.h"
#include "../colour/colour.h"
#include "../ray/intersection.h"
#include "../bxdf/common.h"
#include <tuple>
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

		virtual std::string const& bxdf_name() const = 0;

	};

};
