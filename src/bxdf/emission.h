#pragma once

#include <string>
#include <tuple>

#include "../bxdf/polymorphic.h"
#include "../colour/colour.h"
#include "../mathematics/double3.h"
#include "../ray/intersection.h"
#include "../mathematics/constant.h"
#include <cmath>
#include "../bxdf/common.h"
#include <cstdint>
#include "../random/polymorphic.h"

namespace BxDF
{

	class Emission final : public BxDF::Polymorphic
	{

	private:

		Colour energy;

	public:

		Emission() = delete;

		Emission(
			std::string const& name,
			Colour const& energy
		)
			: energy( energy )
		{};

		std::tuple<Colour, Double3, BxDF::Event> sample(
			Ray::Intersection const& idata,
			Random::Polymorphic &random
		) const override
		{
			// Returns radiance
			return { energy, {}, BxDF::Event::Emission };
		};

		// TODO
		Colour evaluate(
			Double3 const& evaluate_direction,
			Ray::Intersection const& idata
		) const override
		{
			return Colour::Black;
		};

		std::string const& bxdf_name() const override
		{
			return ""; // TODO
		};

	};

};
