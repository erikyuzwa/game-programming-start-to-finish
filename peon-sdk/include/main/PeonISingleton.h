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
#ifndef __PEONISINGLETON_H__
#define __PEONISINGLETON_H__

/* Original version Copyright (C) Scott Bilas, 2000.
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Scott Bilas, 2000"
 */

#include "Peonstdafx.h"

namespace peon
{

    /**
	* Template class for creating single-instance global classes.
	* The code in this file is taken from article 1.3 in the the book:
	* Game Programming Gems from Charles River Media with the
	* copyright notice going to Scott Bilas.
    */
    template <typename T> class PEONSDK_API ISingleton
    {
    protected:

		/** The static member object */
        static T* ms_Singleton;

    public:
		/**
		* Constructor
		*/
        ISingleton( void )
        {
            assert( !ms_Singleton );
			ms_Singleton = static_cast< T* >( this );
        }

		/**
		* Destructor
		*/
        ~ISingleton( void )
            {  assert( ms_Singleton );  ms_Singleton = 0;  }

		/**
		* This method just returns the internal member by
		* reference
		* \return T& - reference to internal abstract Type
		*/
		static T& getSingleton( void )
		{	assert( ms_Singleton );  return ( *ms_Singleton ); }

		/**
		* This method just returns the internal member by
		* a pointer
		* \return T* - pointer to the internal abstract Type
		*/
		static T* getSingletonPtr( void )
		{ return ms_Singleton; }
    };
}

#endif
