/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: This sample demonstrates how to render a billboard in your game world. It's one
* of those buzzwords that sound cool, but actually means something very simple: create
* a textured quad that's always facing the camera. That's it.
*/
#include "PeonMain.h"


class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	peon::SceneTexture* m_pTexture;

public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	void constructBBMatrix( float m[16], float x, float y, float z );

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicBillboard", "data\\system.ini"))
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
	m_pTexture = NULL;
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	m_pTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\sdl_logo.bmp");
	
	
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

}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Billboard Sample ");


	glBindTexture( GL_TEXTURE_2D, m_pTexture->getTex() );

	float mat[16];

    glGetFloatv( GL_MODELVIEW_MATRIX, mat );

    constructBBMatrix( mat, 0.0f, 1.0f, 0.0f ); 

    glMultMatrixf( mat );

	glTranslatef(0.0f, 0.0f, -5.0f);

	glBegin( GL_QUADS );

		glNormal3f( 0.0f, 0.0f, 1.0f );
    
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);

        glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);

        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);

        glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
    
    glEnd();





}

void MainApp::constructBBMatrix( float m[16], float x, float y, float z )
{
    float pi    = 3.141592654f;
    float theta = -180 * atan2f(m[8], m[10]) / pi;
    float d     = x*x + y*y + z*z;
    float ct    = cosf(PEON_DEGTORAD(theta));
    float st    = sinf(PEON_DEGTORAD(theta));

    // Normalize the x,y and z components just to make sure
	// remember that normalization is done by dividing each component
	// by the aggregate sum of each component squared
    if( d > 0 )
    { d = 1/d; x *= d; y *= d; z *= d; }

    //empty the view matrix
	//could use a matrix44 object here from Peon
    m[ 0] = 1; m[ 1] = 0; m[ 2] = 0; m[ 3] = 0;
    m[ 4] = 0; m[ 5] = 1; m[ 6] = 0; m[ 7] = 0;
    m[ 8] = 0; m[ 9] = 0; m[10] = 1; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;

    m[0] = x*x + ct*(1-x*x) + st*0;
    m[4] = x*y + ct*(0-x*y) + st*-z;
    m[8] = x*z + ct*(0-x*z) + st*y;

    m[1] = y*x + ct*(0-y*x) + st*z;
    m[5] = y*y + ct*(1-y*y) + st*0;
    m[9] = y*z + ct*(0-y*z) + st*-x;

    m[2] = z*x + ct*(0-z*x) + st*-y;
    m[6] = z*y + ct*(0-z*y) + st*x;
    m[10]= z*z + ct*(1-z*z) + st*0;
}