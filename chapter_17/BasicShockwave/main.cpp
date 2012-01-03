/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: The purpose of this small sample is to demonstrate how to create a small nifty
* shockwave that you see in some games. Once the wave is started, we simply expand it
* outwards at a set rate and adjust the alpha values of our texture to make it appear
* like it's slowly fading away...
*/
#include "PeonMain.h"




class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	peon::SceneTexture* m_pTexture;

	bool                m_bToggle;
	peon::Shockwave*	m_pShockwave;


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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicShockwave", "data\\system.ini"))
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

	m_pTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\firetile.jpg");

	m_pShockwave = new peon::Shockwave();
	m_pShockwave->load( 1.5f, 0.5f, 32, 2.0f, 0.90f );
	m_pShockwave->m_vecPos = peon::Vector3( 0.0f, 0.0f, -20.0f );

	m_bToggle = false;

	GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[]=	{ 0.0f, 0.0f, 2.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	
	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );
	PEON_DELETE( m_pTexture );
	PEON_DELETE( m_pShockwave );

}

void MainApp::onUpdateWorld( float fElapsedTime )
{
	m_pShockwave->update( fElapsedTime );
	

}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	glDisable(GL_LIGHTING);
	m_pFont->renderText( 10.0f, 10.0f, "Hit 'space' to start/stop shockwave" );
	glEnable(GL_LIGHTING);
	
	if(m_pShockwave->m_bIsRunning)
	{
		glBindTexture(GL_TEXTURE_2D, m_pTexture->getTex() );


		m_pShockwave->render();
		
	}
}

void MainApp::onKeyEvent(SDL_KeyboardEvent* pEvent )
{
	if(pEvent->type == SDL_KEYDOWN)
	{
		if(pEvent->keysym.sym == SDLK_SPACE )
		{
			m_bToggle = !m_bToggle;
			if(m_bToggle)
				m_pShockwave->start();
			else
				m_pShockwave->stop();
		}
	}
}