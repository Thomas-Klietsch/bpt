#pragma once

#include <cstdint>
#include <stdlib.h>
#include <tuple>

#include "../random/polymorphic.h"

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
