#pragma once

#include "../colour/colour.h"
#include "../mathematics/double3.h"
#include "../ray/intersection.h"

namespace Integrator
{

	struct Vertex
	{
		Ray::Intersection idata;

		Colour throughput;

		Vertex() = default;

		// Emission path (materials)
		Vertex( Ray::Intersection const& idata, Colour const& throughput ) :
			idata( idata ),
			throughput( throughput )
		{};

		// Emission start
		Vertex( Double3 const& point, Double3 const& normal, Colour const& throughput ) :
			throughput( throughput )
		{
			idata.point = point;
			idata.normal = normal;
		};

	};

};
