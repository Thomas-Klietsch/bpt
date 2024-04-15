#pragma once

#include <algorithm>
#include <utility>

#include "../epsilon.h"

struct Colour
{
	float r{ 0.f };
	float g{ 0.f };
	float b{ 0.f };

	Colour() {};

	Colour( float const& r, float const& g, float const& b ) : r( r ), g( g ), b( b ) {};

	Colour operator + ( Colour const& value ) const { return Colour( r + value.r, g + value.g, b + value.b ); };

	Colour operator * ( Colour const& value ) const { return Colour( r * value.r, g * value.g, b * value.b ); };
	Colour operator * ( float const& value ) const { return Colour( r * value, g * value, b * value ); };

	Colour operator / ( float const& value ) const { return Colour( r / value, g / value, b / value ); };

	Colour operator = ( Colour const& value ) { r = value.r; g = value.g; b = value.b; return *this; };
	Colour operator += ( Colour const& value ) { r += value.r; g += value.g, b += value.b; return *this; };
	Colour operator *= ( Colour const& value ) { r *= value.r; g *= value.g, b *= value.b; return *this; };

	bool is_black() const { return std::max( { r, g, b } ) < EPSILON_BLACK; };

	// Limit values to [0;1]
	Colour clamp()
	{
		r = std::clamp( r, 0.f, 1.f );
		g = std::clamp( g, 0.f, 1.f );
		b = std::clamp( b, 0.f, 1.f );
		return *this;
	};

	// Limit values to [0;inf[
	Colour clip()
	{
		r = std::max( r, 0.f );
		g = std::max( g, 0.f );
		b = std::max( b, 0.f );
		return *this;
	};

	Colour static const Black;
	Colour static const White;
	Colour static const Red;
	Colour static const Green;
	Colour static const Blue;
};

Colour const Colour::Black( 0.0f, 0.0f, 0.0f );
Colour const Colour::White( 1.0f, 1.0f, 1.0f );
Colour const Colour::Red( 1.0f, 0.0f, 0.0f );
Colour const Colour::Green( 0.0f, 1.0f, 0.0f );
Colour const Colour::Blue( 0.0f, 0.0f, 1.0f );
