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
#ifndef __PEONVECTOR3_H_
#define __PEONVECTOR3_H_

#include "Peonstdafx.h"

namespace peon
{

	/**
	* This object is used to represent a 3-tuple entity for use mostly during
	* object positioning in a 3D gameworld. It's perfectly acceptable to keep
	* the z-plane variable to 1.0f, if you're only interested in using 2D
	* graphics.
	*/
	class PEONSDK_API Vector3
	{
	public:

		/** x component */
		float x;

		/** y component */
		float y;

		/** z component */
		float z;

		/**
		* Constructor
		*
		*/
		Vector3(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f);

		/**
		* Destructor
		*/
		~Vector3();

		/**
		* setter method
		* \param x_ : x component
		* \param y_ : y component
		* \param z_ : z component
		*/
		void set(float x_, float y_, float z_);

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
		static float distance(const Vector3 &v1, const Vector3 &v2);

		inline float dotProduct(const Vector3& vec) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

		inline Vector3 crossProduct( const Vector3& vecInput ) const
        {
            Vector3 vecCross;

            vecCross.x = y * vecInput.z - z * vecInput.y;
            vecCross.y = z * vecInput.x - x * vecInput.z;
            vecCross.z = x * vecInput.y - y * vecInput.x;

            return vecCross;
        }

		// Operators...
		Vector3 operator + (const Vector3 &other);
		Vector3 operator - (const Vector3 &other);
		Vector3 operator * (const Vector3 &other);
		Vector3 operator / (const Vector3 &other);

		Vector3 operator / (const float scalar);
		Vector3 operator * (const float scalar);
		friend Vector3 operator * (const float scalar, const Vector3 &other);

		Vector3& operator = (const Vector3 &other);
		Vector3& operator += (const Vector3 &other);
		Vector3& operator -= (const Vector3 &other);

		Vector3 operator + (void) const;
		Vector3 operator - (void) const;
	};



}

#endif

