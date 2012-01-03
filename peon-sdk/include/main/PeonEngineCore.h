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

#ifndef __PEONENGINECORE_H_
#define __PEONENGINECORE_H_

#include "peonstdafx.h"
#include "PeonIniConfig.h"
#include "PeonISingleton.h"
#include "PeonIApplicationState.h"

namespace peon
{
	/**
	* This is the main object in the Peon SDK that you need to use for proper
	* use of the library and all that it brings you. For base functionality, you
	* only need to provide a body declaration for the core functions:
	* <pre>
	* class MainApplicationState : public IApplicationState
	* {
	*   public:
	*	    MainApplicationState();
	*       ~MainApplicationState();
	*
	*   bool loadState();
	*   void unloadState();
	*   void drawState();
	*   void updateState();
	* };
	* </pre>
	*
	* Then just stick it in a main file in something like...
	*
	* <pre>
	* ##include "MainApplicationState.h"
	*
	* using namespace peon;
	*
	* int main(int argc, char* argv[])
	* {
	*   int ec = 0;
	*	new EngineCore();
	*
	*   if(!EngineCore::getSingleton().loadEngine(APP_NAME, INI_FILE))
	*   {
	*     ec = -1; goto cleanup;
	*   }
    *
	*   MainApplicationState* pMainAppState = new MainApplicationState();
	*
	*   EngineCore::getSingletonPtr()->addAppState( pMainAppState );
	*
	*   ec = EngineCore::getSingleton().runEngine();
	*
	*   cleanup:
	*     delete EngineCore::getSingletonPtr();
	*
	*   return ec;
	* }
	* </pre>
	*/
	class PEONSDK_API EngineCore : ISingleton<EngineCore>
	{

	protected:
		/** @see IniConfig */
		IniConfig*		m_pConfig;

		/** @see SceneRenderer */
		SceneRenderer*		m_pCanvas;

		/** @see OGLSprite */
		boost::ptr_list<Sprite> m_oDisplayList;

		/** @see IApplicationState */
		boost::ptr_list<IApplicationState> m_oAppStateList;

		/** @see IApplicationState */
		IApplicationState* m_pCurrentState;


	public:
		/**
		* Constructor
		*/
		EngineCore();

		/**
		* Destructor
		*/
		~EngineCore();

		/** Override standard Singleton retrieval.
		*/
		static EngineCore& getSingleton(void);

		/** Override standard Singleton retrieval.
		*/
		static EngineCore* getSingletonPtr(void);



		/**
		* This method is used to start things up for your application.
		* \param strWindowTitle - text to display on window title bar
		* \param strIniConfig - location of .INI game configuration data
		* \return true or false
		*/
		bool loadEngine(const char* strWindowTitle, char* strIniConfig = NULL );

		/**
		* This method is an internal cleanup process
		*/
		void unloadEngine();

		/**
		* This method will be your game's "main" processing loop
		*/
		int runEngine();

		/**
		* This method adds an IApplicationState container to our
		* runtime list
		* \param pNewState - an IApplicationState
		*/
		bool addAppState(IApplicationState* pNewState);

		/**
		* This method switches the @IApplicationState list to the
		* proper "runtime" one
		* \param id - the int of the state to switch to
		*/
		void setAppState( int id );

		/**
		* You need to provide a body for this function, used to create any
		* specific objects for your game.
		*/
		virtual bool createWorld() = 0;


		/**
		* You need to provide a body for this function, used to update any
		* specific objects for your game.
		*/
		virtual void updateWorld() = 0;


		/**
		* You need to provide a body for this function, used to draw / render any
		* specific objects for your game.
		*/
		virtual void drawWorld() = 0;


		/**
		* You need to provide a body for this function, used to destroy / cleanup any
		* specific objects for your game.
		*/
		virtual void destroyWorld() = 0;
	};

}

#endif
