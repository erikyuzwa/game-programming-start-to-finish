/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: This sample demonstrates how to use OpenGL's basic lighting in your scene.
* It's all done by the hardware, you just need to position the light along with 
* defining how that light source interacts with any object in your game world. OpenGL
* will do the rest.
*/
#include "PeonMain.h"

class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneTexture* m_pTexture;
	peon::SceneFont*    m_pFont;

	bool				m_bEnableLight;

	float  m_fZRotation;
	float  m_fYRotation;


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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicLight", "data\\system.ini"))
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
	m_pTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\sdl_logo.bmp");

	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	
	GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[]=	{ 0.0f, 0.0f, 50.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);

	m_fZRotation = 0.0f;
	m_fYRotation = 0.0f;

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glEnable( GL_COLOR_MATERIAL );
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

	float specularLight[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glMaterialfv( GL_FRONT, GL_SPECULAR, specularLight );
	glMateriali( GL_FRONT, GL_SHININESS, 128 );

	m_bEnableLight = false;

	
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
	m_pFont->renderText( 10.0f, 10.0f, "Hit 'L' to enable/disable lighting" );

	if(m_bEnableLight)
	{
		m_pFont->renderText( 10.0f, 30.0f, "Lighting enabled" );

		glEnable( GL_LIGHTING );
		

	}else
	{
		m_pFont->renderText( 10.0f, 30.0f, "Lighting disabled" );
		glDisable( GL_LIGHTING );
	}


	glBindTexture( GL_TEXTURE_2D, m_pTexture->getTex() );

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
		if( pEvent->keysym.sym == SDLK_l )
		{
			m_bEnableLight = !m_bEnableLight;
		}
	}

}