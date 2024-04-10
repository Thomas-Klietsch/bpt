#pragma once

#include "../ray/section.h"
#include "../mathematics/double3.h"
#include "../colour/colour.h"
#include <cstdint>
#include "../ray/intersection.h"

namespace Geometry
{

	class Polymorphic
	{

	public:

		virtual double intersect(
			const Ray::Section& ray
		) const = 0;

		virtual Ray::Intersection post_intersect(
			Ray::Section const& ray,
			double const& distance,
			uint32_t const& polygon_id,
			Double3 const& provisional_data
		) const = 0;

	};

};
