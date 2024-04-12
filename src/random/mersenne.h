#pragma once

#include <cstdint>
#include <tuple>
#include <utility>

#include "../random/polymorphic.h"

namespace Random
{

	class Mersenne final : public Random::Polymorphic
	{

	private:

		float const randmaxf = 1.f / 4294967295.f;

		// Random seed found by rolling a d20
		uint32_t seed{ 11 };

	public:

		Mersenne() = delete;

		Mersenne( uint32_t const& seed )
			: seed( seed )
		{};

		float get_float() override
		{
			return next() * randmaxf;
		};

		std::tuple<float, float> get_float2() override
		{
			return std::tuple( next() * randmaxf, next() * randmaxf );
		};

	private:

		inline uint32_t next()
		{
			uint32_t x = seed = ( 1812433253U * ( seed ^ ( seed >> 30 ) ) + 1 ) & 0xFFFFFFFFU;
			x ^= ( x >> 11 );
			x ^= ( x << 7 ) & 0x9D2C5680U;
			x ^= ( x << 15 ) & 0xEFC60000U;
			return x ^= ( x >> 18 );
		};

	};

};
