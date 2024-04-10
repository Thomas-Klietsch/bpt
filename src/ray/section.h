#pragma once

#include "../mathematics/double3.h"

namespace Ray
{

	class Section final
	{

	public:

		Double3 origin;
		Double3 direction;

		Section() : origin( Double3( 0, 0, 0 ) ), direction( Double3( 0, 0, 0 ) ) {};

		Section(
			Double3 const& origin,
			Double3 const& direction
		)
			: origin( origin ), direction( direction )
		{};

	};

};
