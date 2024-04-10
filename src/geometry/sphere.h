#pragma once

#include <cmath>
#include <cstdint>

#include "../geometry/polymorphic.h"
#include "../colour/colour.h"
#include "../mathematics/double3.h"
#include "../ray/section.h"
#include "../ray/intersection.h"
#include "../mathematics/orthogonal.h"

namespace Geometry
{

	class Sphere final : public Geometry::Polymorphic
	{

	private:

		double radius;
		Double3 position;
		uint32_t material_id;

	public:

		Sphere(
			Double3 const& position,
			double const& radius,
			uint32_t const& material_id
		) :
			position( position ), radius( radius ), material_id( material_id )
		{};

		double intersect(
			const Ray::Section& ray
		) const override
		{
			// Check for ray-sphere intersection by solving for t:
			// t^2*d.d + 2*t*(o-p).d + (o-p).(o-p) - R^2 = 0
			Double3 op = position - ray.origin;
			double eps = 1e-4;
			double b = op.dot( ray.direction );
			double radicant = b * b - op.dot( op ) + radius * radius;
			if ( radicant < 0.0 )
				return -1.0;// No intersection
			else
				radicant = std::sqrt( radicant );

			double t= b - radicant;// Check smaller root first
			if ( t > eps )
				return t;

			t = b + radicant;
			if ( t > eps )
				return t;// Check second root

			return -2.0;// No intersection in ray direction
		};

		Ray::Intersection post_intersect(
			Ray::Section const& ray,
			double const& distance,
			uint32_t const& polygon_id,
			Double3 const& provisionary_data
		) const override
		{
			Ray::Intersection idata;
			idata.point = ray.origin + ray.direction * distance;
			idata.normal = ( idata.point - position ).normalise();
			idata.orthogonal = Orthogonal( idata.normal );
			idata.local_wray = idata.orthogonal.to_local( -ray.direction );
			idata.material_id = material_id;

			return idata;
		};

	};

};
