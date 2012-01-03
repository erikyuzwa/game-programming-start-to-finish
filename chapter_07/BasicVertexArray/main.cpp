/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: This sample is a demonstration of using VertexArrays within your scene. It's
* yet another mechanism provided by OpenGL to speed up rendering objects in your
* game world. Instead of feeding the vertices into the pipeline "by hand" you're 
* caching them. 
*/
#include "PeonMain.h"

struct vtx
{
	float x, y, z;
};

vtx g_cubeVertices[] =
{
	{ -1.0f,-1.0f, 1.0f },
	{  1.0f,-1.0f, 1.0f },
	{  1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f },

	{ -1.0f,-1.0f,-1.0f },
	{ -1.0f, 1.0f,-1.0f },
	{  1.0f, 1.0f,-1.0f },
	{  1.0f,-1.0f,-1.0f },

	{ -1.0f, 1.0f,-1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{  1.0f, 1.0f, 1.0f },
	{  1.0f, 1.0f,-1.0f },

	{ -1.0f,-1.0f,-1.0f },
	{  1.0f,-1.0f,-1.0f },
	{  1.0f,-1.0f, 1.0f },
	{ -1.0f,-1.0f, 1.0f },

	{  1.0f,-1.0f,-1.0f },
	{  1.0f, 1.0f,-1.0f },
	{  1.0f, 1.0f, 1.0f },
	{  1.0f,-1.0f, 1.0f },

	{ -1.0f,-1.0f,-1.0f },
	{ -1.0f,-1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f },
	{ -1.0f, 1.0f,-1.0f }
};

struct color_vtx
{
	float r, g, b;
};

color_vtx g_cubeColors[] =
{
	{ 1.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 0.0f },

	{ 0.5f, 1.0f, 0.0f },
	{ 0.5f, 1.0f, 0.0f },
	{ 0.5f, 1.0f, 0.0f },
	{ 0.5f, 1.0f, 0.0f },

	{ 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f },
	{ 0.0f, 0.0f, 1.0f },

	{ 0.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, 1.0f },

	{ 1.0f, 1.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },

	{ 1.0f, 0.5f, 1.0f },
	{ 1.0f, 0.5f, 1.0f },
	{ 1.0f, 0.5f, 1.0f },
	{ 1.0f, 0.5f, 1.0f }
};

struct tex_vtx
{
	float tu, tv;
};

tex_vtx g_cubeTexCoords[] =
{
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f },

	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f },
	{ 0.0f, 0.0f },

	{ 0.0f, 1.0f },
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },

	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f },
	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },

	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f },
	{ 0.0f, 0.0f },

	{ 0.0f, 0.0f },
	{ 1.0f, 0.0f },
	{ 1.0f, 1.0f },
	{ 0.0f, 1.0f }
};



class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pTexture;

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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicVertexArray", "data\\system.ini"))
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
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\sdl_logo.bmp");
	
	m_fZRotation = 0.0f;
	m_fYRotation = 0.0f;

	
	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pTexture );

}

void MainApp::onUpdateWorld(float fElapsedTime)
{
	m_fZRotation += 8.0f * fElapsedTime;
	m_fYRotation += 8.0f * fElapsedTime;

}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pTexture->getTex() );

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);

	glRotatef(m_fYRotation, 0.0f, 1.0f, 0.0f);
	glRotatef(m_fZRotation, 0.0f, 0.0f, 1.0f );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	
	glVertexPointer( 3, GL_FLOAT, 0, g_cubeVertices );
	glColorPointer( 3, GL_FLOAT, 0, g_cubeColors );
	glTexCoordPointer( 2, GL_FLOAT, 0, g_cubeTexCoords );
	
	glDrawArrays( GL_QUADS, 0, 24 );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );


	glPopMatrix();
}



