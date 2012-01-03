
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
#ifndef __PEONVECTOR2_H_
#define __PEONVECTOR2_H_

#include "Peonstdafx.h"

namespace peon
{

	/**
	* This object is used to represent a 3-tuple entity for use mostly during
	* object positioning in a 3D gameworld. It's perfectly acceptable to keep
	* the z-plane variable to 1.0f, if you're only interested in using 2D
	* graphics.
	*/
	class PEONSDK_API Vector2
	{
	public:

		/** x component */
		float x;

		/** y component */
		float y;

		/**
		* Constructor
		*
		*/
		Vector2(float x_ = 0.0f, float y_ = 0.0f);

		/**
		* Destructor
		*/
		~Vector2();

		/**
		* setter method
		* \param x_ : x component
		* \param y_ : y component
		*/
		void set(float x_, float y_);

		/**
		* calculate the length of this vector
		* \return float
		*/
		float length(void);

		/**
		* Normalize the vector. Divide each component by 1.0f
		*/
		void normalize(void);

		// Static utility methods
		static float distance(const Vector2 &v1, const Vector2 &v2);
		static float dotProduct(const Vector2 &v1,  const Vector2 &v2 );

		// Operators...
		Vector2 operator + (const Vector2 &other);
		Vector2 operator - (const Vector2 &other);
		Vector2 operator * (const Vector2 &other);
		Vector2 operator / (const Vector2 &other);

		Vector2 operator * (const float scalar);
		friend Vector2 operator * (const float scalar, const Vector2 &other);

		Vector2& operator = (const Vector2 &other);
		Vector2& operator += (const Vector2 &other);
		Vector2& operator -= (const Vector2 &other);

		Vector2 operator + (void) const;
		Vector2 operator - (void) const;
	};



}

#endif

