/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
* 
* @Goal: A popular technique for hidden surface removal is the occluder algorithm. Very
* simply, it's an algorithm used to detect if a polygon/object is in the view
* frustum, but hidden by another object in the scene: Say a soldier hiding behind a building
* just in front of you. It provides another mechanism to remove vertices before they
* are injected into the pipeline.
*/
#include "PeonMain.h"

PFNGLGENQUERIESARBPROC        glGenQueriesARB        = NULL;
PFNGLDELETEQUERIESARBPROC     glDeleteQueriesARB     = NULL;
PFNGLISQUERYARBPROC           glIsQueryARB           = NULL;
PFNGLBEGINQUERYARBPROC        glBeginQueryARB        = NULL;
PFNGLENDQUERYARBPROC          glEndQueryARB          = NULL;
PFNGLGETQUERYIVARBPROC        glGetQueryivARB        = NULL;
PFNGLGETQUERYOBJECTIVARBPROC  glGetQueryObjectivARB  = NULL;
PFNGLGETQUERYOBJECTUIVARBPROC glGetQueryObjectuivARB = NULL;


class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pTexture;
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*	m_pFont;

	float  m_fZRotation;
	float  m_fYRotation;

	GLuint m_planeQuery;
	GLuint m_boxQuery;

	bool   m_bSupported;

public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	void renderSceneToDepthBuffer();
	void renderSceneToQuery();

	void renderCube(float x,float y, float z);

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicOccluder", "data\\system.ini"))
	{
		return -1;
	}

	MainApp* pApp = new MainApp();

	peon::EngineCore::getSingleton().setApplication( pApp );


	return( peon::EngineCore::getSingleton().runEngine() );

}

MainApp::MainApp()
{
	m_pTexture = NULL; 
	m_bSupported = false;
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\sdl_logo.bmp");
	
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	m_fZRotation = 0.0f;
	m_fYRotation = 0.0f;

	if( peon::EngineCore::getSingleton().getRenderer()->isExtensionSupported( "GL_ARB_occlusion_query" ) )
	{
		glGenQueriesARB        = (PFNGLGENQUERIESARBPROC)SDL_GL_GetProcAddress("glGenQueriesARB");
        glDeleteQueriesARB     = (PFNGLDELETEQUERIESARBPROC)SDL_GL_GetProcAddress("glDeleteQueriesARB");
        glIsQueryARB           = (PFNGLISQUERYARBPROC)SDL_GL_GetProcAddress("glIsQueryARB");
        glBeginQueryARB        = (PFNGLBEGINQUERYARBPROC)SDL_GL_GetProcAddress("glBeginQueryARB");
        glEndQueryARB          = (PFNGLENDQUERYARBPROC)SDL_GL_GetProcAddress("glEndQueryARB");
        glGetQueryivARB        = (PFNGLGETQUERYIVARBPROC)SDL_GL_GetProcAddress("glGetQueryivARB");
        glGetQueryObjectivARB  = (PFNGLGETQUERYOBJECTIVARBPROC)SDL_GL_GetProcAddress("glGetQueryObjectivARB");
        glGetQueryObjectuivARB = (PFNGLGETQUERYOBJECTUIVARBPROC)SDL_GL_GetProcAddress("glGetQueryObjectuivARB");



		m_bSupported = true;

		glGenQueriesARB( 1, &m_boxQuery );
		glGenQueriesARB( 1, &m_planeQuery );
		
	}

	glEnable(GL_DEPTH_TEST);
    
	glDepthFunc(GL_LEQUAL);
	
	

	
	return true;
}

void MainApp::onUnloadWorld()
{
	glDeleteQueriesARB( 1, &m_boxQuery );
    glDeleteQueriesARB( 1, &m_planeQuery );

	PEON_DELETE( m_pTexture );
	PEON_DELETE( m_pFontTexture );
	PEON_DELETE( m_pFont );

}

void MainApp::onUpdateWorld(float fElapsedTime)
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
		m_pFont->renderText( 10.0f, 10.0f, "GL_ARB_occlusion_query is not supported" );

		return;
	}

	glDisable(GL_LIGHTING);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -15.0f);

	glRotatef(-m_fYRotation, 0.0f, 1.0f, 0.0f);
	glRotatef(-m_fZRotation, 0.0f, 0.0f, 1.0f );

	    
    renderSceneToDepthBuffer();


    renderSceneToQuery();
	

	GLuint planeFragmentCount;
    GLuint boxFragmentCount;
    char planeString[50];
    char boxString[50];

    glGetQueryObjectuivARB( m_planeQuery, GL_QUERY_RESULT_ARB, &planeFragmentCount);
    glGetQueryObjectuivARB( m_boxQuery, GL_QUERY_RESULT_ARB, &boxFragmentCount);

	sprintf( planeString, "Plane Fragments = %d", planeFragmentCount );
    sprintf( boxString, "Box Fragments = %d", boxFragmentCount );

    glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "GL_ARB_occlusion_query is supported" );
	m_pFont->renderText( 10.0f, 30.0f, planeString );
	m_pFont->renderText( 10.0f, 50.0f, boxString );



	glPopMatrix();
}

void MainApp::renderSceneToDepthBuffer()
{
    glPushMatrix();   
	glTranslatef( 0.0f, -0.25f, -2.0f);
    glColor3f( 1.0f, 1.0f, 0.0f );
    renderCube( 1.4f, 1.4f, 0.2f);  
    glPopMatrix();

   
    glPushMatrix();
    glTranslatef( 0.0f, 0.25f, 0.0f );
    glColor3f( 1.0f, 0.0f, 0.0f );
    renderCube( 1.0f, 1.0f, 1.0f );
    glPopMatrix();

}


void MainApp::renderSceneToQuery()
{
	

    glPushMatrix();
    glTranslatef( 0.0f, -0.25f, -2.0f);
       
    glBeginQueryARB( GL_SAMPLES_PASSED_ARB, m_planeQuery );
       
    glColor3f( 1.0f, 1.0f, 0.0f );
    renderCube( 1.4f, 1.4f, 0.2f );
       
    glEndQueryARB( GL_SAMPLES_PASSED_ARB );
   
    glPopMatrix();

  
    glPushMatrix();
    
    glTranslatef( 0.0f, 0.25f, 0.0f );

    glBeginQueryARB( GL_SAMPLES_PASSED_ARB, m_boxQuery );
       
    glColor3f( 1.0f, 0.0f, 0.0f );
    renderCube( 1.0f, 1.0f, 1.0f );
        
    glEndQueryARB( GL_SAMPLES_PASSED_ARB );
    
    glPopMatrix();
}

void MainApp::renderCube( float x, float y, float z)
{
	glScalef( x, y, z);


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

}
