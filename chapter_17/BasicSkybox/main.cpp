/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: The purpose of this sample is to demonstrate how to render a skybox in your
* game world. They're nothing new and provides an easy "cheap" way to present some
* good eye candy for the game. You're simply rendering a cube around the position of
* the camera.
*/
#include "PeonMain.h"




class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;
	
	peon::Skybox*		m_pSkybox;

	peon::Vector3		m_vecLookAt;
	peon::Vector3		m_vecUp;
	peon::Vector3		m_vecEye;

public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	void onMouseEvent( SDL_Event* pEvent );
	void rotateView(float x, float y, float z);

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicSkybox", "data\\system.ini"))
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
	m_pFont = NULL;
	m_pSkybox = NULL;
	
}

MainApp::~MainApp()
{
	

}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	m_pSkybox = new peon::Skybox();
	m_pSkybox->m_vecPos = peon::Vector3( 0.0f, 0.0f, 0.0f );
	m_pSkybox->m_vecDim = peon::Vector3( 100.0f, 50.0f, 100.0f);

	m_pSkybox->load( "data\\textures\\rt.jpg",
					 "data\\textures\\lt.jpg",
					 "data\\textures\\ft.jpg",
					 "data\\textures\\bk.jpg",
					 "data\\textures\\up.jpg",
					 "data\\textures\\dn.jpg");

	m_vecEye = peon::Vector3( 0.0f, 0.0f, 0.0f );
	m_vecLookAt = peon::Vector3( 0.0f, 0.0f, 6.0f );
	m_vecUp = peon::Vector3( 0.0f, 1.0f, 0.0f );

	
	
	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );
	PEON_DELETE( m_pSkybox );

	

}

void MainApp::onUpdateWorld( float fElapsedTime )
{
	

}

void MainApp::onRenderWorld()
{

	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	gluLookAt( m_vecEye.x, m_vecEye.y, m_vecEye.z,
	           m_vecLookAt.x, m_vecLookAt.y, m_vecLookAt.z,
			   m_vecUp.x, m_vecUp.y, m_vecUp.z );


	m_pSkybox->render( 0.0f, 0.0f, 0.0f );

	//glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
//	m_pFont->renderText( 10.0f, 10.0f, "rendering your skybox" );
	

}

void MainApp::onMouseEvent( SDL_Event* pEvent )
{


	int mousePos_x, mousePos_y;	
	int mid_x = peon::EngineCore::getSingleton().getRenderer()->getWidth()  >> 1;	
	int mid_y = peon::EngineCore::getSingleton().getRenderer()->getHeight() >> 1;	
	float angle_y  = 0.0f;				
	float angle_z  = 0.0f;							
	
	SDL_GetMouseState(&mousePos_x, &mousePos_y);	// Get the 2D mouse cursor (x,y) position					
	
	if( (mousePos_x == mid_x) && (mousePos_y == mid_y) ) return;

	SDL_WarpMouse(mid_x, mid_y);	// Set the mouse cursor in the middle of the window						

	// Get the direction from the mouse, and bring the number down to a reasonable amount
	angle_y = (float)( (mid_x - mousePos_x) ) / 1000;		
	angle_z = (float)( (mid_y - mousePos_y) ) / 1000;

	// The higher the number(acceleration) is the faster the camera looks around.
	m_vecLookAt.y += angle_z * 2;

	// limit the rotation around the x-axis
	if((m_vecLookAt.y - m_vecEye.y) > 8)  m_vecLookAt.y = m_vecEye.y + 8;
	if((m_vecLookAt.y - m_vecEye.y) <-8)  m_vecLookAt.y = m_vecEye.y - 8;
	
	rotateView(0, -angle_y, 0); // Rotate
	
}

void MainApp::rotateView(float x, float y, float z)
{
	peon::Vector3 tVector = m_vecLookAt - m_vecEye;

	if(x) 
	{
		m_vecLookAt.z = (float)(m_vecEye.z + sin(x)*tVector.y + cos(x)*tVector.z);
		m_vecLookAt.y = (float)(m_vecEye.y + cos(x)*tVector.y - sin(x)*tVector.z);
	}
	
	if(y) 
	{
		m_vecLookAt.z = (float)(m_vecEye.z + sin(y)*tVector.x + cos(y)*tVector.z);
		m_vecLookAt.x = (float)(m_vecEye.x + cos(y)*tVector.x - sin(y)*tVector.z);
	
	}
	
	if(z) 
	{
		m_vecLookAt.x = (float)(m_vecEye.x + sin(z)*tVector.y + cos(z)*tVector.x);		
		m_vecLookAt.y = (float)(m_vecEye.y + cos(z)*tVector.y - sin(z)*tVector.x);
	}
}


