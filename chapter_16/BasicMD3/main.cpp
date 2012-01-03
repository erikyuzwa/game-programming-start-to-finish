/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: Although static models (aka "meshes") are created with 3D Studio Max, such as
* your dungeon or objects such as crates or guns, how do we handle monsters or other
* objects which are animated? 
* A popular animation "format" was designed and created by the geniuses over at Id Software.
* With Quake2, they released the MD2 format which loaded keyframes into a giant structure
* for each monster. This structure was broken into "ranges" for different animations. 
* If we needed to play the "death" animation, then we'd cycle through the appropriate
* frames in the range of that structure, etc.
*
* With Quake3, the Id team took the format one step further to create the MD3 format.
* 
* Currently, the MD spec is at MD5 which was created/updated for the Doom3 engine.
*
* I found several MD3 tutorials and loading code on the internet, but ultimately
* had to combine several of them (plus some obvious alterations) to get it into
* the Peon framework.
*/
#include "PeonMain.h"

class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	peon::AnimatedMD3Mesh* m_pMesh;

	float	m_YRot;


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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicMD3", "data\\system.ini"))
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

	new peon::AnimatedMeshFactory();

	m_pMesh = new peon::AnimatedMD3Mesh();

	 // Load the 3 body part meshes and their skins and texture
   	if(!m_pMesh->loadModel("data\\ash", "ash" ))
	{
		return false;
	}

    // Next, if we want to give our character a gun, we just pass in the name of the gun
    // and the path it's stored in.  I just stuck the gun in the same path as our character.

    // Load the gun and attach it to our character
    if(!m_pMesh->loadWeapon( "data\\ash", "railgun" ))
	{
		return false;
	}


	glEnable(GL_CULL_FACE);                             
    glCullFace(GL_FRONT);                               
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	

	GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat	LightPos[] =        { 0.0f, 0.0f, -100.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT0, GL_POSITION,LightPos);	// Position The Light
	glEnable(GL_LIGHT0);
	
		
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );


	float width = peon::EngineCore::getSingleton().getRenderer()->getWidth();
	float height= peon::EngineCore::getSingleton().getRenderer()->getHeight();
	peon::EngineCore::getSingleton().getRenderer()->getActiveCamera()->setPerspectiveProj(width/height, 4.0f, 4000.0f);


	m_YRot = 0.0f;

	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );

	PEON_DELETE( m_pMesh );

	delete peon::AnimatedMeshFactory::getSingletonPtr();

}

void MainApp::onUpdateWorld(float fElapsedTime)
{
	m_YRot += 15.0f * fElapsedTime;	
}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	gluLookAt(  0.0f, 5.5f, -100.0f,    
	            0.0f, 5.5f, 0.0f, 
				0.0f, 1.0f, 0.0f);

	glRotatef(m_YRot, 0.0f, 1.0f, 0.0f);

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Rendering MD3 Model" );

	m_pMesh->renderModel();

}