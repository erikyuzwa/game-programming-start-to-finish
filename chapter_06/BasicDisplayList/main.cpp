/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* 
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
* 
* @Goal: The goal of this sample is to demonstrate how to use a basic display list to 
* create a cache of vertices that can be quickly processed by OpenGL.
*/
#include "PeonMain.h"

class MainApp : public peon::IApplication
{
private:
	GLuint m_uTriDisplayList;

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
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicDisplayList", "data\\system.ini"))
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
	//similar to texture handle creation, create a display list handle
    m_uTriDisplayList = glGenLists(1);

    //compile a basic triangle within this new list
    glNewList( m_uTriDisplayList, GL_COMPILE );

    glBegin( GL_TRIANGLES );
    glVertex3f( -1.0f, 0.0f, -10.0f );
    glVertex3f( 0.0f, 1.0f, -10.0f);
    glVertex3f(1.0f, 0.0f, -10.0);

    glEnd();

    //you are finished with the display list. Close it off
    glEndList();

	
	return true;
}

void MainApp::onUnloadWorld()
{
	glDeleteLists(m_uTriDisplayList, 1);

}

void MainApp::onUpdateWorld(float)
{
}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);
	glCallList( m_uTriDisplayList );
	glPopMatrix();

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 2.0f, -1.0f);
	glCallList( m_uTriDisplayList );
	glPopMatrix();


	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, -2.0f, -10.0f);
	glCallList( m_uTriDisplayList );
	glPopMatrix();



}


