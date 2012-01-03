/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: The purpose of this "small" sample is to demonstrate how to load a common
* model format into your game. OpenGL is just a graphics library, and so it doesn't know
* anything about anything else. We have to load the model into our rendering engine to
* use it in the game. A popular binary modeling format used in games is the 3DS format
* created by the 3DS Max family of software.
*
* I started off with the 3DS Loader tutorial over at www.morrowland.com, created
* by Ronny, then I had to massage, twist,bend and somewhat alter his code to fit into the
* Peon framework. The model.3DS and accompanying box, wall and floor textures also originate from
* the 3DS Loader tutorial found at www.morrowland.com copyrighted 2002 by Ronny André Reierstad
*/
#include "PeonMain.h"

class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	float				m_fYRotation;

	peon::s3DModel*     m_pMesh;


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
	
	if(!peon::EngineCore::getSingleton().loadEngine("Basic3DS", "data\\system.ini"))
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

	m_pMesh = NULL;
		
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	
	m_fYRotation = -30.0f;

	new peon::MeshFactory();

	m_pMesh = peon::MeshFactory::getSingleton().loadMeshFrom3DS( "data\\media\\model.3DS", "data\\media" );
	


	
	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );

	PEON_DELETE( m_pMesh );

	delete peon::MeshFactory::getSingletonPtr();

}

void MainApp::onUpdateWorld(float fElapsedTime)
{
	m_fYRotation += 8.0f * fElapsedTime;
	if(m_fYRotation > 60.0f )
		m_fYRotation = -30.0f;
}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();
	
	gluLookAt(0.0f,5.0f,7.0f, 
		      0.0f,3.0f,0.0f, 
			  0.0f,1.0f,0.0f);

	//static float angle = 0;								// rotate
	glRotatef(m_fYRotation,0,1.0f,0);

	m_pMesh->onRender();
	
	//static float dir = 1.0f;							// change direction	
	//angle += 0.03f*dir;
	//if(angle>45.0f || angle<0) dir *= (-1);


}


