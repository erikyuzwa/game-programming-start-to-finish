/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: This small sample just demonstrates how to initialize the Lua subsystem for
* use in your applications. A scripting language such as Lua is great for manipulating
* things in your game world without the necessity to recompile the entire binary.
*/
#include "PeonMain.h"

extern "C" 
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}


class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	peon::SceneTexture* m_pTexture;
	


public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicLua", "data\\system.ini"))
	{
		return -1;
	}

	MainApp* pApp = new MainApp();

	peon::EngineCore::getSingleton().setApplication( pApp );


	return( peon::EngineCore::getSingleton().runEngine() );

}

MainApp::MainApp()
{
	
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);


	m_pTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\sdl_logo.bmp");

	lua_State* lua_vm = lua_open();

	if(NULL == lua_vm)
	{
		//serious problem, exit program
		return false;
	}

	luaopen_base( lua_vm );
	luaopen_io( lua_vm );
	luaopen_string( lua_vm );
	luaopen_math( lua_vm );

	//do our script processing here.
	//now we are finished, close off Lua
	lua_close( lua_vm );

	

	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );

	PEON_DELETE( m_pTexture );

}

void MainApp::onUpdateWorld( float fElapsedTime )
{


}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Basic LUA example" );

	glDisable(GL_LIGHTING);
	

}




