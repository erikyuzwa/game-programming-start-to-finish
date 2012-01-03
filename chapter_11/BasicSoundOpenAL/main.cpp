/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
* 
* @Goal: Another popular audio library besides SDL_Mixer is the popular OpenAL library.
* It's another cross-platform audio solution used in many games and titles. This sample
* just demonstrates how to play WAVE files using OpenAL.
*/
#include "PeonMain.h"


class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	peon::AudioNode     m_oAudioNode;
	bool				m_bEnableSound;

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
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	peon::AudioEngine::getSingleton().loadAudioNode( "data\\media\\laser.wav", &m_oAudioNode );

	m_bEnableSound = false;

	m_oAudioNode.sound_position[0] = 0.1f;
	m_oAudioNode.sound_position[1] = 0.0f;
	m_oAudioNode.sound_position[2] = 0.0f;

	m_oAudioNode.sound_loop = true;
		
	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );



}

void MainApp::onUpdateWorld( float fElapsedTime )
{

}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Hit 'S' to play sound effect using OpenAL" );


}

void MainApp::onKeyEvent(SDL_KeyboardEvent* pEvent)
{

	if(pEvent->type == SDL_KEYDOWN )
	{
	
		if(pEvent->keysym.sym == SDLK_s)
		{
			m_bEnableSound = !m_bEnableSound;
			if(m_bEnableSound)
			{
				peon::AudioEngine::getSingleton().setAudioNode( &m_oAudioNode );
				peon::AudioEngine::getSingleton().playAudioNode( &m_oAudioNode );
			}else
			{
				peon::AudioEngine::getSingleton().stopAudioNode( &m_oAudioNode );
			}
		}

	}
}