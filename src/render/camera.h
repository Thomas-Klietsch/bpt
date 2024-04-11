#pragma once

#include <cfloat>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <tuple>
#include <vector>

#include "../mathematics/constant.h"
#include "../mathematics/double3.h"
#include "../random/polymorphic.h"
#include "../ray/section.h"
#include "../render/config.h"

namespace Render
{

	// Pin hole camera.
	// In the real world, the image plane is behind the pin hole.
	// But it is simpler to visualise when in front of it.
	class Camera final
	{

	private:

		uint16_t image_width{ 160 };
		uint16_t image_height{ 90 };
		uint16_t max_samples{ 1 };

		Double3 position{ Double3::Zero };

		Double3 forward{ Double3::Y };
		Double3 right{ Double3::X };
		Double3 up{ Double3::Z };

		std::vector<std::tuple<float, float>> random;

	public:

		Camera() {};

		Camera(
			Double3 const& position,
			Double3 const& look_at,
			Render::Config const& config,
			std::unique_ptr<Random::Polymorphic>& p_random
		)
			: position( position ), image_width( config.image_width ), image_height( config.image_height ), max_samples( config.max_samples )
		{
			float const aspect_ratio = static_cast<float>( image_width ) / static_cast<float>( image_height );
			float const tan_fov = std::tan( 35.f * deg_to_rad );
			Double3 const diff = look_at - position;
			if ( diff.magnitude() < FLT_EPSILON )
				std::cout << "Camera position and view target are too close together!" << std::endl;
			else
				forward = diff.normalise();

			// If view direction and World Up is collinear (or close to it), change World up axis.
			Double3 world_up = std::abs( forward.dot( Double3::Z ) ) < 0.99 ? Double3::Z : Double3::X;
			right = forward.cross( world_up ) * aspect_ratio * tan_fov;
			// Modern image formats/programmes have (0,0) at the top left, up is flipped
			up = -( right.cross( forward ) ).normalise() * tan_fov;

			// All camera paths use the same sequential offsets.
			for ( uint16_t i = 0; i < max_samples; ++i )
			{
				auto [e1, e2] = p_random->get_float2();
				random.push_back( { e1 - 0.5, e2 - 0.5 } );
			}
		};

		Ray::Section generate_ray(
			uint16_t const& x,
			uint16_t const& y,
			uint16_t const& sample
		) const
		{
			auto [e1, e2] = random[ sample % max_samples ];

			Double3 dir = forward +
				right * ( ( static_cast<float>( x ) + e1 ) / static_cast<float>( image_width - 1 ) - 0.5 ) +
				up * ( ( static_cast<float>( y ) + e2 ) / static_cast<float>( image_height - 1 ) - 0.5 );

			return Ray::Section( position, dir.normalise() );
		};

	};

};
