#pragma once

#include <tuple>

#include "../bxdf/common.h"
#include "../bxdf/polymorphic.h"
#include "../colour/colour.h"
#include "../mathematics/double3.h"
#include "../random/polymorphic.h"
#include "../ray/intersection.h"

namespace BxDF
{

	class Emission final : public BxDF::Polymorphic
	{

	private:

		Colour energy;

	public:

		Emission() = delete;

		Emission(
			Colour const& energy
		)
			: energy( energy )
		{};

		std::tuple<Colour, Double3, BxDF::Event> sample(
			Ray::Intersection const& idata,
			Random::Polymorphic &random
		) const override
		{
			// Direct hit on emitter is not affected by surface area,
			// nor does it generate a new direction
			return { energy, {}, BxDF::Event::Emission };
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
