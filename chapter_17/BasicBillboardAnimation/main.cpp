/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: You've seen how to present a billboard in space, which is a textured quad
* that's always facing the camera. A typical "trick" for game developers is to render
* an explosion in the game world: whether that's a grenade about to take out the enemy,
* or shell casings hitting the wall near your head. We simply create a structure that
* renders a frame of the explosion for a given period of time. As we update the structure
* it automatically chooses the next frame of animation based on the time value.
*/
#include "PeonMain.h"



class AnimatedFrame
{

public:
	AnimatedFrame()
	{
		m_pTexture = NULL;
		m_fTime = 0.0f;
	}

	~AnimatedFrame()
	{
		PEON_DELETE( m_pTexture );
	}

	peon::SceneTexture* m_pTexture;
	float m_fTime;


};

class BillboardAnimation {

public:
	bool m_bIsRunning;
	std::vector<AnimatedFrame *> m_oFrames;
	float m_fTotalTime;

	peon::Vector3 m_vecPos;
	
	
public:
	BillboardAnimation()
	{
		m_bIsRunning = false;
		m_fTotalTime = 0.0f;
	};

	~BillboardAnimation()
	{
		unloadFrames();
	}

	bool loadFrame(const peon::String& strFilename, float fTime)
	{
		AnimatedFrame *pFrame = new AnimatedFrame();
  
		// create a texture for this frame
		pFrame->m_pTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture(strFilename, true, false, false);

		
		// add to vector
		pFrame->m_fTime = fTime;

		m_oFrames.push_back(pFrame);

		return true;

	}

	void unloadFrames()
	{
		for (std::vector<AnimatedFrame *>::iterator i = m_oFrames.begin(); 
			i != m_oFrames.end(); ++i) 
		{
			delete *i;
		}
	
		m_oFrames.clear();
	}

	void updateAnim(float elapsed_time)
	{
		if(m_bIsRunning)
		{
			m_fTotalTime += elapsed_time;
			
			//just make sure that our current frame is valid. Meaning that
			//our time length for our total animation hasn't elapsed.	
			if(getCurrentFrame() > (int)m_oFrames.size())
			{
				stopAnim();
			}
		}
		
	}

	void startAnim()
	{
		m_bIsRunning = true;
		m_fTotalTime = 0.0f;
	}

	void stopAnim()
	{
		m_bIsRunning = false;
		m_fTotalTime = 0.0f;
	}
	
	int getCurrentFrame()
	{
		int iCurFrame = 0; float fTimeCount = 0.0f;
		for (std::vector<AnimatedFrame *>::iterator i = m_oFrames.begin(); i != m_oFrames.end(); ++i) {
			fTimeCount += (*i)->m_fTime;
			if (m_fTotalTime <= fTimeCount) break;
			iCurFrame++;
		}
		return(iCurFrame);
	}

	void renderAnim()
	{
		if(!m_bIsRunning)
			return;

		int iCurFrame = getCurrentFrame();

		if (iCurFrame >= (int)m_oFrames.size()) return;

		AnimatedFrame *f = m_oFrames[iCurFrame];

		glPushMatrix();
		glLoadIdentity();
		glTranslatef( m_vecPos.x, m_vecPos.y, m_vecPos.z );
		glColor4f( 1.0f, 1.0f, 1.0f, 0.9f );

		glBindTexture( GL_TEXTURE_2D, f->m_pTexture->getTex() );

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin( GL_QUADS );
		{
			glNormal3f( 0.0f, 0.0f, 1.0f );

			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);

			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);

			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 0.0f);

			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 0.0f);
		}
		glEnd();

		glPopMatrix();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);


	}

};


class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	BillboardAnimation* m_pAnim;


public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	void onKeyEvent(SDL_KeyboardEvent* pEvent );

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicBillboardAnimation", "data\\system.ini"))
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

	m_pAnim = NULL;
	
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	m_pAnim = new BillboardAnimation();

	m_pAnim->loadFrame("data\\textures\\explosion_frame01.tga", 0.07f);
	m_pAnim->loadFrame("data\\textures\\explosion_frame02.tga", 0.07f);
	m_pAnim->loadFrame("data\\textures\\explosion_frame03.tga", 0.07f);
	m_pAnim->loadFrame("data\\textures\\explosion_frame04.tga", 0.10f);
	m_pAnim->loadFrame("data\\textures\\explosion_frame05.tga", 0.07f);
	m_pAnim->loadFrame("data\\textures\\explosion_frame06.tga", 0.07f);
	m_pAnim->loadFrame("data\\textures\\explosion_frame07.tga", 0.10f);
	m_pAnim->loadFrame("data\\textures\\explosion_frame08.tga", 0.07f);

	m_pAnim->m_vecPos = peon::Vector3( 0.0f, 0.0f, -10.0f );

	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );

	PEON_DELETE( m_pAnim );

}

void MainApp::onUpdateWorld( float fElapsedTime )
{

	m_pAnim->updateAnim( fElapsedTime );
}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Billboard Animation ");
	m_pFont->renderText( 10.0f, 30.0f, " Press Left Control to run explosion" );

	m_pAnim->renderAnim();

}

void MainApp::onKeyEvent(SDL_KeyboardEvent* pEvent )
{
	if(pEvent->type == SDL_KEYDOWN)
	{

		if(pEvent->keysym.sym == SDLK_LCTRL)
		{
			m_pAnim->stopAnim();
			m_pAnim->startAnim();

		}

	}
}