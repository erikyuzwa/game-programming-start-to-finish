/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: The purpose of this sample is to show how to update an object moving in space
* with the use of Lua. Every frame, the lua script is called which contains some
* incredibly rudimentary movement instructions.
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
	
	lua_State*			lua_vm;
	float				m_fXPosition;

	float  m_fZRotation;
	float  m_fYRotation;


public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	float calculatePosition( lua_State *pLuaState, float arg1, float arg2 );

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicLuaPosition", "data\\system.ini"))
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

	lua_vm = lua_open();

	if(NULL == lua_vm )
	{
		//serious problem, exit program
		return false;
	}

	luaopen_base( lua_vm );
	luaopen_io( lua_vm );
	luaopen_string( lua_vm );
	luaopen_math( lua_vm );



	// Execute the script 
	luaL_loadfile( lua_vm, "data\\calculate_position.lua" );

	m_fZRotation = 0.0f;
	m_fYRotation = 0.0f;
	m_fXPosition = 0.0f;

	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );

	PEON_DELETE( m_pTexture );

	//do our script processing here.
	//now we are finished, close off Lua
	lua_close( lua_vm );


}

void MainApp::onUpdateWorld( float fElapsedTime )
{
	m_fXPosition = calculatePosition( lua_vm, m_fXPosition, fElapsedTime );

	m_fZRotation += 8.0f * fElapsedTime;
	m_fYRotation += 8.0f * fElapsedTime;



}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Basic LUA Position example" );

	glDisable(GL_LIGHTING);

	glBindTexture(GL_TEXTURE_2D, m_pTexture->getTex() );
	
	glPushMatrix();
	glLoadIdentity();
	glTranslatef( m_fXPosition, 0.0f, -10.0f );

	glRotatef(m_fYRotation, 0.0f, 1.0f, 0.0f);
	glRotatef(m_fZRotation, 0.0f, 0.0f, 1.0f );



	glBegin(GL_QUADS);
	// Front Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	// Back Face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	// Top Face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	// Bottom Face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	// Right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
	// Left Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

	glEnd();
	
	glPopMatrix();

}

float MainApp::calculatePosition( lua_State *pLuaState, float arg1, float arg2 ) 
{
	lua_getglobal( pLuaState, "calculatePosition" );

	lua_pushnumber( pLuaState, arg1 );

	lua_pushnumber( pLuaState, arg2 );


	lua_call( pLuaState, 2, 1 );

	float fValue = (float)lua_tonumber( pLuaState, -1 );

	
	lua_pop( pLuaState, 1 );

	return fValue;
}


