#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iosfwd>
#include <memory>
#include <omp.h>
#include <omp.h>
#include <string>
#include <vector>

#include "../colour/colour.h"
#include "../integrator/bpt.h"
#include "../integrator/polymorphic.h"
#include "../random/mersenne.h"
#include "../random/polymorphic.h"
#include "../render/config.h"
#include "../render/scene.h"

namespace Render
{

	class Image final
	{

	private:

		std::vector<std::unique_ptr<Integrator::Polymorphic>> integrator;

		uint16_t image_width{ 0 };
		uint16_t image_height{ 0 };
		uint32_t n_pixel{ 0 };

		std::shared_ptr<Colour[]> image_data{ nullptr };

		// Fix for libgdk (Linux), if it detects TGA as ICO set this to true
		bool const f_libgdk = false;

	public:

		Image() = delete;

		Image(
			Render::Scene const& scene,
			Render::Config const& config
		)
			: image_width( config.image_width ), image_height( config.image_height ), n_pixel( config.image_width * config.image_height )
		{
			// Shared pointer, since unique_ptr can not use init value? (black)
			image_data = std::make_shared<Colour[]>( n_pixel, Colour::Black );
			for ( uint8_t i = 0; i < omp_get_max_threads(); ++i )
			{
				std::unique_ptr< Random::Polymorphic > random = std::make_unique< Random::Mersenne>( ( i + 0x1337 ) * 0xbeef );
				integrator.emplace_back( std::make_unique<Integrator::BPT>( scene, config, random ) );
			}
		};

		void render()
		{
			// Ignore Microsoft Visual Studio warning about omp
#pragma warning ( suppress: 6993 )
#pragma omp parallel for
			// Lazy arse parallel processing. This is terrible. xD
			for ( int y = 0; y < image_height; ++y )
				for ( int x = 0; x < image_width; ++x )
					image_data[ x + y * image_width ] = integrator[ omp_get_thread_num() ]->process( x, y );
		};

		bool save(
			std::string const& file_name
		)
		{
			// File output only
			std::ofstream tga_file;
			// If already open, delete content. Binary mode is needed
			tga_file.open( file_name + ".tga", std::ios::trunc | std::ios::binary );
			// Check if file is open
			if ( !tga_file.is_open() )
				return false;

			uint8_t const tga_header_size = 18 + ( f_libgdk ? 1 : 0 );
			uint32_t const tga_data_size = tga_header_size + n_pixel * 3;

			std::shared_ptr<uint8_t[]> p_data = std::make_shared<uint8_t[]>( tga_data_size, 0 );
			if ( !p_data )
				return false;

			// Set header
			// Comment data size
			p_data[ 0 ] = f_libgdk ? 1 : 0;
			// Colourmap type
			p_data[ 1 ] = 0;
			// Datatype
			p_data[ 2 ] = 2;
			// Colourmap origin
			p_data[ 3 ] = 0;
			p_data[ 4 ] = 0;
			// Colourmap length
			p_data[ 5 ] = 0;
			p_data[ 6 ] = 0;
			// Colourmap depth
			p_data[ 7 ] = 0;
			// X origin
			p_data[ 8 ] = 0;
			p_data[ 9 ] = 0;
			// Y origin
			p_data[ 10 ] = 0;
			p_data[ 11 ] = 0;
			// X size
			p_data[ 12 ] = static_cast<uint8_t>( image_width % 256 );
			p_data[ 13 ] = static_cast<uint8_t>( image_width / 256 );
			// Y size
			p_data[ 14 ] = static_cast<uint8_t>( image_height % 256 );
			p_data[ 15 ] = static_cast<uint8_t>( image_height / 256 );
			// Bits per pixel
			p_data[ 16 ] = 24;
			// Image descriptor
			// 32 (bit 5) is screen origin, 0 lower left, 1 upper left
			p_data[ 17 ] = 32;
			if ( f_libgdk )
				// Nonzero length bug fix for libgdk
				p_data[ 18 ] = 0;

			// Set image, TGA uses BGR colour order
			for ( uint32_t i = 0; i < n_pixel; ++i )
			{
				Colour& c = image_data[ i ];
				uint8_t r = static_cast<uint8_t>( std::pow( std::clamp( c.r, 0.f, 1.f ), 1.f / 2.2f ) * 255 );
				uint8_t g = static_cast<uint8_t>( std::pow( std::clamp( c.g, 0.f, 1.f ), 1.f / 2.2f ) * 255 );
				uint8_t b = static_cast<uint8_t>( std::pow( std::clamp( c.b, 0.f, 1.f ), 1.f / 2.2f ) * 255 );
				p_data[ i * 3 + tga_header_size ] = b;
				p_data[ i * 3 + 1 + tga_header_size ] = g;
				p_data[ i * 3 + 2 + tga_header_size ] = r;
			}

			// Dump p_data from memory to file
			tga_file.write( (char*)( &p_data[ 0 ] ), tga_data_size );
			tga_file.close();

			return true;
		};

	}; // end image class

};
