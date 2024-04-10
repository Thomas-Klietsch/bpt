#pragma once

#include <cstdint>

#include "../colour/colour.h"

namespace Integrator
{

	class Polymorphic
	{

	public:

		Polymorphic() {};

		virtual Colour process( uint16_t const& x, uint16_t const& y ) const = 0;

	};

};

