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
#include "PeonSceneRenderer.h"
#include "PeonSprite.h"
#include "PeonEngineCore.h"
#include "PeonFileLogger.h"

namespace peon
{
	//-----------------------------------------------------------------------
	template<> EngineCore* ISingleton<EngineCore>::ms_Singleton = 0;

	EngineCore* EngineCore::getSingletonPtr(void)
	{
		return ms_Singleton;
	}

	EngineCore& EngineCore::getSingleton(void)
	{

		assert( ms_Singleton );

		return ( *ms_Singleton );
	}

	EngineCore::EngineCore()
	{
		m_pCanvas = NULL;

		m_pCurrentState = NULL;

	}

	EngineCore::~EngineCore()
	{

	}

	bool EngineCore::loadEngine( const char* strWindowTitle, char* strIniConfig )
	{

		//Need to test in other compilers, but for now let's only enable the
		//CrtSetDbgFlag stuff if we are using a Visual Studio product.
		#if defined( _MSC_VER )

			#if defined(DEBUG) | defined(_DEBUG)
				_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
			#endif

		#endif

		new FileLogger();
		if(!FileLogger::getSingleton().openLogStream("PeonSDK.log"))
		{
			unloadEngine();
			return false;
		}


		//start up SDL
		if(SDL_Init( SDL_INIT_EVERYTHING ) < 0 || !SDL_GetVideoInfo() )
		{
			FileLogger::getSingleton().logFatal("EngineCore", "Failed to load SDL. Exiting");
			unloadEngine();
			return false;
		}
		FileLogger::getSingleton().logInfo("EngineCore", "SDL created");

		assert( strIniConfig != NULL );

		m_pConfig = new IniConfig( strIniConfig );
		if(NULL == m_pConfig)
		{
			FileLogger::getSingleton().logFatal("EngineCore", "Failed to load INI file. Exiting");
			unloadEngine();
			return false;
		}
		FileLogger::getSingleton().logInfo("EngineCore", "IniConfigReader created");


		//create an initial window
		SDL_SetVideoMode( 800, 600, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_RESIZABLE );


		SDL_WM_SetCaption(strWindowTitle, strWindowTitle );


		m_pCanvas = new SceneRenderer();
		if(!m_pCanvas->createCanvas( m_pConfig ))
		{
			unloadEngine();
			return false;
		}

		if(!createWorld())
		{
			unloadEngine();
			return false;
		}


		return true;
	}

	void EngineCore::unloadEngine()
	{
		destroyWorld();

		PEON_DELETE( m_pCanvas );

		PEON_DELETE( m_pConfig );

		m_oAppStateList.clear(); //all the IApplicationState pointers are cleared

		m_oDisplayList.clear(); // all pointers in our sprite display list are deleted

		SDL_Quit();

		delete FileLogger::getSingletonPtr();
	}

	int EngineCore::runEngine()
	{

		bool done = false;
		SDL_Event event;

		while(! done)
		{
			while( SDL_PollEvent(&event) )
			{
				switch ( event.type )
				{
/*
					case SDL_KEYDOWN:
					case SDL_KEYUP:

						if(m_pApplication)
							m_pApplication->onKeyEvent(&event.key);
                    break;

					case SDL_MOUSEMOTION:
					case SDL_MOUSEBUTTONDOWN:
						if(m_pApplication)
							m_pApplication->onMouseEvent( &event );
					break;

 */

				case SDL_KEYDOWN:
				case SDL_KEYUP:

					if(m_pCurrentState)
						m_pCurrentState->onKeyEvent(&event.key);
                break;

				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONDOWN:
					if(m_pCurrentState)
						m_pCurrentState->onMouseEvent( &event );
				break;

				case SDL_QUIT :
					done = true;
					break;

				default:
					break;
				}
			}

		    if( NULL != m_pCurrentState )
			{
				//update the current state
				m_pCurrentState->updateState( 1.0f );
			}


			if( m_pCanvas->clearCanvas())
			{
				if( NULL != m_pCurrentState )
				{
					//draw the current state
					m_pCurrentState->drawState();
				}

				m_pCanvas->flipCanvas();
			}

			//float elapsed_time = m_oTimer.getElapsedTime();

			//if(m_pApplication)
			//{
			//    m_pApplication->onUpdateWorld( elapsed_time );

			//	if(m_pVideoDevice->clearDevice())
			//	{

					//m_pApplication->onRenderWorld();

					//m_pVideoDevice->flipDevice();

					//updateFPS();

			//	}

			//}

		}

		//m_oTimer.stop();

		unloadEngine();


		return 0;
	}

	bool EngineCore::addAppState(IApplicationState* pNewState)
	{


		//load this puppy into our managed texture list
		m_oAppStateList.push_back( pNewState );


		return true;
	}

	void EngineCore::setAppState( int key )
	{
		int found = -1;
		for( boost::ptr_list<IApplicationState>::iterator i = m_oAppStateList.begin();
			i != m_oAppStateList.end(); ++i)
		{

			if( i->getID() == key )
			{
				found = 1;
				//m_pCurrentState = *i;
                break;
			}

		}

		if( found < 0 )
		{
			TCHAR strOutput[512];
			sprintf(strOutput, "Couldn't find the state: %d", key);
			FileLogger::getSingleton().logError("EngineCore", strOutput);

		}



	}

}

