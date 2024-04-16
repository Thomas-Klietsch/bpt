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
		uint32_t n_bxdf{ 0 };

		Render::Camera camera;

	public:

		Scene() = delete;

		Scene(
			Render::Config const& config
		)
		{
			camera = Render::Camera( Double3( -278, -800, 273 ), Double3( -278, 0, 273 ), config );

			bxdf.emplace_back( std::make_shared<BxDF::Lambert>( Colour( .8f, .8f, .8f ) ) ); // White
			bxdf.emplace_back( std::make_shared<BxDF::Lambert>( Colour( 0.6f, 0.01f, 0.01f ) ) ); // Red
			bxdf.emplace_back( std::make_shared<BxDF::Lambert>( Colour( 0.01f, 0.25f, 0.01f ) ) ); // Green

			bxdf.emplace_back( std::make_shared<BxDF::Mirror>( Colour::White ) );

			// Energy is split over two equal triangles
			Colour energy = ( Colour( 0.f, .929f, .659f ) * 8.f + Colour( 1.f, .447f, .0f ) * 15.6f + Colour( 0.376f, 0.f, 0.f ) * 18.4f ) * 0.5;
			bxdf.emplace_back( std::make_shared<BxDF::Emission>( energy ) );

			uint32_t const tall_block_material = 0; // 3 for mirror

			// The Cornell Box
			// https://www.graphics.cornell.edu/online/box/

			// Note that the order, and sign, of the data is altered here, as world up is the Z axis.

			// Cornell (big box)
			Double3 const cbox[ 8 ] = {
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
			Double3 const sbox[ 8 ] =
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
			Double3 const tbox[ 8 ] =
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
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 6 ], tbox[ 7 ], tbox[ 3 ], tall_block_material ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 6 ], tbox[ 3 ], tbox[ 2 ], tall_block_material ) );
			// Front
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 0 ], tbox[ 1 ], tbox[ 5 ], tall_block_material ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 0 ], tbox[ 5 ], tbox[ 4 ], tall_block_material ) );
			// Top
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 5 ], tbox[ 1 ], tbox[ 3 ], tall_block_material ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 5 ], tbox[ 3 ], tbox[ 7 ], tall_block_material ) );
			// Left
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 4 ], tbox[ 5 ], tbox[ 7 ], tall_block_material ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 4 ], tbox[ 7 ], tbox[ 6 ], tall_block_material ) );
			// Right
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 2 ], tbox[ 3 ], tbox[ 1 ], tall_block_material ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( tbox[ 2 ], tbox[ 1 ], tbox[ 0 ], tall_block_material ) );

			// Offset to avoid "z fighting"
			Double3 const light[ 4 ] =
			{
				Double3( -213.0, 227.0, 548.8 - 0.01 ),
				Double3( -213.0, 332.0, 548.8 - 0.01 ),
				Double3( -343.0, 227.0, 548.8 - 0.01 ),
				Double3( -343.0, 332.0, 548.8 - 0.01 ),
			};
			// Visible emitters
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( light[ 2 ], light[ 3 ], light[ 1 ], 4 ) );
			geometry.emplace_back( std::make_shared<Geometry::Triangle>( light[ 2 ], light[ 1 ], light[ 0 ], 4 ) );
			// Emitters
			emitter.emplace_back( std::make_shared<Emitter::Triangle>( light[ 2 ], light[ 3 ], light[ 1 ], energy ) );
			emitter.emplace_back( std::make_shared<Emitter::Triangle>( light[ 2 ], light[ 1 ], light[ 0 ], energy ) );

			// Update
			n_geometry = static_cast<uint32_t>( geometry.size() );
			n_emitter = static_cast<uint32_t>( emitter.size() );
			n_bxdf = static_cast<uint32_t>( bxdf.size() );
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

			auto idata = geometry[ object_id ]->post_intersect( ray, distance );
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
			if ( id >= n_bxdf )
				return bxdf[ 0 ].get();
			return bxdf[ id ].get();
		};

		Emitter::Polymorphic const& light( uint32_t const& id ) const
		{
			// TODO
			if ( id >= n_emitter )
				return *emitter[ 0 ];
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
