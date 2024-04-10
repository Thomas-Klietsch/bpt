#pragma once

#include <random>
#include <cstdlib>

#include "../random/polymorphic.h"
#include <utility>
#include <tuple>
#include <cstdint>

namespace Random
{

	class Rand final : public Random::Polymorphic
	{

	private:

	public:

		Rand() {};

		Rand(uint32_t const& seed) {};

		float get_float() override
		{
			return std::rand() / static_cast<float>( RAND_MAX );
		};

		std::tuple<float, float> get_float2() override
		{
			return { std::rand() / static_cast<float>( RAND_MAX ), std::rand() / static_cast<float>( RAND_MAX ) };
		};

	};

};
