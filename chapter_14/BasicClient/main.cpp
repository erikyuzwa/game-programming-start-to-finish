/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: The server is the boss of the game world, and each client connects to it in order
* to interact in the game world. This sample demonstrates how to startup a client and
* connect to a server.
*/
#include "PeonMain.h"
#include <SDL_net.h>


class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	Uint16    m_hostPort;
	IPaddress m_hostAddress;
	TCPsocket m_hostSocket;
	

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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicClient", "data\\system.ini"))
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
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	SDLNet_Init();

	// get the port from the commandline
	m_hostPort = (Uint16)strtol("9090",NULL,0);
	
	// Resolve the argument into an IPaddress type
	if(SDLNet_ResolveHost(&m_hostAddress,"127.0.0.1",m_hostPort) == -1)
	{
		//printf("SDLNet_ResolveHost: %s\n",SDLNet_GetError());
		//exit(3);
		return false;
	}

	// open the server socket
	m_hostSocket = SDLNet_TCP_Open(&m_hostAddress);
	if(!m_hostSocket)
	{
		//printf("SDLNet_TCP_Open: %s\n",SDLNet_GetError());
		//exit(4);
		return false;
	}

	

	
	return true;
}

void MainApp::onUnloadWorld()
{

	SDLNet_TCP_Close(m_hostSocket);

	SDLNet_Quit();

	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );

}

void MainApp::onUpdateWorld(float fElapsedTime )
{

}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Connecting to server 127.0.0.1:9090" );



}

