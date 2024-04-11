#pragma once

#include <cstdint>
#include <memory>
#include <tuple>
#include <vector>

#include "../bxdf/emission.h"
#include "../bxdf/lambert.h"
#include "../bxdf/mirror.h"
#include "../bxdf/polymorphic.h"
#include "../colour/colour.h"
#include "../emitter/polymorphic.h"
#include "../emitter/triangle.h"
#include "../geometry/polymorphic.h"
#include "../geometry/triangle.h"
#include "../mathematics/double3.h"
#include "../random/polymorphic.h"
#include "../random/rand.h"
#include "../ray/intersection.h"
#include "../ray/section.h"
#include "../render/camera.h"
#include "../render/config.h"

namespace Render
{

	class Scene final
	{

	private:

		std::vector< std::shared_ptr<Geometry::Polymorphic> > geometry;
		uint32_t n_geometry{ 0 };

		std::vector< std::shared_ptr<Emitter::Polymorphic> > emitter;
		uint32_t n_emitter{ 0 };

		std::vector< std::shared_ptr<BxDF::Polymorphic> > bxdf;

		Render::Camera camera;

	public:

		Scene() = delete;

		Scene(
			Render::Config const& config
		)
		{
			std::unique_ptr<Random::Polymorphic> p_random = std::make_unique< Random::Rand>();
			camera = Render::Camera( Double3( -278, -800, 273 ), Double3( -278, 0, 273 ), config, p_random );

			bxdf.emplace_back( std::make_shared<BxDF::Lambert>( Colour( .8f, .8f, .8f ) ) ); // White
			bxdf.emplace_back( std::make_shared<BxDF::Lambert>( Colour( 0.8f, 0.15f, 0.15f ) ) ); // Red
			bxdf.emplace_back( std::make_shared<BxDF::Lambert>( Colour( 0.16f, 0.8f, 0.17f ) ) ); // Green

			// Cornell (big box)
			Double3 cbox[ 8 ] = {
				Double3( 0.0, 0.0, 0.0 ),
				Double3( 0.0, 0.0, 548.8 ),
				Double3( 0.0, 559.2, 0.0 ),
				Double3( 0.0, 559.2, 548.8 ),
				Double3( -552.8, 0.0, 0.0 ),
				Double3( -556.0, 0.0, 548.8 ),
				Double3( -549.6, 559.2, 0.0 ),
				Double3( -556.0, 559.2, 548.8 ),
			};
			// Back
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( cbox[ 2 ], cbox[ 3 ], cbox[ 7 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( cbox[ 2 ], cbox[ 7 ], cbox[ 6 ], 0 ) );
			// Top
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( cbox[ 1 ], cbox[ 5 ], cbox[ 7 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( cbox[ 1 ], cbox[ 7 ], cbox[ 3 ], 0 ) );
			// Bottom
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( cbox[ 0 ], cbox[ 2 ], cbox[ 6 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( cbox[ 0 ], cbox[ 6 ], cbox[ 4 ], 0 ) );
			// Left
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( cbox[ 4 ], cbox[ 6 ], cbox[ 7 ], 1 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( cbox[ 4 ], cbox[ 7 ], cbox[ 5 ], 1 ) );
			// Right
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( cbox[ 0 ], cbox[ 1 ], cbox[ 3 ], 2 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( cbox[ 0 ], cbox[ 3 ], cbox[ 2 ], 2 ) );

			// Short block
			Double3 sbox[ 8 ] =
			{
				Double3( -82.0, 225.0, 0.0 ),
				Double3( -82.0, 225.0, 165.0 ),
				Double3( -130.0, 65.0, 0.0 ),
				Double3( -130.0, 65.0, 165.0 ),
				Double3( -240.0, 272.0, 0.0 ),
				Double3( -240.0, 272.0, 165.0 ),
				Double3( -290.0, 114.0, 0.0 ),
				Double3( -290.0, 114.0, 165.0 )
			};
			// Back
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( sbox[ 4 ], sbox[ 5 ], sbox[ 1 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( sbox[ 4 ], sbox[ 1 ], sbox[ 0 ], 0 ) );
			// Front
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( sbox[ 2 ], sbox[ 3 ], sbox[ 7 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( sbox[ 2 ], sbox[ 7 ], sbox[ 6 ], 0 ) );
			// Top
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( sbox[ 3 ], sbox[ 1 ], sbox[ 5 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( sbox[ 3 ], sbox[ 5 ], sbox[ 7 ], 0 ) );
			// Left
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( sbox[ 6 ], sbox[ 7 ], sbox[ 5 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( sbox[ 6 ], sbox[ 5 ], sbox[ 4 ], 0 ) );
			// Right
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( sbox[ 0 ], sbox[ 1 ], sbox[ 3 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( sbox[ 0 ], sbox[ 3 ], sbox[ 2 ], 0 ) );

			// Tall block
			Double3 tbox[ 8 ] =
			{
				Double3( -265.0, 296.0, 0.0 ),
				Double3( -265.0, 296.0, 330.0 ),
				Double3( -314.0, 456.0, 0.0 ),
				Double3( -314.0, 456.0, 330.0 ),
				Double3( -423.0, 247.0, 0.0 ),
				Double3( -423.0, 247.0, 330.0 ),
				Double3( -472.0, 406.0, 0.0 ),
				Double3( -472.0, 406.0, 330.0 )
			};
			// Back
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 6 ], tbox[ 7 ], tbox[ 3 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 6 ], tbox[ 3 ], tbox[ 2 ], 0 ) );
			// Front
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 0 ], tbox[ 1 ], tbox[ 5 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 0 ], tbox[ 5 ], tbox[ 4 ], 0 ) );
			// Top
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 5 ], tbox[ 1 ], tbox[ 3 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 5 ], tbox[ 3 ], tbox[ 7 ], 0 ) );
			// Left
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 4 ], tbox[ 5 ], tbox[ 7 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 4 ], tbox[ 7 ], tbox[ 6 ], 0 ) );
			// Right
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 2 ], tbox[ 3 ], tbox[ 1 ], 0 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 2 ], tbox[ 1 ], tbox[ 0 ], 0 ) );

			// Offset to avoid "z fighting"
			Double3 light[ 4 ] =
			{
				Double3( -213.0, 227.0, 548.8 - 0.01 ),
				Double3( -213.0, 332.0, 548.8 - 0.01 ),
				Double3( -343.0, 227.0, 548.8 - 0.01 ),
				Double3( -343.0, 332.0, 548.8 - 0.01 ),
			};

			Colour energy = ( Colour( 0.f, .929f, .659f ) * 8.f + Colour( 1.f, .447f, .0f ) * 15.6f + Colour( 0.376f, 0.f, 0.f ) * 18.4f );
			bxdf.emplace_back( std::make_shared<BxDF::Emission>( energy ) );

			geometry.emplace_back( std::make_shared<Geometry::Triangle>( light[ 2 ], light[ 3 ], light[ 1 ], 3 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( light[ 2 ], light[ 1 ], light[ 0 ], 3 ) );

			emitter.emplace_back( std::make_shared<Emitter::Triangle>( light[ 2 ], light[ 3 ], light[ 1 ], energy ) );
			emitter.emplace_back( std::make_shared<Emitter::Triangle>( light[ 2 ], light[ 1 ], light[ 0 ], energy ) );

			bxdf.emplace_back( std::make_shared<BxDF::Mirror>( Colour::White ) );

			n_geometry = static_cast<uint32_t>( geometry.size());
			n_emitter = static_cast<uint32_t>( emitter.size());
		};

		std::tuple<bool, double, Ray::Intersection> intersect( Ray::Section const& ray ) const
		{
			double distance = 1e20;
			uint32_t object_id = 0;
			for ( uint32_t i = 0; i < n_geometry; ++i )
			{
				double d = geometry[ i ]->intersect( ray );
				if ( d > 0.0 && d < distance )
				{
					distance = d;
					object_id = i;
				}
			}

			if ( distance >= 1e19 )
				return { false, {}, {} };

			auto idata = geometry[ object_id ]->post_intersect( ray, distance, 0, {} );
			//idata.object_id = object_id;
			return { true, distance, idata };
		};

		bool occluded( Ray::Section const& ray, double const& distance ) const
		{
			for ( uint32_t i = 0; i < n_geometry; ++i )
			{
				double d = geometry[ i ]->intersect( ray );
				if ( d > 0.0 && d < distance )
					return true;
			}
			return false;
		};

		BxDF::Polymorphic const* material( uint32_t const& id ) const
		{
			// TODO
			return bxdf[ id ].get();
		};

		Emitter::Polymorphic const& light( uint32_t const& id ) const
		{
			// TODO
			return *emitter[ id ];
		};

		Ray::Section camera_ray(
			uint16_t const& x,
			uint16_t const& y,
			uint16_t const& sample
		) const
		{
			return camera.generate_ray( x, y, sample );
		};

		uint32_t n_object() const { return n_geometry; };
		uint32_t n_light() const { return n_emitter; };

	};

};
