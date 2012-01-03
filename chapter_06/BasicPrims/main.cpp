/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: The goal of this sample is to show how to define and render some basic
* primitives.
*/
#include "PeonMain.h"


class MainApp : public peon::IApplication
{
private:

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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicPrims", "data\\system.ini"))
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
	
	return true;
}

void MainApp::onUnloadWorld()
{

}

void MainApp::onUpdateWorld(float)
{
}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-2.0f, 0.0f, -10.0f);
	glBegin( GL_QUADS );
		glVertex3f(-1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f, -1.0f,  1.0f);
		glVertex3f( 1.0f,  1.0f,  1.0f);
		glVertex3f(-1.0f,  1.0f,  1.0f);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(2.0f, 1.0f, -10.0f);
	glBegin( GL_TRIANGLES );
	glVertex3f( -1.0f, 0.0f, 1.0f );
	glVertex3f( 0.0f, 1.0f, 1.0f );
	glVertex3f( 1.0f, 0.0f, 1.0f );
	glEnd();
	glPopMatrix();


}

