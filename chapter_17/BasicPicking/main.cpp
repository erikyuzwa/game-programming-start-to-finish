/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: Depending upon your game, an essential component to selecting a unit in your
* game world (ie. when casting a spell to destroy that Zombie), you need a way to
* select a unit within a 3D game world. The typical algorithm is to take your mouse coordinates
* at the spot you hit the button, and cast a ray into the game world. Whatever that ray
* intersects with, becomes the selected unit. There's a few ways to get this done, but
* for OpenGL we can use the selection buffer and gluUnProject.
*
*/
#include "PeonMain.h"

#define SUN 101
#define MOON 102
#define EARTH 103



class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;
	peon::SceneTexture* m_pTexture;

	peon::String		m_strChosen;
	peon::String	    m_strPos;

	bool				m_bToggle;

public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	void onMouseEvent(SDL_Event* pEvent );
	int testObjectID(int x, int y);
	void renderCube(float r, float g, float b, float a);

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicPicking", "data\\system.ini"))
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

	m_bToggle = false;
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	m_pTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\sdl_logo.bmp");


	m_strChosen = "";
	m_strPos = "";
	
	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );
	PEON_DELETE( m_pTexture );

}

void MainApp::onUpdateWorld(float)
{
}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	if(!m_bToggle)
	{
		glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
		m_pFont->renderText( 10.0f, 10.0f, "Picking Example. Click something!" );
		m_pFont->renderText( 10.0f, 30.0f, m_strPos );
		m_pFont->renderText( 10.0f, 50.0f, m_strChosen );
		glDisable(GL_LIGHTING);
	}

	glBindTexture(GL_TEXTURE_2D, m_pTexture->getTex() );

	glInitNames();
	glPushName( 0 ); //push at least one name on the stack
	//for the EARTH object
	//do the same for the MOON and the SUN
	glLoadName( SUN );
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-3.0f, 2.0f, -10.0f);

	//render 
	renderCube(1.0f, 1.0f, 0.0f, 1.0f );

	glPopMatrix();
	

	//do the same for the MOON and the SUN
	glLoadName( MOON );
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(2.0f, 2.0f, -10.0f);

	//render 
	renderCube(0.5f, 0.5f, 0.5f, 1.0f );

	glPopMatrix();
	

	glLoadName( EARTH );
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -10.0f);

	//render 
	renderCube(0.0f, 1.0f, 0.0f, 1.0f );

	
	glPopMatrix();
	
	

	

}

void MainApp::onMouseEvent(SDL_Event* pEvent )
{
	int obj_ID;
	char strTemp[MAX_PATH];

	if(pEvent->type == SDL_MOUSEBUTTONDOWN)
	{
	
		// Here we pass in the cursors X and Y co-ordinates to test an
		//object under the mouse.
		obj_ID = testObjectID(pEvent->button.x,pEvent->button.y);

		sprintf(strTemp, "Position of click and ObjID: (x,y):(%d, %d) : (%d)", pEvent->button.x,pEvent->button.y, obj_ID);
		m_strPos = strTemp;

		switch( obj_ID )
		{
			case MOON:
				m_strChosen = "Moon";
			break;
			case SUN:
				m_strChosen = "Sun";
			break;
			case EARTH:
				m_strChosen = "Earth";
			break;
		}

	}else
	{
		sprintf(strTemp, "Position of mouse: (x,y):(%d, %d)", pEvent->motion.x,pEvent->motion.y);
		m_strPos = strTemp;
		m_strChosen = "";
	}
}

int MainApp::testObjectID(int x, int y)
{
	int window_width = peon::EngineCore::getSingleton().getRenderer()->getWidth();
	int window_height= peon::EngineCore::getSingleton().getRenderer()->getHeight();

	int objectsFound = 0;                                // This will hold the amount of objects clicked
	int    viewportCoords[4] = {0};                        // We need an array to hold our view port coordinates

	unsigned int selectBuffer[32] = {0};                

	
	glSelectBuffer(32, selectBuffer);                    // Setup our selection buffer to accept object ID's

	
	glGetIntegerv(GL_VIEWPORT, viewportCoords);            // Get the current view port coordinates

	
	glMatrixMode(GL_PROJECTION);                        // We want to now effect our projection matrix

	glPushMatrix();                                        // We push on a new matrix so we don't effect our 3D projection

	
	glRenderMode(GL_SELECT);                        // Allows us to render the objects, but not change the frame buffer

	

	glLoadIdentity();                                // Reset our projection matrix

	
	gluPickMatrix(x, viewportCoords[3] - y, 2, 2, viewportCoords);

	
	gluPerspective(45.0f,(float)window_width/(float)window_height,1.0f,100.0f);

	glMatrixMode(GL_MODELVIEW);                        // Go back into our model view matrix

	m_bToggle = true;

	onRenderWorld();                                    // Now we render into our selective mode to pinpoint clicked objects

	m_bToggle = false;

	
	objectsFound = glRenderMode(GL_RENDER);            // Return to render mode and get the number of objects found

	glMatrixMode(GL_PROJECTION);                    // Put our projection matrix back to normal.
	glPopMatrix();                                        // Stop effecting our projection matrix

	glMatrixMode(GL_MODELVIEW);                            // Go back to our normal model view matrix

	
	if (objectsFound > 0)
	{        
		unsigned int lowestDepth = selectBuffer[1];

		int selectedObject = selectBuffer[3];

		for(int i = 1; i < objectsFound; i++)
		{
			if(selectBuffer[(i * 4) + 1] < lowestDepth)
			{
				// Set the current lowest depth
				lowestDepth = selectBuffer[(i * 4) + 1];

				// Set the current object ID
				selectedObject = selectBuffer[(i * 4) + 3];
			}
		}

		// Return the selected object
		return selectedObject;
	}

	// We didn't click on any objects so return 0
	return 0;                                            
}

void MainApp::renderCube(float r, float g, float b, float a)
{
	glBegin(GL_QUADS);

	glColor4f( r, g, b, a);
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

