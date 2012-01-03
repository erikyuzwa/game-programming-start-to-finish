/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: This sample is all about how to read the position and button information from
* your mouse.
*/
#include "PeonMain.h"

class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	char                m_strMouseInfo[MAX_PATH];

public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	void onMouseEvent( SDL_Event* pEvent );

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicMouse", "data\\system.ini"))
	{
		return -1;
	}

	MainApp* pApp = new MainApp();

	peon::EngineCore::getSingleton().setApplication( pApp );


	return( peon::EngineCore::getSingleton().runEngine() );

}

MainApp::MainApp()
{
	m_pFontTexture = NULL; 
	m_pFont = NULL;
	m_strMouseInfo[0] = '\0';
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	
	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );

}

void MainApp::onUpdateWorld(float)
{
}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, m_strMouseInfo );

}

void MainApp::onMouseEvent(SDL_Event* pEvent)
{
	int x, y;
	Uint8 button;

	// Get the mouse's current X,Y position
    button = SDL_GetMouseState(&x, &y);

	sprintf(m_strMouseInfo, "Mouse Position (x,y): ( %d, %d )", x, y);

	if(button & SDL_BUTTON(1))
	{
		strcat( m_strMouseInfo, "  LMB");

	}else if(button & SDL_BUTTON(3))
	{
		strcat( m_strMouseInfo, "  RMB");
	}



}


