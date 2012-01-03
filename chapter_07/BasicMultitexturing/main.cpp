/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: This sample demonstrates how to use multitexturing in your scene. In the
* past, to render a scorch mark on a wall, or maybe some brains exploding, you had
* to first set one texture, render the object, then set the other texture, render,
* and repeat. With multitexturing, you can set multiple textures at once, which
* feed into the final output for the scene.
*/
#include "PeonMain.h"


PFNGLACTIVETEXTUREARBPROC   glActiveTextureARB = NULL;
PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB = NULL;

class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	peon::SceneTexture* m_pLayer1;
	peon::SceneTexture* m_pLayer2;
	
	bool m_bSupported;

	float  m_fZRotation;
	float  m_fYRotation;



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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicMultitexturing", "data\\system.ini"))
	{
		return -1;
	}

	MainApp* pApp = new MainApp();

	peon::EngineCore::getSingleton().setApplication( pApp );


	return( peon::EngineCore::getSingleton().runEngine() );

}

MainApp::MainApp()
{
	m_bSupported = false;
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);


	m_pLayer1 = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\sdl_logo.bmp");
	m_pLayer2 = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\shockwave.png", true, false, true);

	if( peon::EngineCore::getSingleton().getRenderer()->isExtensionSupported( "GL_ARB_multitexture" ) )
	{
		glActiveTextureARB    = (PFNGLACTIVETEXTUREPROC)SDL_GL_GetProcAddress("glActiveTextureARB");
		glMultiTexCoord2fARB  = (PFNGLMULTITEXCOORD2FPROC)SDL_GL_GetProcAddress("glMultiTexCoord2fARB");
	
		m_bSupported = true;
		
	}

	m_fYRotation = 0.0f;
	m_fZRotation = 0.0f;


	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );

	PEON_DELETE( m_pLayer1 );
	PEON_DELETE( m_pLayer2 );

}

void MainApp::onUpdateWorld( float fElapsedTime )
{
	m_fZRotation += 8.0f * fElapsedTime;
	m_fYRotation += 8.0f * fElapsedTime;


}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();


	if(!m_bSupported)
	{
		glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
		m_pFont->renderText( 10.0f, 10.0f, "GL_ARB_multitexture is not supported" );

		return;
	}

	glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );

	m_pFont->renderText( 10.0f, 10.0f, "GL_ARB_multitexture is supported" );
	
	glDisable(GL_LIGHTING);

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,  m_pLayer1->getTex());

    glActiveTextureARB(GL_TEXTURE1_ARB);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,  m_pLayer2->getTex());

     glPushMatrix();

	glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -10.0f);
	glRotatef(m_fYRotation, 0.0f, 1.0f, 0.0f);
	glRotatef(m_fZRotation, 0.0f, 0.0f, 1.0f );

    glBegin(GL_QUADS);

		// Front Face
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f,  1.0f);
        
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f,  1.0f);

		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
        glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f,  1.0f);
        
        
		// Back Face
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		
		
		// Top Face
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		
		
		
		// Bottom Face
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		
		
		// Right face
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
		glVertex3f( 1.0f, -1.0f, -1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f, -1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		
		
		// Left Face
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 0.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f,  1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
		
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 1.0f);
		glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 1.0f);
		glVertex3f(-1.0f,  1.0f, -1.0f);

    // Stop drawing 
    glEnd();                                            

    glPopMatrix();




}


