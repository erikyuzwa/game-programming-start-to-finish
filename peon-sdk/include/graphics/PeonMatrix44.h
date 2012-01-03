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
#ifndef __PEONMATRIX44_H_
#define __PEONMATRIX44_H_

#include "Peonstdafx.h"
#include "PeonVector3.h"

namespace peon
{

	/**
	* This is a bare-bones Matrix object which is of size 4x4.
	* It's not optimized, so use at your own risk but it demonstrates
	* how to use some basic matrix operations.
	*
	* The matrix is column-major which is exactly how the matrices are
	* represented in OpenGL. If you're porting this library over to
	* Direct3D, just remember that Direct3D's format is row-major.
	*
	* For other math libraries, check out the Ogre project, the
	* Irrlicht engine or even the basic objects found at Wild Magic Software.
	*/
	class PEONSDK_API Matrix44
	{
	public:

		/** our matrix data. It's in one contiguous element block. */
		float m[16];

		/**
		* Constructor
		*/
		Matrix44(){ }

		/**
		* Constructor containing given values
		*/
		Matrix44( float m0, float m4, float  m8, float m12,
				   float m1, float m5, float  m9, float m13,
				   float m2, float m6, float m10, float m14,
				   float m3, float m7, float m11, float m15 );

		void identity(void);
		void translate(const Vector3 &trans);
		void translate_x(const float &dist);
		void translate_y(const float &dist);
		void translate_z(const float &dist);
		void rotate(const float &angle, Vector3 &axis);
		void rotate_x(const float &angle);
		void rotate_y(const float &angle);
		void rotate_z(const float &angle);
		void scale(const Vector3 &scale);
		void transformPoint( Vector3 *vec );
		void transformVector( Vector3 *vec );

		// Operators...
		Matrix44 operator + (const Matrix44 &other);
		Matrix44 operator - (const Matrix44 &other);
		Matrix44 operator * (const Matrix44 &other);

		Matrix44 operator * (const float scalar);
	};



}

#endif

