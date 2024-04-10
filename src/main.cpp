// Copyright (c) 2024 Thomas Klietsch, all rights reserved.
//
// Licensed under the GNU Lesser General Public License, version 3.0 or later
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or ( at your option ) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General
// Public License along with this program.If not, see < https://www.gnu.org/licenses/>. 

#include <chrono>
#include <cstdlib>
#include <iostream>

#include "render/config.h"
#include "render/image.h"
#include "render/scene.h"

int main( int argc, char* argv[] )
{
	Render::Config const config( 800, 800, 5, 1 );

	Render::Scene const scene( config );
	if ( !scene.n_light() || !scene.n_object() )
	{
		std::cout << "Nothing to render, no light and/or object(s)." << std::endl;
		return EXIT_FAILURE;
	}

	Render::Image image( scene, config );

	std::cout << "Render start." << std::endl;
	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

	image.render();

	std::chrono::steady_clock::time_point stop_time = std::chrono::steady_clock::now();
	std::chrono::milliseconds total_time = std::chrono::duration_cast<std::chrono::milliseconds>( stop_time - start_time );
	std::cout << "Render time: " << total_time.count() << " millie seconds." << std::endl;

	std::cout << "Saving image." << std::endl;
	if ( !image.save( "result" ) )
	{
		std::cout << "PANIC! Could not save image." << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Work complete." << std::endl;
	return EXIT_SUCCESS;
};
