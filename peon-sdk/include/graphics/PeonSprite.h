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
#ifndef __PEONSPRITE_H_
#define __PEONSPRITE_H_

#include "peonstdafx.h"
#include "peonvector2.h"
#include "peonvector3.h"

namespace peon
{
	/***
	* This is our base sprite object
	*/
	class PEONSDK_API Sprite
	{
	public:
		/** @see Vector3 position */
		Vector3 m_vecPos;

		/** @see Vector2 bounding box for collisions */
		Vector2 m_vecBBox;

		/** is our sprite alive or dead? */
		bool	m_bIsAlive;

	public:
		/**
		* Constructor
		*/
		Sprite();

		/**
		* Destructor
		*/
		virtual ~Sprite();

		/**
		* This method sets the position of the sprite
		* \param x - float pos
		* \param y - float pos
		* \param z - float pos
		*/
		void setPosition( float x, float y, float z);

		/**
		* Check if the sprite is alive / dead
		* \return alive or dead
		*/
		bool isAlive(){ return m_bIsAlive; }

	};

}

#endif
