#pragma once

#include <string>
#include <tuple>

#include "../bxdf/polymorphic.h"
#include "../mathematics/double3.h"
#include "../ray/intersection.h"
#include "../mathematics/constant.h"
#include <cmath>
#include "../bxdf/common.h"
#include "../colour/colour.h"
#include "../random/polymorphic.h"
#include "../sample/sphere.h"
#include "../sample/hemisphere.h"

namespace BxDF
{

	class Lambert final : public BxDF::Polymorphic
	{

	private:

		std::string name;
		Colour albedo;

	public:

		Lambert() = delete;

		Lambert(
			std::string const& name,
			Colour const& albedo
		)
			: name( name ), albedo( albedo )
		{};

		std::tuple<Colour, Double3, BxDF::Event> sample(
			Ray::Intersection const& idata,
			Random::Polymorphic& random
		) const override
		{
			Double3 const sample_direction = Sample::HemiSphere( random );
			return { albedo * inv_pi * sample_direction.z, idata.orthogonal.to_world( sample_direction ), BxDF::Event::Diffuse };
		};

		Colour evaluate(
			Double3 const& evaluate_direction,
			Ray::Intersection const& idata
		) const override
		{
			// One sided material
			double const cos_theta = evaluate_direction.dot( idata.normal );
			return cos_theta > 0 ? albedo * inv_pi * cos_theta : Colour::Black;
		};

		std::string const& bxdf_name() const override
		{
			return name;
		};

	};

};
