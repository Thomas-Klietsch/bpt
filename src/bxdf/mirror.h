#pragma once

#include <string>
#include <tuple>

#include "../bxdf/common.h"
#include "../bxdf/polymorphic.h"
#include "../colour/colour.h"
#include "../mathematics/double3.h"
#include "../mathematics/double3.h"
#include "../random/polymorphic.h"
#include "../ray/intersection.h"

namespace BxDF
{

	// Not physical based
	class Mirror final : public BxDF::Polymorphic
	{

	private:

		Colour reflectance;

	public:

		Mirror() = delete;

		Mirror( Colour const& reflectance )
			: reflectance( reflectance )
		{};

		std::tuple<Colour, Double3, BxDF::Event> sample(
			Ray::Intersection const& idata,
			Random::Polymorphic &random
		) const override
		{
			Double3 const wsample_local( -idata.local_wray.x, -idata.local_wray.y, idata.local_wray.z );
			return { reflectance, idata.orthogonal.to_world( wsample_local ), BxDF::Event::Reflect };
		};

		Colour evaluate(
			Double3 const& evaluate_direction,
			Ray::Intersection const& idata
		) const override
		{
			return Colour::Black;
		};

	};

};

