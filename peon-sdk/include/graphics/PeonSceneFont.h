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
#ifndef __PEONSCENEFONT_H_
#define __PEONSCENEFONT_H_

#include "peonstdafx.h"

using namespace std;

namespace peon
{
	/**
	* This object of the Peon library is meant to encapsulate a textured
	* font object for your game. You provide it with a bitmap image
	* of the alphabet you want to use (in ASCII format), then just notify
	* this object of the dimensions of the bitmap. ie. how many rows
	* and columns of characters there are, along with the pixel width
	* and height of a single character.
	*
	* This object is based off of the GLFont object made available by Jeff
	* Molofee (aka. Nehe) at http://nehe.gamedev.net, an excellent
	* repository of OpenGL tutorials.
	*/
	class PEONSDK_API SceneFont
	{
	public:
		/** key value used to identify this font object */
		int m_iKey;

	protected:
		/** display list to contain alphabet */
		GLuint m_display_list;

		/** width of a single character */
		int m_char_width;

		/** height of a single character */
		int m_char_height;

		/** spacing between characters */
		int m_char_spacing;

		/** how many characters in a line/row */
		int m_fxCount;

		/** how many characters in a column */
		int m_fyCount;

		/** the current red color when rendering our text*/
		float m_r;

		/** the current green color when rendering our text*/
		float m_g;

		/** the current blue color when rendering our text*/
		float m_b;

		/** the current alpha color when rendering our text*/
		float m_a;

		/** the current resolution width */
		int m_iResWidth;

		/** the current resolution height */
		int m_iResHeight;


	public:
		/**
		* Constructor
		*/
		SceneFont();

		/**
		* Destructor
		*/
		~SceneFont();

		/**
		* This method is responsible for loading up our SceneFont
		* instance.
		* \param width -
		* \param height -
		* \param spacing -
		* \param res_width -
		* \param res_height -
		* \return true if everything loaded properly
		*/
		bool createFont(int width = 16, int height = 16, int spacing = 14, int res_width = 640, int res_height = 480);

		/**
		* This method is just used to unload our SceneFont instance
		*/
		void destroyFont();

		/**
		* This method is the "workhorse" which renders our text. We
		* specify the x and y position of the text and that's where
		* the SceneFont will render it!
		* \param xpos - x position of text
		* \param ypos - y position of text
		* \param strText - our text to render
		*/
		void drawText(int xpos, int ypos, const string& strText);

		/**
		* This method just sets our color components in case we want
		* to apply any color to the text we're about to render.
		* \param r - red component
		* \param g - green component
		* \param b - blue component
		* \param a - alpha component
		*/
		void setColor( float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f);




	};


}

#endif