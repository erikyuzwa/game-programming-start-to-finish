/**

Peon-SDK
---------

Peon-SDK - Copyright (c) 2002-2011 Erik Yuzwa - erik@erikyuzwa.com

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment
   in the product documentation would be appreciated but
   is not required.

2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
   source distribution.
**/
#include "PeonSceneRenderer.h"
#include "PeonFileLogger.h"

namespace peon
{
	SceneRenderer::SceneRenderer()
	{
		m_iDeviceWidth = 640;
		m_iDeviceHeight = 480;
		m_iBitsPerPixel = 16;
		m_bWindowed = true;

		m_pOGLSurface = NULL;

		m_vecEye = Vector3( 0.0f, 0.0f, 5.0f );
		m_vecLookAt = Vector3( 0.0f, 0.0f, 0.0f );
		m_vecUp = Vector3( 0.0f, 1.0f, 0.0f );

	}

	SceneRenderer::~SceneRenderer()
	{
		destroyCanvas();

	}

	bool SceneRenderer::createCanvas( IniConfig* pConfig )
	{
		m_iDeviceWidth  = (int)pConfig->getInt("Application", "WindowWidth", 640);
		m_iDeviceHeight = (int)pConfig->getInt("Application", "WindowHeight", 480);
		m_iBitsPerPixel  = (int)pConfig->getInt("Application", "WindowDepth", 16);
		m_bWindowed     = pConfig->getBool("Application", "Windowed", "TRUE");

		Uint32 iFlags;
		iFlags    = SDL_OPENGL;     // it's an openGL window
		iFlags   |= SDL_HWPALETTE;  // exclusive access to hardware colour palette
		iFlags   |= SDL_RESIZABLE;  // the window must be resizeable

		// query SDL for information about our video hardware
		const SDL_VideoInfo *pVideoInfo = SDL_GetVideoInfo();

		if(pVideoInfo == NULL)
		{
			FileLogger::getSingleton().logFatal("Renderer", "Error querying video information");
			return false;
		}


		if( !m_bWindowed )
		{
			iFlags |= SDL_FULLSCREEN;
		}


		//we set the system dependant flags here
		if(pVideoInfo -> hw_available)
			iFlags |= SDL_HWSURFACE;
		else
			iFlags |= SDL_SWSURFACE;

		// Blitting is fast copying / moving /swapping of contiguous sections of memory
		// more blitting info: http://www.csc.liv.ac.uk/~fish/HTML/blitzman/bm_blitter.html
		// is hardware blitting available
		if(pVideoInfo -> blit_hw)
			iFlags |= SDL_HWACCEL;


		// tell SDL that the GL drawing is going to be double buffered
		// along with specifying the size of depth buffer. The
		// rest we don't really need.
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,   m_iBitsPerPixel);
		SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 0);
		SDL_GL_SetAttribute( SDL_GL_ACCUM_RED_SIZE, 0);
		SDL_GL_SetAttribute( SDL_GL_ACCUM_GREEN_SIZE, 0);
		SDL_GL_SetAttribute( SDL_GL_ACCUM_BLUE_SIZE, 0);
		SDL_GL_SetAttribute( SDL_GL_ACCUM_ALPHA_SIZE, 0);


		//log our information...for now just log the width x height x bit depth
		//of our requested surface
		char strOutput[1024];
		sprintf(strOutput, "Renderer surface: (%d x %d x %d)", m_iDeviceWidth, m_iDeviceHeight,
			m_iBitsPerPixel);
		FileLogger::getSingleton().logInfo("Renderer", strOutput );


		if(m_bWindowed)
		{
			sprintf(strOutput, "Renderer surface: applying windowed");
		}else
			sprintf(strOutput, "Renderer surface: applying fullscreen");



		FileLogger::getSingleton().logInfo("Renderer", strOutput );


		//This is the method which creates/generates our SDL_Surface
		//according to the creation parameters we've set above. The
		//SDL documentation is a good place to look for more information,
		//but at the end of the day this should create our OpenGL
		//surface.
		m_pOGLSurface = SDL_SetVideoMode( m_iDeviceWidth, m_iDeviceHeight, m_iBitsPerPixel, iFlags );
		if(NULL == m_pOGLSurface)
		{
			FileLogger::getSingleton().logError("Renderer", "Failed to create the OpenGL surface!");
			return false;
		}


		glClearColor( 0.0f, 0.0f, 1.0f, 1.0f );


		const char *glVendor = (const char *) glGetString(GL_VENDOR);
		const char *glRenderer = (const char *) glGetString(GL_RENDERER);
		const char *glVersion = (const char *) glGetString(GL_VERSION);


		sprintf(strOutput, "Renderer Vendor: %s", glVendor );
		FileLogger::getSingleton().logInfo("Renderer", strOutput );

		sprintf(strOutput, "Renderer Stats: %s", glRenderer );
		FileLogger::getSingleton().logInfo("Renderer", strOutput );

		sprintf(strOutput, "Renderer Version: %s", glVersion );
		FileLogger::getSingleton().logInfo("Renderer", strOutput );

		restoreCanvas( m_iDeviceWidth, m_iDeviceHeight );



		return true;
	}

	void SceneRenderer::restoreCanvas( int width, int height )
	{

		if(height < 1)
			height = 1;



		//set some default OpenGL properties
		glEnable( GL_TEXTURE_2D );

		glShadeModel( GL_SMOOTH );

		glEnable( GL_DEPTH_TEST );

		glFrontFace( GL_CW );


		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		float fAspect = (float)width / (float)height;

		glViewport( 0, 0, width, height );

		glMatrixMode (GL_PROJECTION);										// Select The Projection Matrix
		glLoadIdentity ();													// Reset The Projection Matrix
		gluPerspective (45.0f, (GLfloat)(width)/(GLfloat)(height),			// Calculate The Aspect Ratio Of The Window
						1.0f, 100.0f);
		glMatrixMode (GL_MODELVIEW);										// Select The Modelview Matrix
		glLoadIdentity ();													// Reset The Modelview Matrix


		gluLookAt( m_vecEye.x, m_vecEye.y, m_vecEye.z, /* eye point */
			       m_vecLookAt.x, m_vecLookAt.y, m_vecLookAt.z, /* look at */
				   m_vecUp.x, m_vecUp.y, m_vecUp.z); /* up vector */

		/*
		m_pActiveCamera = new SceneCamera();

		m_pActiveCamera->setPerspectiveProj( fAspect, 0.1f, 100.0f );


		m_pActiveCamera->setViewMatrix( Vector3( 0.0f, 0.0f, 10.0f ),
									    Vector3( 0.0f, 0.0f, 0.0f ),
										Vector3( 0.0f, 1.0f, 0.0f ) );
		*/


	}

	void SceneRenderer::destroyCanvas()
	{

		//unload our boost map of textures
		m_oTextureList.clear(); // All pointers held in list are deleted.

		m_oFontList.clear(); // all pointers in font list are deleted

		m_oDisplayList.clear();

		//PEON_DELETE( m_pActiveCamera );

		SDL_FreeSurface( m_pOGLSurface );

	}

	void SceneRenderer::flipCanvas()
	{
		glFlush();
		SDL_GL_SwapBuffers();
	}

	bool SceneRenderer::clearCanvas()
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLoadIdentity();

		return true;
	}

	bool SceneRenderer::loadTexture(int key, const string& strFilename, bool bAlpha /* = true */, bool bMipMaps /* = true */, bool bRepeat /* = false */)
	{
		SceneTexture* pTex = new SceneTexture();


		//load the image data to an SDL_Surface structure
		SDL_Surface* pTexSurface = IMG_Load( strFilename.c_str() );
		if( NULL == pTexSurface )
		{
			//error
			return false;
		}

		//calculate the total size of the image data. If you are needing
		//the alpha channel then account for that
		int dim = pTexSurface->w * pTexSurface->h * ((bAlpha) ? 4: 3);
		GLubyte *pData = new GLubyte[ dim ];

		//loop through our SDL_Surface and copy it into the array
		//if the image has an extra alpha channel of information then
		//be sure to append that
		int pos = 0;
		for( int y = (pTexSurface->h) - 1; y > -1; y-- )
		{
			for(int x = 0; x < pTexSurface->w; x++)
			{
				Uint8 r, g, b, a;

				//getPixel is defined in the SDL documentation. It just
				//grabs the pixel data from a given SDL_Surface at
				//coordinates x,y
				Uint32 color = getPixel(pTexSurface, x, y);

				if(!bAlpha)
					SDL_GetRGB( color, pTexSurface->format, &r, &g, &b);

				else
					SDL_GetRGBA( color, pTexSurface->format, &r, &g, &b, &a);

				pData[pos] = r; pos++;
				pData[pos] = g; pos++;
				pData[pos] = b; pos++;
				if( bAlpha )
					pData[pos] = a; pos++;
			}
		}


		int type = (bAlpha) ? GL_RGBA : GL_RGB;
		glGenTextures(1, &pTex->m_uTex );		// Generate texture ID
		glBindTexture(GL_TEXTURE_2D, pTex->m_uTex);

		int filter_min, filter_mag;


		filter_min = (bMipMaps) ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST;
		filter_mag = GL_NEAREST;

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			filter_min);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
			filter_mag);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			(bRepeat) ? GL_REPEAT : GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			(bRepeat) ? GL_REPEAT : GL_CLAMP);

		if(bMipMaps)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, type, pTexSurface->w, pTexSurface->h,
				type, GL_UNSIGNED_BYTE, pData);
		}else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, type, pTexSurface->w,
				pTexSurface->h, 0, type, GL_UNSIGNED_BYTE, pData);
		}

		//now that we are finished, do some garbage collection
		//clean up our array and destroy the surface you loaded
		delete [] pData;
		SDL_FreeSurface( pTexSurface );







		//load this puppy into our managed texture list
		m_oTextureList.push_back( pTex );


		return true;
	}

	int SceneRenderer::setTexture(int key)
	{

		int found = -1;
		for( boost::ptr_list<SceneTexture>::iterator i = m_oTextureList.begin();
			i != m_oTextureList.end(); ++i)
		{

			if( i->m_uTex == key )
			{
				found = 1;
				glBindTexture( GL_TEXTURE_2D, i->m_uTex );
                break;
			}

		}

		if( found < 0 )
		{
			TCHAR strOutput[512];
			sprintf(strOutput, "Couldn't find texture: %d", key);
			FileLogger::getSingleton().logError("SceneRenderer", strOutput);

		}

		return found;

	}

	/*
	* Return the pixel value at (x, y)
	* NOTE: The surface must be locked before calling this!
	* Taken from SDL documentation.
	*/
	Uint32 SceneRenderer::getPixel(SDL_Surface *surface, int x, int y)
	{
		int bpp = surface->format->BytesPerPixel;
		/* Here p is the address to the pixel we want to retrieve */
		Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

		switch(bpp) {
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
		}
	}

	bool SceneRenderer::loadFont( int key, int width, int height, int spacing, int res_width, int res_height)
	{

		SceneFont* pFont = new SceneFont();

		pFont->m_iKey = key;

		pFont->createFont( width, height, spacing, res_width, res_height );


		//load this puppy into our managed texture list
		m_oFontList.push_back( pFont );


		return true;
	}



	int SceneRenderer::drawText(int key, const string& strOutput, int x, int y)
	{

		int found = -1;


		for( boost::ptr_list<SceneFont>::iterator i = m_oFontList.begin();
			i != m_oFontList.end(); ++i)
		{

			if( i->m_iKey == key )
			{
				found = 1;

				i->drawText( x, y, strOutput );

				//glBindTexture( GL_TEXTURE_2D, i->m_uTex );
                break;
			}

		}

		if( found < 0 )
		{
			TCHAR strOutput[512];
			sprintf(strOutput, "Couldn't find texture: %d", key);
			FileLogger::getSingleton().logError("SceneRenderer", strOutput);

		}

		return found;

	}

	bool SceneRenderer::addSpriteToDisplayList( Sprite* pSprite )
	{

		//load this puppy into our managed texture list
		m_oDisplayList.push_back( pSprite );


		return true;
	}

	void SceneRenderer::cleanDisplayList()
	{
		m_oDisplayList.clear();

	}

	void SceneRenderer::setCamera( Vector3& vecEye, Vector3& vecLookAt, Vector3& vecUp )
	{
		m_vecEye = vecEye;
		m_vecLookAt = vecLookAt;
		m_vecUp = vecUp;

	}



}

