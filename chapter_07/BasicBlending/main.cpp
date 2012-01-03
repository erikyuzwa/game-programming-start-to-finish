/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
* 
* @Goal: This sample demonstrates how to use the handy dandy alpha blending capabilites
* of OpenGL. This allows you to create transparency effects within your scene, which
* steps up the "Cool Factor".
*/
#include "PeonMain.h"

class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneTexture* m_pTexture;
	peon::SceneFont*    m_pFont;

	bool				m_bEnableBlend;

	float  m_fZRotation;
	float  m_fYRotation;


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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicBlending", "data\\system.ini"))
	{
		return -1;
	}

	MainApp* pApp = new MainApp();

	peon::EngineCore::getSingleton().setApplication( pApp );

	int ret = peon::EngineCore::getSingleton().runEngine();

	delete peon::EngineCore::getSingletonPtr();


	return( ret );

}

MainApp::MainApp()
{
	m_pFontTexture = NULL; 
	m_pTexture = NULL;
	m_pFont = NULL;

	m_bEnableBlend = false;
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	m_pTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\sdl_logo.bmp");

	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	
	GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[]=	{ 0.0f, 0.0f, 50.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	
	glEnable(GL_LIGHT1);
	
	glEnable(GL_LIGHTING);								// Enable Light One

	m_fZRotation = 0.0f;
	m_fYRotation = 0.0f;

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

	float specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv( GL_FRONT, GL_SPECULAR, specularLight );
	glMateriali( GL_FRONT, GL_SHININESS, 128 );

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
	m_fZRotation += 8.0f * fElapsedTime;
	m_fYRotation += 8.0f * fElapsedTime;

}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Hit 'B' to enable/disable blending" );

	if(m_bEnableBlend)
	{
		m_pFont->renderText( 10.0f, 30.0f, "Blending enabled");
	}else
	{
		m_pFont->renderText( 10.0f, 30.0f, "Blending disabled");
	}

	glBindTexture( GL_TEXTURE_2D, m_pTexture->getTex() );

	if(m_bEnableBlend)
	{
		glEnable(GL_BLEND);
		glDisable( GL_DEPTH_TEST );



	}else
	{
		glDisable( GL_BLEND );
		glEnable( GL_DEPTH_TEST );
	}


	glPushMatrix();
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -10.0f);
	glRotatef(m_fYRotation, 0.0f, 1.0f, 0.0f);
	glRotatef(m_fZRotation, 0.0f, 0.0f, 1.0f );

	

	glBegin(GL_QUADS);

		glColor4f(1.0f, 0.0f, 0.0f, 0.6f);
		
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		
		// Back Face
		glNormal3f( 0.0f, 0.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

		// Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		
		// Bottom Face
		glNormal3f( 0.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		
		// Left Face
		glNormal3f( -1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

    glEnd();

	glPopMatrix();


}

void MainApp::onKeyEvent(SDL_KeyboardEvent* pEvent)
{
	if(pEvent->type == SDL_KEYDOWN )
	{
		if( pEvent->keysym.sym == SDLK_b )
		{
			m_bEnableBlend = !m_bEnableBlend;
			

		}
	}

}