#pragma once

#include <numbers>

// pi
constexpr float pi = static_cast<float>( std::numbers::pi );

// 1 / pi
constexpr float inv_pi = static_cast<float>( std::numbers::inv_pi );

// 2 * pi
constexpr float two_pi = static_cast<float>( std::numbers::pi * 2. );

// Convert degree to radian
constexpr float deg_to_rad = static_cast<float>( std::numbers::pi / 180. );
