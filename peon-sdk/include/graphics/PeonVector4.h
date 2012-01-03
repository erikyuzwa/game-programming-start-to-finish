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
#ifndef __PEONVECTOR4_H_
#define __PEONVECTOR4_H_

#include "Peonstdafx.h"

namespace peon
{
	/**
	* This object is used as a 4-tuple. So far it's mainly just used for
	* lighting purposes in the @see SceneLight object, but it could also perhaps
	* be used to store RGBA color information...
	*
	*/
	class PEONSDK_API Vector4
	{
	public:
		/** x component */
		float x;

		/** y component */
		float y;

		/** z component */
		float z;

		/** w component */
		float w;

		/**
		* Constructor
		*/
		Vector4(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, float w_ = 0.0f);

		/**
		* Destructor
		*/
		~Vector4();

		void set(float x_, float y_, float z_, float w_);
		float length(void);
		void normalize(void);

		// Static utility methods
		static float distance(const Vector4 &v1, const Vector4 &v2);
		static float dotProduct(const Vector4 &v1,  const Vector4 &v2 );
		static Vector4 crossProduct(const Vector4 &v1, const Vector4 &v2);

		// Operators...
		Vector4 operator + (const Vector4 &other);
		Vector4 operator - (const Vector4 &other);
		Vector4 operator * (const Vector4 &other);
		Vector4 operator / (const Vector4 &other);

		Vector4 operator * (const float scalar);
		friend Vector4 operator * (const float scalar, const Vector4 &other);

		Vector4& operator = (const Vector4 &other);
		Vector4& operator += (const Vector4 &other);
		Vector4& operator -= (const Vector4 &other);

		Vector4 operator + (void) const;
		Vector4 operator - (void) const;
	};

}

#endif

