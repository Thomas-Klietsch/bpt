#pragma once

#include "../ray/intersection.h"
#include "../ray/section.h"

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
			double const& distance
		) const = 0;

	};

};
