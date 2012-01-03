/**

Peon-SDK
---------

Peon-SDK - Copyright (c) 2002-2011 Erik Yuzwa - erik@erikyuzwa.com

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment
   in the product documentation would be appreciated but
   is not required.

2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
   source distribution.
**/
#include "PeonVector2.h"

namespace peon
{
	Vector2::Vector2( float x_, float y_ )
	{
		x = x_;
		y = y_;

	}

	Vector2::~Vector2()
	{
	}

	void Vector2::set( float x_, float y_ )
	{
		x = x_;
		y = y_;

	}

	float Vector2::length( void )
	{
		return( (float)sqrt( x * x + y * y ) );
	}

	void Vector2::normalize( void )
	{
		float fLength = length();

		x = x / fLength;
		y = y / fLength;

	}

	// Static utility methods...

	static float distance( const Vector2 &v1,  const Vector2 &v2  )
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;


		return (float)sqrt( dx * dx + dy * dy  );
	}

	static float dotProduct( const Vector2 &v1,  const Vector2 &v2 )
	{
		return( v1.x * v2.x + v1.y * v2.y  );
	}


	// Operators...

	Vector2 Vector2::operator + ( const Vector2 &other )
	{
		Vector2 vResult(0.0f, 0.0f);

		vResult.x = x + other.x;
		vResult.y = y + other.y;


		return vResult;
	}

	Vector2 Vector2::operator + ( void ) const
	{
		return *this;
	}

	Vector2 Vector2::operator - ( const Vector2 &other )
	{
		Vector2 vResult(0.0f, 0.0f);

		vResult.x = x - other.x;
		vResult.y = y - other.y;


		return vResult;
	}

	Vector2 Vector2::operator - ( void ) const
	{
		Vector2 vResult(-x, -y);

		return vResult;
	}

	Vector2 Vector2::operator * ( const Vector2 &other )
	{
		Vector2 vResult(0.0f, 0.0f);

		vResult.x = x * other.x;
		vResult.y = y * other.y;


		return vResult;
	}

	Vector2 Vector2::operator * ( const float scalar )
	{
		Vector2 vResult(0.0f, 0.0f);

		vResult.x = x * scalar;
		vResult.y = y * scalar;

		return vResult;
	}

	Vector2 operator * ( const float scalar, const Vector2 &other )
	{
		Vector2 vResult(0.0f, 0.0f );

		vResult.x = other.x * scalar;
		vResult.y = other.y * scalar;

		return vResult;
	}

	Vector2 Vector2::operator / ( const Vector2 &other )
	{
		Vector2 vResult(0.0f, 0.0f );

		vResult.x = x / other.x;
		vResult.y = y / other.y;

		return vResult;
	}

	Vector2& Vector2::operator = ( const Vector2 &other )
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	Vector2& Vector2::operator += ( const Vector2 &other )
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vector2& Vector2::operator -= ( const Vector2 &other )
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}
}

