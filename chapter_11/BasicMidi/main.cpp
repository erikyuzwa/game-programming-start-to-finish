
/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: Before the MPEG3 or OGG music formats took off, most music data was rather 
* large and expensive to distribute. As such, one popular music format was the MIDI
* format and wasn't really audio binary data per se, but a sequence of instructions
* for the audio hardware to play/emulate. Massive songs could only take a few k of
* disk space, so this format was/is extremely popular.
*/
#include "PeonMain.h"

class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	bool                m_bEnableMidi;
	Mix_Music*          m_pMidi;


public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	void onKeyEvent(SDL_KeyboardEvent* pEvent );

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicMidi", "data\\system.ini"))
	{
		return -1;
	}

	MainApp* pApp = new MainApp();

	peon::EngineCore::getSingleton().setApplication( pApp );


	return( peon::EngineCore::getSingleton().runEngine() );

}

MainApp::MainApp()
{
	m_bEnableMidi = false;
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	m_pMidi = peon::AudioEngine::getSingleton().loadMIDI( "data\\media\\archive.mid" );
	
	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFontTexture );
	PEON_DELETE( m_pFont );

	Mix_FreeMusic( m_pMidi );


}

void MainApp::onUpdateWorld(float)
{

	

}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Hit 'M' to enable/disable midi" );

}

void MainApp::onKeyEvent(SDL_KeyboardEvent* pEvent)
{

	if(pEvent->type == SDL_KEYDOWN )
	{
	
		if(pEvent->keysym.sym == SDLK_m)
		{
			m_bEnableMidi = !m_bEnableMidi;
			if(m_bEnableMidi)
			{
				//play the midi file
				Mix_PlayMusic(m_pMidi, 0);


			}else
			{
				//stop the midi file
				Mix_HaltMusic();


			}

		}

	}
}