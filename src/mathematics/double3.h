#pragma once

#include <cmath>

class Double3 final
{

public:

	double x, y, z;

	Double3() : x( 0.0 ), y( 0.0 ), z( 0.0 ) {};

	Double3( double const& x, double const& y, double const& z ) : x( x ), y( y ), z( z ) {};

	// Unary minus
	Double3 operator - () const { return Double3( -x, -y, -z ); };

	Double3 operator + ( Double3 const& value ) const { return Double3( x + value.x, y + value.y, z + value.z ); };
	Double3 operator - ( Double3 const& value ) const { return Double3( x - value.x, y - value.y, z - value.z ); };
	Double3 operator * ( double const& value ) const { return Double3( x * value, y * value, z * value ); };
	Double3 operator / ( double const& value ) const { return Double3( x / value, y / value, z / value ); };

	Double3& operator = ( Double3 const& value ) { x = value.x; y = value.y; z = value.z; return *this; };

	Double3 normalise() const { return Double3( x, y, z ) / std::sqrt( x * x + y * y + z * z ); };

	double dot( Double3 const& value ) const { return x * value.x + y * value.y + z * value.z; };
	Double3 cross( Double3 const& value ) const { return Double3( y * value.z - z * value.y, z * value.x - x * value.z, x * value.y - y * value.x ); };
	double magnitude() const { return std::sqrt( x * x + y * y + z * z ); };

	Double3 static const Zero;
	Double3 static const One;
	Double3 static const X;
	Double3 static const Y;
	Double3 static const Z;

};

// ( 0,0,0 )
Double3 const Double3::Zero( 0.0, 0.0, 0.0 );
// ( 1,1,1 )
Double3 const Double3::One( 1.0, 1.0, 1.0 );
// ( 1,0,0 )
Double3 const Double3::X( 1.0, 0.0, 0.0 );
// ( 0,1,0 ) 
Double3 const Double3::Y( 0.0, 1.0, 0.0 );
// ( 0,0,1 )
Double3 const Double3::Z( 0.0, 0.0, 1.0 );
