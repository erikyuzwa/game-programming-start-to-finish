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
#include "PeonVector4.h"

namespace peon
{

	Vector4::Vector4( float x_, float y_, float z_, float w_ )
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}

	Vector4::~Vector4()
	{

	}

	void Vector4::set( float x_, float y_, float z_, float w_ )
	{
		x = x_;
		y = y_;
		z = z_;
		w = w_;
	}

	float Vector4::length( void )
	{
		return( (float)sqrt( x * x + y * y + z * z + w * w ) );
	}

	void Vector4::normalize( void )
	{
		float fLength = length();

		x = x / fLength;
		y = y / fLength;
		z = z / fLength;
		w = w / fLength;
	}

	// Static utility methods...

	static float distance( const Vector4 &v1,  const Vector4 &v2  )
	{
		float dx = v1.x - v2.x;
		float dy = v1.y - v2.y;
		float dz = v1.z - v2.z;

		return (float)sqrt( dx * dx + dy * dy + dz * dz );
	}

	static float dotProduct( const Vector4 &v1,  const Vector4 &v2 )
	{
		return( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z  );
	}

	static Vector4 crossProduct( const Vector4 &v1,  const Vector4 &v2 )
	{
		Vector4 vCrossProduct;

		vCrossProduct.x = v1.y * v2.z - v1.z * v2.y;
		vCrossProduct.y = v1.z * v2.x - v1.x * v2.z;
		vCrossProduct.z = v1.x * v2.y - v1.y * v2.x;

		return vCrossProduct;
	}

	// Operators...

	Vector4 Vector4::operator + ( const Vector4 &other )
	{
		Vector4 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = x + other.x;
		vResult.y = y + other.y;
		vResult.z = z + other.z;

		return vResult;
	}

	Vector4 Vector4::operator + ( void ) const
	{
		return *this;
	}

	Vector4 Vector4::operator - ( const Vector4 &other )
	{
		Vector4 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = x - other.x;
		vResult.y = y - other.y;
		vResult.z = z - other.z;

		return vResult;
	}

	Vector4 Vector4::operator - ( void ) const
	{
		Vector4 vResult(-x, -y, -z);

		return vResult;
	}

	Vector4 Vector4::operator * ( const Vector4 &other )
	{
		Vector4 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = x * other.x;
		vResult.y = y * other.y;
		vResult.z = z * other.z;

		return vResult;
	}

	Vector4 Vector4::operator * ( const float scalar )
	{
		Vector4 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = x * scalar;
		vResult.y = y * scalar;
		vResult.z = z * scalar;

		return vResult;
	}

	Vector4 operator * ( const float scalar, const Vector4 &other )
	{
		Vector4 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = other.x * scalar;
		vResult.y = other.y * scalar;
		vResult.z = other.z * scalar;

		return vResult;
	}

	Vector4 Vector4::operator / ( const Vector4 &other )
	{
		Vector4 vResult(0.0f, 0.0f, 0.0f);

		vResult.x = x / other.x;
		vResult.y = y / other.y;
		vResult.z = z / other.z;

		return vResult;
	}

	Vector4& Vector4::operator = ( const Vector4 &other )
	{
		x = other.x;
		y = other.y;
		z = other.z;

		return *this;
	}

	Vector4& Vector4::operator += ( const Vector4 &other )
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	Vector4& Vector4::operator -= ( const Vector4 &other )
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

}

