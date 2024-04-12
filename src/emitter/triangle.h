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

	public:

		Triangle() = delete;

		Triangle(
			Double3 const& a,
			Double3 const& b,
			Double3 const& c,
			Colour const& a_energy
		) :
			position( a ), edge1( b - a ), edge2( c - a )
		{
			Double3 const cross_product = edge1.cross( edge2 );
			normal = ( cross_product ).normalise();
			local_space = Orthogonal( normal );
			double area = .5 * ( cross_product ).magnitude();
			energy = ( a_energy * area ).clip();
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
			return { energy, point, local_space.to_world( direction.normalise() ), normal };
		};

	};

};

