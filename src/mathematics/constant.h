#pragma once

#include <limits>
#include <numbers>

// pi
constexpr float pi = static_cast<float>( std::numbers::pi );

// pi / 2
constexpr float pi_over_2 = static_cast<float>( std::numbers::pi * 0.5 );

// pi / 4
constexpr float pi_over_4 = static_cast<float>( std::numbers::pi * 0.25 );

// 1 / pi
constexpr float inv_pi = static_cast<float>( std::numbers::inv_pi );
// 1 / pi
constexpr float one_over_pi = static_cast<float>( std::numbers::inv_pi );

// 1 / ( 0.5 * pi )
constexpr float inv_half_pi = static_cast<float>( std::numbers::inv_pi * 2.0 );
// 2 / pi
constexpr float two_over_pi = static_cast<float>( std::numbers::inv_pi * 2.0 );

// 2 * pi
constexpr float two_pi = static_cast<float>( std::numbers::pi * 2. );

// 2 * pi * pi
constexpr float two_pipi = static_cast<float>( std::numbers::pi * std::numbers::pi * 2. );

// 1 / ( 2 * pi * pi )
constexpr float inv_two_pipi = static_cast<float>( std::numbers::inv_pi * std::numbers::inv_pi * 0.5 );

// 1 / ( 2 * pi )
constexpr float inv_2pi = static_cast<float>( std::numbers::inv_pi * 0.5 );
// 1 / ( 2 * pi )
constexpr float one_over_2pi = static_cast<float>( std::numbers::inv_pi * 0.5 );

// 1 / ( 4 * pi )
constexpr float inv_4pi = static_cast<float>( std::numbers::inv_pi * 0.25 );
// 1 / ( 4 * pi )
constexpr float one_over_4pi = static_cast<float>( std::numbers::inv_pi * 0.25 );

// 1 / sqrt( 2 * pi )
constexpr float inv_sqrt_2pi = static_cast<float>( std::numbers::inv_sqrtpi / std::numbers::sqrt2 );

// Square root of 0.5
constexpr float sqrt_05 = static_cast<float>( 1. / std::numbers::sqrt2 );

// Square root of 2.0
constexpr float sqrt_2 = static_cast<float>( std::numbers::sqrt2 );

// pdf of uniform sphere area sample
constexpr float sphere_pdf = static_cast<float>( std::numbers::inv_pi * 0.25 );

// pdf of uniform dome/hemishere area sample
constexpr float dome_pdf = static_cast<float>( std::numbers::inv_pi * 0.5 );

// pdf of uniform disk/disc/circle area sample
constexpr float disk_pdf = static_cast<float>( std::numbers::inv_pi );

// Convert radian to degree
constexpr float rad_to_deg = static_cast<float>( 180. / std::numbers::pi );

// Convert degree to radian
constexpr float deg_to_rad = static_cast<float>( std::numbers::pi / 180. );

// Convert byte (8 bit) to float
constexpr float byte_to_float = 1.f / 255.f;

// Convert word (16 bit) to float
constexpr float word_to_float = 1.f / 65535.f;

// Infinity, used for upper limit of ray section lengths (that do not intersect anything)
constexpr double infinity = std::numeric_limits<double>::infinity();
