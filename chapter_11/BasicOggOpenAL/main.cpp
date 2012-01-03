/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
* 
* @Goal: Nearly every game or entertainment product must play music at SOME point. There's
* been several battles over the years, but the two main formats that have risen to the
* top are MPEG-3 and OGG. 
* MPEG-3 is popular, but the drawback for most developers is that it's ludicrously 
* expensive to license. 
* The OGG format, however, is completely free to use and provides just as much
* audio compression and clarity. Enjoy!
*/
#include "PeonMain.h"

class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	bool                m_bEnableBGM;

	peon::AudioNode     m_oBGMusicNode;
	peon::AudioNode     m_oLaserNode;


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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicOggOpenAL", "data\\system.ini"))
	{
		return -1;
	}

	MainApp* pApp = new MainApp();

	peon::EngineCore::getSingleton().setApplication( pApp );


	return( peon::EngineCore::getSingleton().runEngine() );

}

MainApp::MainApp()
{
	m_bEnableBGM = false;
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	peon::AudioEngine::getSingleton().loadAudioNode( "data\\media\\laser.wav", &m_oLaserNode );
	peon::AudioEngine::getSingleton().loadAudioNode( "data\\media\\Asteroid_Ambient.ogg", &m_oBGMusicNode );



	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFontTexture );
	PEON_DELETE( m_pFont );




}

void MainApp::onUpdateWorld(float)
{

	

}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Hit 'M' to start/stop background music" );
	m_pFont->renderText( 10.0f, 30.0f, "Hit 'Left Control' to play laser sound effect" );

}

void MainApp::onKeyEvent(SDL_KeyboardEvent* pEvent)
{

	if(pEvent->type == SDL_KEYDOWN )
	{
		if(pEvent->keysym.sym == SDLK_m )
		{
			peon::AudioEngine::getSingleton().setAudioNode( &m_oBGMusicNode );
			peon::AudioEngine::getSingleton().playAudioNode( &m_oBGMusicNode );
		}
	
		if(pEvent->keysym.sym == SDLK_LCTRL )
		{
			peon::AudioEngine::getSingleton().setAudioNode( &m_oLaserNode );
			peon::AudioEngine::getSingleton().playAudioNode( &m_oLaserNode );
		}
	}
}