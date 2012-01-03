
/**
* This source code is part of the samples for "Game Programming in C++: Start to Finish". 
*
* @Author : Erik Yuzwa
* @Date   : 09/01/2005
* @Version: 1.0
*
* @Goal: The goal of this sample is to demonstrate how to use a physics library in
* your game world to control rigid body dynamics. Plenty of quality physics packages
* are out there, but this one is demonstrating Tokamak. It's an easy-to-use library
* which handles pretty much all the calculations for you. You simply setup the
* main simulation container, define/add the objects you want in your simulation and
* then start it up!
* 
* Please note that if you have a problem compiling/linking this sample with 
* Tokamak with the error message "cannot open include file: xmmintrin.h", then
* simply comment it out in the tokamak.h file. It's a linux header file that won't
* affect anything on Windows.
*/
#include "PeonMain.h"

#include "tokamak.h"




// This macro converts a Tokamak matrix (neT3) to matrix44.
#define NET3_TO_MATRIX44( tmat )                                        \
peon::Matrix44( tmat.rot[0][0], tmat.rot[1][0], tmat.rot[2][0], tmat.pos[0],  \
          tmat.rot[0][1], tmat.rot[1][1], tmat.rot[2][1], tmat.pos[1],  \
          tmat.rot[0][2], tmat.rot[1][2], tmat.rot[2][2], tmat.pos[2],  \
          0.0f,          0.0f,          0.0f,          1.0f           );

const int NUM_RED_CUBES = 5;
const int NUM_GREEN_PLATFORMS = 1;


LARGE_INTEGER g_performanceFrequency; // Used to store the performance frequency of the target box
LARGE_INTEGER g_startTime;            // Used to store the tick count at the beginning of each game loop
LARGE_INTEGER g_endTime;              // Used to store the tick count at the end of each game loop
float		  g_fFrameTime;           // Used to store the delta time
float		  g_fTimeSinceLastUpdate = 0.0f;
const float	CONST_FRAME_INTERVAL = 0.016666f;


struct vtx
{
	// GL_N3F_V3F compatible for some other ways to render vertices
	// We're just specifying to OpenGL that we want to define the
	// vertex normals (N3F) and the vertex position (V3F)..
	float nx, ny, nz;
	float x, y, z;
};


vtx g_cubeVertices[] =
{
	{ 0.0f, 0.0f, 1.0f,  -0.5f,-0.5f, 0.5f },
	{ 0.0f, 0.0f, 1.0f,   0.5f,-0.5f, 0.5f },
	{ 0.0f, 0.0f, 1.0f,   0.5f, 0.5f, 0.5f },
	{ 0.0f, 0.0f, 1.0f,  -0.5f, 0.5f, 0.5f },

	{ 1.0f, 0.0f, 0.0f,   0.5f,-0.5f,-0.5f },
	{ 1.0f, 0.0f, 0.0f,   0.5f, 0.5f,-0.5f },
	{ 1.0f, 0.0f, 0.0f,   0.5f, 0.5f, 0.5f },
	{ 1.0f, 0.0f, 0.0f,   0.5f,-0.5f, 0.5f },


	{ 0.0f, 0.0f,-1.0f,  -0.5f,-0.5f,-0.5f },
	{ 0.0f, 0.0f,-1.0f,  -0.5f, 0.5f,-0.5f },
	{ 0.0f, 0.0f,-1.0f,   0.5f, 0.5f,-0.5f },
	{ 0.0f, 0.0f,-1.0f,   0.5f,-0.5f,-0.5f },

	{-1.0f, 0.0f, 0.0f,  -0.5f,-0.5f,-0.5f },
	{-1.0f, 0.0f, 0.0f,  -0.5f,-0.5f, 0.5f },
	{-1.0f, 0.0f, 0.0f,  -0.5f, 0.5f, 0.5f },
	{-1.0f, 0.0f, 0.0f,  -0.5f, 0.5f,-0.5f },

	{ 0.0f, 1.0f, 0.0f,  -0.5f, 0.5f,-0.5f },
	{ 0.0f, 1.0f, 0.0f,  -0.5f, 0.5f, 0.5f },
	{ 0.0f, 1.0f, 0.0f,   0.5f, 0.5f, 0.5f },
	{ 0.0f, 1.0f, 0.0f,   0.5f, 0.5f,-0.5f },

	{ 0.0f,-1.0f, 0.0f,  -0.5f,-0.5f,-0.5f },
	{ 0.0f,-1.0f, 0.0f,   0.5f,-0.5f,-0.5f },
	{ 0.0f,-1.0f, 0.0f,   0.5f,-0.5f, 0.5f },
	{ 0.0f,-1.0f, 0.0f,  -0.5f,-0.5f, 0.5f }

};

#define NUM_VERTICES_CUBE  (sizeof(g_cubeVertices)/sizeof(vtx))


class MainApp : public peon::IApplication
{
private:
	peon::SceneTexture* m_pFontTexture;
	peon::SceneFont*    m_pFont;

	neSimulator    *m_simulator;
	neRigidBody    *m_redCubes[NUM_RED_CUBES];
	neAnimatedBody *m_greenPlane;

	peon::Vector3		m_vecLookAt;
	peon::Vector3		m_vecUp;
	peon::Vector3		m_vecEye;
	peon::Vector3		m_vecRight;

public:
	MainApp();
	~MainApp();

	bool onLoadWorld();
	void onUnloadWorld();
	void onUpdateWorld(float);
	void onRenderWorld();

	void onKeyEvent(SDL_KeyboardEvent* pEvent);

};


int main( int argc, char* argv[] )
{
	new peon::EngineCore();

	if(!peon::EngineCore::getSingleton().loadEngine("BasicCubeSim", "data\\system.ini"))
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

}

MainApp::~MainApp()
{
}

bool MainApp::onLoadWorld()
{
	using namespace peon;

	m_pFontTexture = EngineCore::getSingleton().getRenderer()->loadTexture("data\\textures\\font.png");

	m_pFont = EngineCore::getSingleton().getRenderer()->loadFont(16, 16, 14);

	glClearColor( 0.1f, 0.3f, 0.5f, 1.0f );
    glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
	glEnable( GL_NORMALIZE );

	glEnable( GL_LIGHT0 );

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		

	GLfloat diffuse_light0[]  = {  1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specular_light0[] = {  1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient );
	glLightfv( GL_LIGHT0, GL_DIFFUSE,  diffuse_light0 );
	glLightfv( GL_LIGHT0, GL_SPECULAR, specular_light0 );



	GLfloat ambient_lightModel[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_lightModel );



	// For lighting, pull our material colors from the vertex color...
	glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
	glEnable( GL_COLOR_MATERIAL );
	



	neV3 gravity;
	gravity.Set( 0.0f, -10.0f, 0.0f );

	neSimulatorSizeInfo sizeInfo;

	sizeInfo.rigidBodiesCount        = NUM_RED_CUBES;
	sizeInfo.animatedBodiesCount     = NUM_GREEN_PLATFORMS;
	sizeInfo.rigidParticleCount      = 0;
	sizeInfo.controllersCount        = 0;
	// Use the following formula to compute the number of overlapped-pairs required: 
	//(num_rigid_bodies * num_animated_bodies) + num_rigid_bodies * (num_rigid_bodies - 1) / 2
	sizeInfo.overlappedPairsCount    = (NUM_RED_CUBES * NUM_GREEN_PLATFORMS) + NUM_RED_CUBES * (NUM_RED_CUBES - 1) / 2;
	sizeInfo.geometriesCount         = NUM_RED_CUBES + NUM_GREEN_PLATFORMS;
	sizeInfo.constraintsCount        = 0;
	sizeInfo.constraintSetsCount     = 0;
	sizeInfo.constraintBufferSize    = 0;
	sizeInfo.sensorsCount            = 0;
	sizeInfo.terrainNodesStartCount  = 0;
	sizeInfo.terrainNodesGrowByCount = 0;

	m_simulator = neSimulator::CreateSimulator( sizeInfo, NULL, &gravity );

	

	for( int i = 0; i < NUM_RED_CUBES; ++i )
	{
		m_redCubes[i] = m_simulator->CreateRigidBody();
		neGeometry *cubeGeometry = m_redCubes[i]->AddGeometry();

		neV3 cubeSize;
		cubeSize.Set( 1.0f, 1.0f, 1.0f ); // Unit size
		cubeGeometry->SetBoxSize( cubeSize );

		//whenever you adjust a geometry's size, you have to let the 
		//object update it's bounding information
		m_redCubes[i]->UpdateBoundingInfo();

		float fMass = 0.1f;
		m_redCubes[i]->SetInertiaTensor( neBoxInertiaTensor( cubeSize, fMass ) );
		m_redCubes[i]->SetMass( fMass );

		neV3 position;

		if( i == 0 )
			position.Set( -2.0f, 1.5f, 0.0f );
		else if( i != 0 )
			position.Set( -1.0f + (float)(i + 1), (1.5f * (float)(i + 1)), 0.0f );

		m_redCubes[i]->SetPos( position );
	}

	

	m_greenPlane = m_simulator->CreateAnimatedBody();

	neGeometry *platformGeometry = m_greenPlane->AddGeometry();

	platformGeometry->SetBoxSize( 10.0f, 1.0f, 10.0f );

	m_greenPlane->UpdateBoundingInfo();

	neV3 position;
	position.Set( 1.0f, -1.0f, -3.0f );

	m_greenPlane->SetPos( position );

	m_vecEye = Vector3( -7.0f, 1.5f, 11.0f );
	m_vecLookAt = Vector3( 0.0f, 0.0f, -1.0f );
	m_vecUp = Vector3( 0.0f, 1.0f, 0.0f );
	m_vecRight = Vector3(1.0f, 0.0f, 0.0f );

	QueryPerformanceFrequency( &g_performanceFrequency );
    QueryPerformanceCounter( &g_startTime );



	return true;
}

void MainApp::onUnloadWorld()
{


	PEON_DELETE( m_pFont );
	PEON_DELETE( m_pFontTexture );

	neSimulator::DestroySimulator( m_simulator );

}

void MainApp::onUpdateWorld(float fElapsedTime)
{

	QueryPerformanceCounter( &g_endTime );

    // Calculate the elapsed time since the physics was last updated.
    g_fFrameTime = ( (float)g_endTime.QuadPart - (float)g_startTime.QuadPart ) 
                   / g_performanceFrequency.QuadPart;

    g_fTimeSinceLastUpdate += g_fFrameTime;

    // If the time is greater than 1/60th of a second update the physics
    if( g_fTimeSinceLastUpdate > CONST_FRAME_INTERVAL )
    {
        m_simulator->Advance( CONST_FRAME_INTERVAL, 1, NULL );
        g_fTimeSinceLastUpdate -= CONST_FRAME_INTERVAL;
    }

    QueryPerformanceCounter( &g_startTime );

    
}

void MainApp::onRenderWorld()
{
	using namespace peon;



	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( m_vecEye.x, m_vecEye.y, m_vecEye.z,
	           m_vecLookAt.x, m_vecLookAt.y, m_vecLookAt.z,
			   m_vecUp.x, m_vecUp.y, m_vecUp.z );


	GLfloat position_light0[] = { 0.0f, 0.0f, 2.0f, 1.0f };
	glLightfv( GL_LIGHT0, GL_POSITION, position_light0 );

	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_pFontTexture->getTex() );
	m_pFont->renderText( 10.0f, 10.0f, "Tokamak cube simulation" );
	m_pFont->renderText( 10.0f, 30.0f, "Use F1 to juggle random cube");
	glDisable(GL_TEXTURE_2D);
	


	Matrix44 matWorld;
	Matrix44 matScale;


	glEnable(GL_BLEND);
		
	for( int i = 0; i < NUM_RED_CUBES; i++ )
	{
		if( m_redCubes[i] )
		{
            matWorld = NET3_TO_MATRIX44( m_redCubes[i]->GetTransform() );

			
			glMatrixMode( GL_MODELVIEW ); 
            glPushMatrix();		 
            glMultMatrixf( matWorld.m );
			glColor4f( 1.0f, 0.3f, 0.3f, 0.6f );
            glInterleavedArrays( GL_N3F_V3F, 0, g_cubeVertices );
            glDrawArrays( GL_QUADS, 0, NUM_VERTICES_CUBE );
			
            glPopMatrix();
		}
	} 
	

    matWorld = NET3_TO_MATRIX44( m_greenPlane->GetTransform() );

	// Scale it so it matches the Tokamak bounding box we specified earlier
    matScale.scale( peon::Vector3( 10.0f, 1.0f, 10.0f ) ); 
    matWorld = matWorld * matScale;

	
    glPushMatrix();
	glMatrixMode( GL_MODELVIEW );
    glMultMatrixf( matWorld.m );
	glColor4f( 0.0f, 1.0f, 0.0f, 0.5f );
	glInterleavedArrays( GL_N3F_V3F, 0, g_cubeVertices );
    glDrawArrays( GL_QUADS, 0, NUM_VERTICES_CUBE );
    
    glPopMatrix();

	glDisable(GL_BLEND);

}

void MainApp::onKeyEvent(SDL_KeyboardEvent* pEvent)
{
	if(pEvent->type == SDL_KEYDOWN )
	{
		if( pEvent->keysym.sym == SDLK_F1 )
		{
			
			int rand_val = peon::MathUnit::getSingleton().randInt(0, NUM_RED_CUBES);

			neV3 vel;
			vel.Set( 0.0f, 10.0f, 0.0f );
			m_redCubes[ rand_val ]->SetVelocity( vel );
				

		}
	}

}
