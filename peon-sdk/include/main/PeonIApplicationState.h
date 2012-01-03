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
#ifndef __PEONIAPPLICATIONSTATE_H_
#define __PEONIAPPLICATIONSTATE_H_

#include "peonstdafx.h"

namespace peon
{
	/**
	* The game should be broken down into seperate states which then are used to govern the behaviour
	* allowed in each state. This keeps things isolated and as modular as possible
	*/
	class PEONSDK_API IApplicationState
	{
	protected:
		/** primary key */
		int		m_iID;

	public:
		/** constructor */
		IApplicationState(int key = 0);

		/** destructor */
		virtual ~IApplicationState();

		/**
		* return the id of this state
		* \return the state id
		*/
		int getID(){ return m_iID; }

		/**
		* \brief set the ID of this application state
		* \param newID - the new ID of this application state
		*/
		void setID(int newID){ m_iID = newID; }

		/**
		* \brief call this method to load the initial state
		* \return true if things are ok
		*/
		virtual bool loadState(){ return true; };

		/**
		* \brief call this method to unload the state
		*/
		virtual void unloadState(){};

		/**
		* \brief call this method to update the state
		* \param fTimeKey - the time differential between frames
		*/
		virtual void updateState(float fTimeKey){};

		/**
		* \brief this method is used to render this state
		*/
		virtual void drawState(){};

		/**
		* This method provides quick keyboard access to the
		* current state
		* \param pEvent - a generated SDL_KeyboardEvent handle
		*/
		virtual void onKeyEvent( SDL_KeyboardEvent* pEvent ){}

		/**
		* This method provides quick mouse access to the
		* current state
		* \param pEvent - a generated SDL_Event message
		*/
		virtual void onMouseEvent( SDL_Event* pEvent ){}







	};

}

#endif
