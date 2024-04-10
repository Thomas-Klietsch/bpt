#pragma once

#include <cmath>
#include <tuple>
#include <utility>

#include "../colour/colour.h"
#include "../emitter/polymorphic.h"
#include "../mathematics/double3.h"
#include "../mathematics/orthogonal.h"
#include "../random/polymorphic.h"
#include "../sample/hemisphere.h"

namespace Emitter
{

	class Triangle final : public Emitter::Polymorphic
	{

	private:

		Double3 position; // a
		Double3 edge1; // b-a
		Double3 edge2; // c-a
		Double3 normal; // edge1 cross edge2

		Orthogonal local_space;

		Colour energy;

		double area;

	public:

		Triangle(
			Double3 const& a,
			Double3 const& b,
			Double3 const& c,
			Colour const& energy
		) :
			position( a ), edge1( b - a ), edge2( c - a ), energy( energy )
		{
			normal = ( edge1.cross( edge2 ) ).normalise();
			local_space = Orthogonal( normal );
			area = .5 * ( edge1.cross( edge2 ) ).magnitude();
		};

		std::tuple<Colour, Double3, double> evaluate(
			Double3 const& point_illuminated,
			Random::Polymorphic& random
		) const override
		{
			auto const [e1, e2] = random.get_float2();
			float const temp = std::sqrt( e1 );
			float const u = 1.f - temp;
			float const v = e2 * temp;
			Double3 point = position + edge1 * u + edge2 * v;
			Double3 diff = point - point_illuminated;
			Double3 direction = ( diff ).normalise();
			double cos_theta = std::max( .0, -( normal.dot( direction ) ) );
			double distance = ( diff ).magnitude();
			return { energy * ( area * cos_theta / ( distance * distance ) ), direction, distance };
		};

		std::tuple <Colour, Double3, Double3, Double3> emit(
			Random::Polymorphic& random
		) const override
		{
			// https://extremelearning.com.au/evenly-distributing-points-in-a-triangle/
			auto const [e1, e2] = random.get_float2();
			float const e1_sqrt = std::sqrt( e1 );
			float const u = e2 * e1_sqrt;
			float const v = ( 1.f - e2 ) * e1_sqrt;
			Double3 point = position + edge1 * u + edge2 * v;
			Double3 direction = Sample::HemiSphere( random );
			return { energy * area, point + normal * 0.01, local_space.to_world( direction.normalise() ), normal };
		};

	};

};

