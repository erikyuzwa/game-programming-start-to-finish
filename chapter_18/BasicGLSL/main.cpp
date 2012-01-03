/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: This sample is slightly long, but the goal of it is to demonstrate how to 
* initialize and inject GLSL shading instructions into your project. Shaders are becoming
* the "future" of graphics programming, and even new video hardware has no more "real" FFP: 
* the underlying hardware simply translates your FFP instructions into shader code.
*/
#include "PeonMain.h"
#include <sys/stat.h>

// GL_ARB_shader_objects
PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB  = NULL;
PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB         = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB     = NULL;
PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB   = NULL;
PFNGLSHADERSOURCEARBPROC         glShaderSourceARB         = NULL;
PFNGLCOMPILESHADERARBPROC        glCompileShaderARB        = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB = NULL;
PFNGLATTACHOBJECTARBPROC         glAttachObjectARB         = NULL;
PFNGLGETINFOLOGARBPROC           glGetInfoLogARB           = NULL;
PFNGLLINKPROGRAMARBPROC          glLinkProgramARB          = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB   = NULL;
PFNGLUNIFORM4FARBPROC            glUniform4fARB            = NULL;
PFNGLUNIFORM1IARBPROC            glUniform1iARB            = NULL;


class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	peon::SceneTexture* m_pTexture;
	
	bool m_bSupported;

	float  m_fZRotation;
	float  m_fYRotation;

	GLhandleARB m_programObj;
	GLhandleARB m_vertexShader;
	GLhandleARB m_fragmentShader;
	GLuint      m_testTexture;



public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	void onKeyEvent( SDL_KeyboardEvent* pEvent );

	unsigned char* readShaderFile( const peon::String& fileName );

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


	m_pTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\sdl_logo.bmp");

	if( peon::EngineCore::getSingleton().getRenderer()->isExtensionSupported( "GL_ARB_shader_objects" ) )
	{
	
		glCreateProgramObjectARB  = (PFNGLCREATEPROGRAMOBJECTARBPROC)SDL_GL_GetProcAddress("glCreateProgramObjectARB");
        glDeleteObjectARB         = (PFNGLDELETEOBJECTARBPROC)SDL_GL_GetProcAddress("glDeleteObjectARB");
        glUseProgramObjectARB     = (PFNGLUSEPROGRAMOBJECTARBPROC)SDL_GL_GetProcAddress("glUseProgramObjectARB");
        glCreateShaderObjectARB   = (PFNGLCREATESHADEROBJECTARBPROC)SDL_GL_GetProcAddress("glCreateShaderObjectARB");
        glShaderSourceARB         = (PFNGLSHADERSOURCEARBPROC)SDL_GL_GetProcAddress("glShaderSourceARB");
        glCompileShaderARB        = (PFNGLCOMPILESHADERARBPROC)SDL_GL_GetProcAddress("glCompileShaderARB");
        glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)SDL_GL_GetProcAddress("glGetObjectParameterivARB");
        glAttachObjectARB         = (PFNGLATTACHOBJECTARBPROC)SDL_GL_GetProcAddress("glAttachObjectARB");
        glGetInfoLogARB           = (PFNGLGETINFOLOGARBPROC)SDL_GL_GetProcAddress("glGetInfoLogARB");
        glLinkProgramARB          = (PFNGLLINKPROGRAMARBPROC)SDL_GL_GetProcAddress("glLinkProgramARB");
        glGetUniformLocationARB   = (PFNGLGETUNIFORMLOCATIONARBPROC)SDL_GL_GetProcAddress("glGetUniformLocationARB");
        glUniform4fARB            = (PFNGLUNIFORM4FARBPROC)SDL_GL_GetProcAddress("glUniform4fARB");
		glUniform1iARB            = (PFNGLUNIFORM1IARBPROC)SDL_GL_GetProcAddress("glUniform1iARB");


		m_bSupported = true;
		
	}

	m_fYRotation = 0.0f;
	m_fZRotation = 0.0f;

	const char *vertexShaderStrings[1];
	const char *fragmentShaderStrings[1];
	GLint bVertCompiled;
	GLint bFragCompiled;
	GLint bLinked;
	char str[4096];

	
	m_vertexShader = glCreateShaderObjectARB( GL_VERTEX_SHADER_ARB );

	unsigned char *vertexShaderAssembly = readShaderFile( "data\\shaders\\basic_shader.vert" );
	vertexShaderStrings[0] = (char*)vertexShaderAssembly;
	glShaderSourceARB( m_vertexShader, 1, vertexShaderStrings, NULL );
	glCompileShaderARB( m_vertexShader);
	delete vertexShaderAssembly;

	glGetObjectParameterivARB( m_vertexShader, GL_OBJECT_COMPILE_STATUS_ARB, 
		&bVertCompiled );
	if( bVertCompiled  == false )
	{
		glGetInfoLogARB(m_vertexShader, sizeof(str), NULL, str);
	}

	

	m_fragmentShader = glCreateShaderObjectARB( GL_FRAGMENT_SHADER_ARB );

	unsigned char *fragmentShaderAssembly = readShaderFile( "data\\shaders\\basic_shader.frag" );
	fragmentShaderStrings[0] = (char*)fragmentShaderAssembly;
	glShaderSourceARB( m_fragmentShader, 1, fragmentShaderStrings, NULL );
	glCompileShaderARB( m_fragmentShader );
	delete fragmentShaderAssembly;

	glGetObjectParameterivARB( m_fragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, 
		&bFragCompiled );
	if( bFragCompiled == false )
	{
		glGetInfoLogARB( m_fragmentShader, sizeof(str), NULL, str );
	}

	

	m_programObj = glCreateProgramObjectARB();
	glAttachObjectARB( m_programObj, m_vertexShader );
	glAttachObjectARB( m_programObj, m_fragmentShader );

	
	glLinkProgramARB( m_programObj );
	glGetObjectParameterivARB( m_programObj, GL_OBJECT_LINK_STATUS_ARB, &bLinked );

	if( bLinked == false )
	{
		glGetInfoLogARB( m_programObj, sizeof(str), NULL, str );
		MessageBox( NULL, str, "Linking Error", MB_OK|MB_ICONEXCLAMATION );
	}

	
	m_testTexture = glGetUniformLocationARB( m_programObj, "sdl_tex" );



	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );

	PEON_DELETE( m_pTexture );

	glDeleteObjectARB( m_vertexShader );
	glDeleteObjectARB( m_fragmentShader );
	glDeleteObjectARB( m_programObj );


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
		m_pFont->renderText( 10.0f, 10.0f, "GL_ARB_shader_objects is not supported" );

		return;
	}

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "GL_ARB_shader_objects is supported" );


	glDisable(GL_LIGHTING);

	glUseProgramObjectARB( m_programObj );

	// Identify the texture to use and bind it to texture unit 0
	if( m_testTexture != -1 )
		glUniform1iARB( m_testTexture, 0 );

	glBindTexture(GL_TEXTURE_2D,  m_pTexture->getTex());

	glPushMatrix();

	glLoadIdentity();

       
	glTranslatef(0.0f, 0.0f, -6.0f);
	glRotatef(m_fYRotation, 0.0f, 1.0f, 0.0f);
	glRotatef(m_fZRotation, 0.0f, 0.0f, 1.0f );

    glBegin(GL_QUADS);

       
		// Front Face
		glTexCoord2f(0.0f, 0.0f);glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f( 1.0f, 0.0f);glVertex3f(1.0f, -1.0f,  1.0f);  
		glTexCoord2f( 1.0f, 1.0f);glVertex3f(1.0f, 1.0f,  1.0f);
		glTexCoord2f( 0.0f, 1.0f);glVertex3f(-1.0f, 1.0f,  1.0f);     
        
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

    // Stop drawing 
    glEnd();                                            

    glPopMatrix();

	glUseProgramObjectARB( NULL );


}

void MainApp::onKeyEvent( SDL_KeyboardEvent* pEvent )
{

}

unsigned char* MainApp::readShaderFile( const peon::String& fileName )
{
	FILE *file = fopen( fileName.c_str(), "r" );

	if( file == NULL )
	{
		return 0;
	}

	struct _stat fileStats;

	if( _stat( fileName.c_str(), &fileStats ) != 0 )
	{
		return 0;
	}

	unsigned char *buffer = new unsigned char[(int)fileStats.st_size];

	int bytes = fread( buffer, 1, (int)fileStats.st_size, file );

	buffer[bytes] = 0;

	fclose( file );

	return buffer;
}

