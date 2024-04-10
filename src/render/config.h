#pragma once

#include <cstdint>

namespace Render
{

	struct Config
	{
		// Image resolution
		uint16_t image_width = 800;
		uint16_t image_height = 800;
		// Path length of traces, i.e. how many surface bounces
		uint16_t max_depth = 5;
		// Samples per pixels
		uint16_t max_samples = 1;

		Config() = default;

		Config(
			uint16_t const& image_width,
			uint16_t const& image_height,
			uint16_t const& max_depth,
			uint16_t const& max_samples
		)
			: image_width( image_width ), image_height( image_height ), max_depth( max_depth ), max_samples( max_samples )
		{};

	};

};
