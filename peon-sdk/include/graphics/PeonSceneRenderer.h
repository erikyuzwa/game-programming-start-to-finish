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
#ifndef __PEONSCENERENDERER_H_
#define __PEONSCENERENDERER_H_

#include "peonstdafx.h"
#include "PeonIniConfig.h"
#include "PeonVector3.h"
#include "PeonSceneFont.h"
#include "PeonSprite.h"

namespace peon
{

	/**
	* This is a small object to manage texture data within the system.
	*/
	class PEONSDK_API SceneTexture
	{
	public:
		/** handle to our primary key */
		GLuint	m_uTex;

	public:
		/**
		* Constructor
		*/
		SceneTexture(){};

		/**
		* Destructor
		*/
		~SceneTexture(){};
	};

	/**
	* This object is our overall manager for rendering to the OpenGL device.
	*/
	class PEONSDK_API SceneRenderer
	{
	protected:
		/** OpenGL rendering surface */
		SDL_Surface*		m_pOGLSurface;

		/** surface width */
		int					m_iDeviceWidth;

		/** surface height */
		int					m_iDeviceHeight;

		/** surface bits-per-pixel */
		int					m_iBitsPerPixel;

		/** windowed or fullscreen? */
		bool				m_bWindowed;

		/** @see Vector3 which is the eye point of the camera */
		Vector3             m_vecEye;

		/** @see Vector3 which is the look at point of the camera */
		Vector3				m_vecLookAt;

		/** @see Vector3 which is the up vector of the camera */
		Vector3				m_vecUp;

		/** @see OGLTexture which is a managed list of our texture objects */
		boost::ptr_list<SceneTexture> m_oTextureList;

		/** @see OGLFont which is a managed list of our font objects */
		boost::ptr_list<SceneFont> m_oFontList;

		/** @see OGLSprite which is a managed display list */
		boost::ptr_list<Sprite> m_oDisplayList;

	protected:
		/**
		* This function just returns a pixel from a given SDL_Surface object
		* at the given coordinates
		* \param surface - pointer to the SDL_Surface
		* \param x - int location
		* \param y - int location
		* \return the pixel value
		*/
		Uint32 getPixel(SDL_Surface *surface, int x, int y);


	public:
		/**
		* Constructor
		*/
		SceneRenderer();

		/**
		* Destructor
		*/
		~SceneRenderer();

		/**
		* This method loads our configuration settings in order to prepare
		* our rendering context
		* \param pConfig - IniConfigReader object
		* \return true or false
		*/
		bool createCanvas( IniConfig* pConfig );

		/**
		* This method deallocated and cleans up our surfaces
		*/
		void destroyCanvas();

		/**
		* This method clears the back buffer and prepares it for us
		* to send rendering commands to
		* \return always true
		*/
		bool clearCanvas();

		/**
		* This method is responsible for swapping our back buffer with
		* the front one...thereby rendering a new screen
		*/
		void flipCanvas();

		/**
		* This method is called when our window is resized, hence
		* our aspect ratio may need to be updated...
		* \param width - int of our window
		* \param height - int of our window
		*/
		void restoreCanvas(int width, int height);

		/**
		* This method just loads textuer data from a given image
		* \param key - int handle of the texture
		* \param strFilename - string containing the filename
		* \param bAlpha - bool value, do we want alpha-blending?
		* \param bMipMaps - bool value, do we want mipmapping
		* \param bRepeat - do we want a repeating texture?
		* \return true or false
		*/
		bool loadTexture( int key, const string& strFilename, bool bAlpha = true, bool bMipMaps = true, bool bRepeat = false);

		/**
		* This method just sets our texture to the given handle
		* \param key - int of the texture we want
		* \return -1 if any error
		*/
		int setTexture(int key);

		/**
		* This method loads a given font data set
		*/
		bool loadFont( int key, int width = 16, int height = 16, int spacing = 14, int res_width = 640, int res_height = 480);

		/**
		* This is used for drawing / rendering text using a given font handle
		*/
		int drawText(int key, const string& strOutput, int x, int y);

		/**
		* This method is used to add a new sprite to the display list. Typically in a draw frame,
		* the purpose of this list is to only add the sprites that need to be drawn that frame.
		* \param pSprite - Sprite to add
		* \return true or false
		*/
		bool addSpriteToDisplayList( Sprite* pSprite );

		/**
		* This method is used to clean the display list between frames.
		*/
		void cleanDisplayList();

		/**
		* This method is used to set the camera for the scene
		* \param vecEye - the Vector3 eye point
		* \param vecLookAt - the Vector3 look at point
		* \param vecUp - the Vector3 up point
		*/
		void setCamera( Vector3& vecEye, Vector3& vecLookAt, Vector3& vecUp );



	};

}

#endif
