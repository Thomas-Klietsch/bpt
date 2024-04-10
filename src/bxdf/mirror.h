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

		std::string name;

	public:

		Mirror() = delete;

		Mirror(
			std::string const& name
		)
			: name( name )
		{};

		std::tuple<Colour, Double3, BxDF::Event> sample(
			Ray::Intersection const& idata,
			Random::Polymorphic &random
		) const override
		{
			Double3 const wsample_local( -idata.local_wray.x, -idata.local_wray.y, idata.local_wray.z );

			return { Colour::White, idata.orthogonal.to_world( wsample_local ), BxDF::Event::Dirac };
		};

		Colour evaluate(
			Double3 const& evaluate_direction,
			Ray::Intersection const& idata
		) const override
		{
			return Colour::Black;
		};

		std::string const& bxdf_name() const override
		{
			return name;
		};

	};

};

