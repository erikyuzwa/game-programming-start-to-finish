
/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: A popular "feature" for any game is the use of multiplayer code. Essential to
* a client/server architecture is the "server". It's usually the boss of the game. This
* sample demonstrates how to start up a simple server using the SDL networking
* library, SDL_net. Once it starts up, it simply listens for client connections until
* shutdown.
*/
#include "PeonMain.h"
#include <sdl_net.h>


class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;


	Uint16    m_port;
	TCPsocket m_hostSocket;
	IPaddress m_hostAddress;

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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicServer", "data\\system.ini"))
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

	m_port = 9090;

	// Resolve the argument into an IPaddress type
	if(SDLNet_ResolveHost(&m_hostAddress,NULL,m_port)==-1)
	{
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

	TCPsocket clientSocket;
	IPaddress *clientAddress;


	// try to accept a connection
	clientSocket = SDLNet_TCP_Accept(m_hostSocket);
	if(!clientSocket)
	{ // no connection accepted
			//printf("SDLNet_TCP_Accept: %s\n",SDLNet_GetError());
		SDL_Delay(100); //sleep 1/10th of a second
		return;
	}
		
	// get the clients IP and port number
	clientAddress=SDLNet_TCP_GetPeerAddress(clientSocket);
	if(!clientAddress)
	{
		//printf("SDLNet_TCP_GetPeerAddress: %s\n",SDLNet_GetError());
		return;	
	}


	// print out the clients IP and port number
	Uint32 ipaddr;
	ipaddr = SDL_SwapBE32(clientAddress->host);
	char strOutput[256];

	sprintf(strOutput, "Accepted a connection from %d.%d.%d.%d port %hu",
		ipaddr>>24,
		(ipaddr>>16)&0xff,
		(ipaddr>>8)&0xff,
		ipaddr&0xff,
		clientAddress->port);

	peon::FileLogger::getSingleton().logInfo("BasicServer", strOutput );


	int result;
	char strMessage[128];

	strcpy( strMessage, "Hello");
	int len = strlen(strMessage);
		
	// print out the message
	sprintf(strOutput, "Sending to client: 'Hello' message");
	peon::FileLogger::getSingleton().logInfo("BasicServer", strOutput );

	
	result = SDLNet_TCP_Send(clientSocket,strMessage,len); // add 1 for the NULL
	/*
	if(result < len)
	{
		printf("SDLNet_TCP_Send: %s\n",SDLNet_GetError());
	}*/





	SDLNet_TCP_Close(clientSocket);



}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Server listening on 127.0.0.1:9090" );

	m_pFont->renderText( 10.0f, 30.0f, "Client activity is logged in the Peon.log file");


}
