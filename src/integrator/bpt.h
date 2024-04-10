#pragma once

#include "../integrator/polymorphic.h"
#include "../mathematics/double3.h"
#include "../geometry/polymorphic.h"
#include <vector>
#include "../colour/colour.h"
#include <map>
#include "../integrator/vertex.h"
#include "../ray/section.h"
#include <utility>
#include <cmath>
#include "../mathematics/orthogonal.h"
#include "../bxdf/common.h"
#include <cstdint>
#include "../render/scene.h"
#include <memory>
#include "../ray/intersection.h"
#include "../mathematics/constant.h"
#include "../random/polymorphic.h"
#include <type_traits>
#include "../render/config.h"
#include <omp.h>
#include "../bxdf/polymorphic.h"
#include "../mathematics/epsilon.h"

namespace Integrator
{

	// Bi-directional path tracing, 1993
	// Eric P.Lafortune, Yves D.Willems

	class BPT final : public Integrator::Polymorphic
	{

	private:

		std::unique_ptr<Random::Polymorphic> p_random{ nullptr };

		uint8_t const max_depth{ 1 };
		uint16_t const max_samples{ 1 };

		Render::Scene scene;

	public:

		BPT(
			Render::Scene const& scene,
			Render::Config const& config,
			std::unique_ptr<Random::Polymorphic>& p_random

		)
			: scene( scene ), p_random( std::move( p_random ) ), max_depth( config.max_depth ), max_samples( config.max_samples )
		{};

		Colour process(
			uint16_t const& x,
			uint16_t const& y
		) const override
		{
			Colour accumulate( Colour::Black );
			for ( uint16_t s = 0; s < max_samples; ++s )
			{
				// In the paper light start is part of the light path
				std::vector<Integrator::Vertex> light_start;
				std::vector<Integrator::Vertex> light_path;

				for ( uint8_t i = 0; i < scene.n_light(); ++i )
				{
					auto [energy, point, direction, normal] = scene.light( i ).emit( *p_random );
					light_start.emplace_back( Vertex( point, normal, energy ) );

					auto sub_path = emission_path( Ray::Section( point, direction ), energy * direction.dot( normal ) );

					if ( sub_path.size() > 0 )
						light_path.insert( std::end( light_path ), std::begin( sub_path ), std::end( sub_path ) );
				}
				Ray::Section ray = scene.camera_ray( x, y, s );
				accumulate += camera_path( ray, light_start, light_path );
			}
			return accumulate / static_cast<float>( max_samples );
		};

	private:

		std::vector<Integrator::Vertex> emission_path(
			Ray::Section ray,
			Colour throughput
		) const
		{
			std::vector<Integrator::Vertex> light_path;
			uint8_t depth{ 0 };
			while ( 1 )
			{
				auto [f_hit, hit_distance, idata] = scene.intersect( ray );
				if ( !f_hit )
					break;

				auto [bxdf_colour, bxdf_direction, bxdf_event] = scene.material( idata.material_id )->sample( idata, *p_random );

				if ( ( bxdf_event == BxDF::Event::None ) || ( bxdf_event == BxDF::Event::Emission ) )
					break;

				if ( ( bxdf_event == BxDF::Event::Diffuse ) )
					light_path.emplace_back( Integrator::Vertex( idata, throughput ) );

				if ( ++depth >= max_depth )
					break;

				throughput *= bxdf_colour;
				ray = Ray::Section( idata.point + idata.normal * 0.01, bxdf_direction );
			}

			return light_path;
		};

		Colour camera_path(
			Ray::Section ray,
			std::vector<Integrator::Vertex> const& light_start,
			std::vector<Integrator::Vertex> const& light_path
		) const
		{
			// if last hit was diffuse, don't sample lights
			bool f_prev_event_dirac = true;
			// Accumulated emissions, Cij
			Colour accumulate( Colour::Black );
			// State of path colour after each bounce
			Colour throughput( Colour::White );

			uint8_t depth{ 0 };
			while ( 1 )
			{
				auto [f_hit, hit_distance, idata] = scene.intersect( ray );
				if ( !f_hit )
					break;

				BxDF::Polymorphic const& material( *scene.material( idata.material_id ) );

				auto [bxdf_colour, bxdf_direction, bxdf_event] = material.sample( idata, *p_random );

				if ( bxdf_event == BxDF::Event::None )
					break;

				if ( bxdf_event == BxDF::Event::Emission )
				{
					// C00, if depth==0
					// If prev event was diffuse, an emitter have already been sample
					if ( f_prev_event_dirac )
						accumulate += throughput * bxdf_colour;
					break;
				}

				f_prev_event_dirac = true;
				if ( bxdf_event == BxDF::Event::Diffuse )
				{
					f_prev_event_dirac = false;

					// C0j, j>0
					Colour explicit_light{ Colour::Black };
					for ( uint32_t i = 0; i < light_start.size(); ++i )
					{
						Double3 const diff = light_start[ i ].idata.point - idata.point;
						Double3 const direction = diff.normalise();
						// Direction is pointing in the "wrong" direction at light_start, hence the minus
						double const cos_theta = -( direction.dot( light_start[ i ].idata.normal ) );
						if ( cos_theta > 0. )
						{
							double const distance = diff.magnitude();
							Colour bxdf_eval = material.evaluate( direction, idata );

							if ( !bxdf_eval.is_black() )
								if ( !scene.occluded( Ray::Section( idata.point, direction ), distance - EPSILON_DISTANCE ) )
									explicit_light += light_start[ i ].throughput * bxdf_eval * ( cos_theta / ( distance * distance ) );
						}
					}
					accumulate += throughput * explicit_light;

					// Cij, i>0 j>0
					Colour implicit_light{ Colour::Black };
					for ( uint32_t i = 0; i < light_path.size(); ++i )
					{
						Double3 diff = light_path[ i ].idata.point - idata.point;
						Double3 direction = diff.normalise();
						double distance = diff.magnitude();
						if ( !scene.occluded( Ray::Section( idata.point, direction ), distance - EPSILON_DISTANCE ) )
						{
							Colour bxdf = material.evaluate( direction, idata );
							Colour path = scene.material( light_path[ i ].idata.material_id )->evaluate( -direction, light_path[ i ].idata );
							implicit_light += light_path[ i ].throughput * bxdf * path / ( distance * distance );
						}
					}
					accumulate += throughput * implicit_light;
				}

				if ( ++depth >= max_depth )
					break;

				throughput *= bxdf_colour;
				ray = Ray::Section( idata.point + idata.normal * 0.01, bxdf_direction );
			}
			return accumulate;
		};

	}; // end pbt class

};