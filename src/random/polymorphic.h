#pragma once

#include <tuple>
#include <utility>

namespace Random
{

	class Polymorphic
	{

	public:

		Polymorphic() {};

		virtual float get_float() = 0;

		virtual std::tuple<float, float> get_float2() = 0;

	};

};
