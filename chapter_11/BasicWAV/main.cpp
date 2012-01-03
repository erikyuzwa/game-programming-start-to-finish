/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
* 
* @Goal: No matter what the game you're making, the use of sound is essential. A popular
* sound library for the SDL family is SDL_Mixer. It's an easy-to-use sound package which
* can handle many audio file formats. A popular one is the "WAVE" format introduced
* by Windows. Here's how to play them with SDL_Mixer.
*/
#include "PeonMain.h"


class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	Mix_Chunk*  m_pSound;
	int			m_iSound;

public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	void onKeyEvent( SDL_KeyboardEvent* pEvent );

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicWav", "data\\system.ini"))
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
	m_pSound = NULL;
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	m_pSound = peon::AudioEngine::getSingleton().loadWAVChunk( "data\\media\\laser.wav" );
	
	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );

	Mix_HaltChannel( m_iSound );
	Mix_FreeChunk( m_pSound );


}

void MainApp::onUpdateWorld(float)
{
}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Hit 'S' to play sound effect" );

}

void MainApp::onKeyEvent(SDL_KeyboardEvent* pEvent)
{

	if(pEvent->type == SDL_KEYDOWN )
	{
	
		if(pEvent->keysym.sym == SDLK_s)
		{
			//play the midi file
			m_iSound = Mix_PlayChannel(-1, m_pSound, 0);

		}

	}
}