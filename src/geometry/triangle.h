#pragma once

#include <cstdint>
#include <cstdlib>

#include "../geometry/polymorphic.h"
#include "../mathematics/double3.h"
#include "../mathematics/orthogonal.h"
#include "../ray/intersection.h"
#include "../ray/section.h"

namespace Geometry
{

	class Triangle final : public Geometry::Polymorphic
	{

	private:

		Double3 position; // a
		Double3 edge1; // b-a
		Double3 edge2; // c-a
		Double3 normal; // edge1 cross edge2

		Orthogonal orthogonal;

		uint32_t material_id;

	public:

		Triangle() = delete;

		Triangle(
			Double3 const& a,
			Double3 const& b,
			Double3 const& c,
			uint32_t const& material_id
		) :
			position( a ), edge1( b - a ), edge2( c - a ), material_id( material_id )
		{
			normal = ( edge1.cross( edge2 ) ).normalise();
			orthogonal = Orthogonal( normal );
		};

		double intersect(
			Ray::Section const& ray
		) const override
		{
			// Möller-Trumbore intersection algorithm
			// Fast, minimum storage ray/triangle intersection, 1997

			// Calculating determinant
			Double3 p = ray.direction.cross( edge2 );
			double d = edge1.dot( p );

			// If determinant is near zero, ray lies in plane of triangle
			if ( std::abs(d) < 0.000001 ) // TODO magic constant
				return -1.0;

			double inv_d = 1.0 / d;

			// Calculate distance from triangle position to ray origin

			Double3 diff = ray.origin - position;
			// Calculate u parameter and test bound
			double u = diff.dot( p) * inv_d;
			if ( ( u < 0 ) || ( u > 1 ) )
				return -2.0;

			// Calculate v parameter and test bound
			Double3 q = diff.cross( edge1 );
			double v = ray.direction.dot( q ) * inv_d;
			if ( ( v < 0 ) || ( u + v > 1 ) )
				return -3.0;

			double t = q.dot( edge2 ) * inv_d;

			if ( t < 0.000001 ) // TODO magic constant
				return -4.0;

			return t;
		};

		Ray::Intersection post_intersect(
			Ray::Section const& ray,
			double const& distance
		) const override
		{
			Ray::Intersection idata;
			idata.point = ray.origin + ray.direction * distance;
			idata.normal = normal;
			idata.orthogonal = orthogonal;
			idata.local_wray = idata.orthogonal.to_local( -ray.direction );
			idata.material_id = material_id;

			return idata;
		}

	};

};
