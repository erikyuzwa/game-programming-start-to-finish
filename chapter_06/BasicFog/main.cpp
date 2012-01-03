/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* This code sample demonstrates how to use some basic fog properties for using
* it in a scene. If you use the up and down arrows, it will move the box towards
* and away from the camera which demonstrates how it comes out of the fog
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: A common cheap effect (which can be amazing) is the use of fog. The hardware
* can do the underlying calculations in order to determine which vertices are 
* obscured by fog or are partially blended, etc.
*/
#include "PeonMain.h"

class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneTexture* m_pTexture;
	peon::SceneFont*    m_pFont;

	bool				m_bFog;

	GLuint m_uTriDisplayList;
	float  m_fZRotation;
	float  m_fYRotation;
	float  m_fZPos;


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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicFog", "data\\system.ini"))
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
	m_pTexture = NULL;
	m_pFont = NULL;
	m_bFog  = false;
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	m_pTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\sdl_logo.bmp");

	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	//similar to texture handle creation, create a display list handle
    m_uTriDisplayList = glGenLists(1);

    //compile a basic cube within this new list
    glNewList( m_uTriDisplayList, GL_COMPILE );

    glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);

    glEnd();

    //you are finished with the display list. Close it off
    glEndList();

	m_fZRotation = 0.0f;
	m_fYRotation = 0.0f;


	GLuint	fogMode = GL_EXP;	// Storage For Three Types Of Fog
	GLfloat	fogColor[4] = {0.5f,0.5f,0.5f,1.0f};		// Fog Color

	glFogi(GL_FOG_MODE, fogMode );			// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);					// Set Fog Color
	glFogf(GL_FOG_DENSITY, 0.35f);						// How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);					// Fog Hint Value
	glFogf(GL_FOG_START, 5.0f);							// Fog Start Depth
	glFogf(GL_FOG_END, 15.0f);							// Fog End Depth

	glClearColor(0.5f, 0.5f, 0.5f, 0.5f);

	m_fZPos = -10.0f;
	
	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );
	PEON_DELETE( m_pTexture );

	glDeleteLists(m_uTriDisplayList, 1);

}

void MainApp::onUpdateWorld(float fElapsedTime )
{
	m_fZRotation += 8.0f * fElapsedTime;
	m_fYRotation += 8.0f * fElapsedTime;
}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Use 'F' to enable/disable fog support" );
	m_pFont->renderText( 10.0f, 30.0f, "Use Up and Down arrows to move cube" );

	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, m_pTexture->getTex() );

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, m_fZPos);

	glRotatef(m_fYRotation, 0.0f, 1.0f, 0.0f);
	glRotatef(m_fZRotation, 0.0f, 0.0f, 1.0f );
	
	glCallList( m_uTriDisplayList );
	glPopMatrix();


}

void MainApp::onKeyEvent(SDL_KeyboardEvent* pEvent)
{

	if(pEvent->type == SDL_KEYDOWN )
	{
	
		if(pEvent->keysym.sym == SDLK_f)
		{
			m_bFog = !m_bFog;
			if(m_bFog)
			{
				glEnable(GL_FOG);


			}else
			{
				
				glDisable(GL_FOG);

			}

		}else if(pEvent->keysym.sym == SDLK_UP)
		{
			m_fZPos -= 0.3f;
		
		}else if(pEvent->keysym.sym == SDLK_DOWN)
		{
			m_fZPos += 0.3f;
		}

	

	}
}