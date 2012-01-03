/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: The goal of this sample is to show how to display a simple particle within 
* our game world. We just keep a list of particle objects and as we scroll through it,
* we render a textured quad in that position of the game world.
*/
#include "PeonMain.h"


struct Particle
{
	float x, y, z;
	float fWidth;
	float fHeight;
	bool bActive;

};

class Emitter 
{
public:
	std::vector<Particle*> m_oParticles;
	bool m_bIsRunning;


public:
	Emitter()
	{
		m_bIsRunning = false;
	}

	~Emitter()
	{
		unloadParticles();

	}

	bool loadParticle(float x, float y, float z, float width, float height)
	{
		Particle* pParticle = new Particle();
		
		pParticle->x = x;
		pParticle->y = y;
		pParticle->z = z;
		pParticle->fWidth = width;
		pParticle->fHeight= height;
		pParticle->bActive = true;

	
		m_oParticles.push_back(pParticle);

		return true;

	}

	void start()
	{
		m_bIsRunning = true;
	}

	void stop()
	{
		m_bIsRunning = false;
	}

	void unloadParticles()
	{
		m_bIsRunning = false;

		for (std::vector<Particle*>::iterator i = m_oParticles.begin(); 
			i != m_oParticles.end(); ++i) 
		{
			delete *i;
		}
	
		m_oParticles.clear();

	}

	void update(float fElapsedTime)
	{
		if(m_bIsRunning)
		{

			Particle* p;
			float velocity = 0.0f;
			for (std::vector<Particle*>::iterator i = m_oParticles.begin(); 
				i != m_oParticles.end(); ++i) 
			{
				p = (*i);

				if(p->bActive)
				{
					if( p->z > -10.0f && p->z < -8.0f )
						velocity = 10.0f;

					if( p->z > -8.0f && p->z < -6.0f )
						velocity = 20.0f;

					if( p->z > -6.0f )
						velocity = 35.0f;


					p->y -= 0.3f * velocity * fElapsedTime;

					if(p->y < -5.0f)
						p->y = 5.0f;

				}
		
			}

		}
	}

	void render()
	{
		if(m_bIsRunning)
		{
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			Particle* p;
			for (std::vector<Particle*>::iterator i = m_oParticles.begin(); 
				i != m_oParticles.end(); ++i) 
			{
				p = (*i);
			

				glPushMatrix();
				glLoadIdentity();
				glTranslatef( p->x, p->y, p->z );

			
				glBegin( GL_QUADS );
				{
					glTexCoord2f(0.0f, 0.0f);glVertex3f(-p->fWidth, -p->fHeight, 1.0f);

					glTexCoord2f(1.0f, 0.0f);glVertex3f(p->fWidth, -p->fHeight, 1.0f);

					glTexCoord2f(1.0f, 1.0f);glVertex3f(p->fWidth, p->fHeight, 1.0f);

					glTexCoord2f(0.0f, 1.0f); glVertex3f(-p->fWidth, p->fHeight, 1.0f);
				}
				glEnd();

				glPopMatrix();

				

			}

			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}
	}

};


class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	peon::SceneTexture* m_pTex;

	Emitter*			m_pEmitter;

public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	void onKeyEvent(SDL_KeyboardEvent* pEvent );

	float randFloat(const float& min, const float& max);

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();
	
	if(!peon::EngineCore::getSingleton().loadEngine("BasicParticle", "data\\system.ini"))
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
	
	m_pTex = NULL;
	m_pEmitter = NULL;
}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	m_pFontTexture = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");
	
	m_pFont = peon::EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	m_pTex = peon::EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\star.tga");


	m_pEmitter = new Emitter();

	int width = peon::EngineCore::getSingleton().getRenderer()->getWidth();
	int height= peon::EngineCore::getSingleton().getRenderer()->getHeight();

	float x, y, z;
	for(int i = 0; i < 1000; i++)
	{

		x = randFloat( -4.5f, 4.5f);
		y = randFloat( -4.5f, 5.5f);
		z = randFloat( -10.0f, -5.0f );

		m_pEmitter->loadParticle( x, y, z, 0.05f, 0.05f );


	}

	m_pEmitter->start();
		

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glShadeModel( GL_SMOOTH );

	
	return true;
}

void MainApp::onUnloadWorld()
{
	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );
	PEON_DELETE( m_pTex );
	PEON_DELETE( m_pEmitter );


}

void MainApp::onUpdateWorld(float fElapsedTime)
{
	m_pEmitter->update( fElapsedTime );

}

void MainApp::onRenderWorld()
{
	peon::SceneRenderer* pRenderer = peon::EngineCore::getSingleton().getRenderer();

	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Rendering Particle Storm!" );

	glDisable( GL_LIGHTING );
	glBindTexture( GL_TEXTURE_2D, m_pTex->getTex() );

	m_pEmitter->render();


}

void MainApp::onKeyEvent(SDL_KeyboardEvent* pEvent )
{
	if(pEvent->type == SDL_KEYDOWN)
	{

		if(pEvent->keysym.sym == SDLK_LCTRL)
		{
			m_pEmitter->stop();
			m_pEmitter->start();

		}

	}
}

float MainApp::randFloat(const float& min, const float& max)
{
	float range = max - min;
	float num = range * rand() / RAND_MAX;
	return (num + min);
}
