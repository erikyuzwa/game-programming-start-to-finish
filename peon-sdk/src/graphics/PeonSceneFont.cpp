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
#include "PeonSceneFont.h"

namespace peon
{

	SceneFont::SceneFont()
	{
		m_r = m_g = m_b = m_a = 1.0f;
		m_char_width = 16;
		m_char_height = 16;
		m_char_spacing = 14;

		m_fxCount = 16;
		m_fyCount = 16;

		m_iResWidth = 640;
		m_iResHeight = 480;

	}

	SceneFont::~SceneFont()
	{
		destroyFont();
	}

	bool SceneFont::createFont(int char_width, int char_height, int char_spacing, int res_width, int res_height)
	{

		m_iResWidth = res_width;

		m_iResHeight = res_height;

		int     loop;
		float	cx;																// Holds Our X Character Coord
		float	cy;																// Holds Our Y Character Coord
		float   cwx;															// CharWidth in texture units
		float   cwy;

		m_char_width = char_width;
		m_char_height = char_height;
		m_char_spacing = char_spacing;															// CharHeight in texture units

		cwx         = (1.0f / 256.0f) * m_char_width;
		cwy         = (1.0f / 256.0f) * m_char_height;

		m_display_list = glGenLists(m_fxCount * m_fyCount);									// Creating Display Lists

		for (loop=0; loop<(m_fxCount * m_fyCount); loop++)							// Loop Through All Lists
		{
			cx=float(loop%m_fxCount) * cwx;										// X Position Of Current Character
			cy=float(loop/m_fyCount) * cwy;										// Y Position Of Current Character

			glNewList(m_display_list + loop,GL_COMPILE);									// Start Building A List
				glBegin(GL_QUADS);												// Use A Quad For Each Character
					glTexCoord2f(cx,1-cy-cwy);									// Texture Coord (Bottom Left)
					glVertex2i(0, m_char_height);											// Vertex Coord (Bottom Left)
					glTexCoord2f(cx+cwx,1-cy-cwy);								// Texture Coord (Bottom Right)
					//glVertex2i(m_char_width - 1,0);									// Vertex Coord (Bottom Right)
					glVertex2i( m_char_width, m_char_height);
					glTexCoord2f(cx+cwx,1-cy);									// Texture Coord (Top Right)
					//glVertex2i(m_char_width - 1,m_char_height -1);							// Vertex Coord (Top Right)
					glVertex2i( m_char_width, 0);
					glTexCoord2f(cx,1-cy);										// Texture Coord (Top Left)
					//glVertex2i(0,m_char_height -1);									// Vertex Coord (Top Left)
					glVertex2i(0, 0);
				glEnd();														// Done Building Our Quad (Character)
				glTranslated(m_char_spacing,0,0);										// Move To The Right Of The Character
			glEndList();														// Done Building The Display List
		}

		return true;
	}

	void SceneFont::destroyFont()
	{
		glDeleteLists(m_display_list,m_fxCount * m_fyCount);
	}

	void SceneFont::drawText(int xpos, int ypos, const string& strText)
	{
		int width = 800;
		int height = 600;

		//int width = EngineCore::getSingleton().getRenderer()->getWidth();
		//int height= EngineCore::getSingleton().getRenderer()->getHeight();


			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glDisable(GL_DEPTH_TEST);
														// Disables Depth Testing
			glMatrixMode(GL_PROJECTION);				// Select The Projection Matrix
			glPushMatrix();								// Store The Projection Matrix
			glLoadIdentity();							// Reset The Projection Matrix
			glOrtho(0,m_iResWidth,m_iResHeight, 0, -1,1);				// Set Up An Ortho Screen
			glMatrixMode(GL_MODELVIEW);					// Select The Modelview Matrix

			glPushAttrib( GL_CURRENT_BIT );



		glPushMatrix();															// Store The Modelview Matrix
		glLoadIdentity();														// Reset The Modelview Matrix
		glTranslated(xpos,ypos,0);											// Position The Text (0,0 - Bottom Left)
		glColor4f(m_r, m_g, m_b, m_a);
		glListBase( m_display_list - 32 + 128 );
		glCallLists((int)strText.length(), GL_UNSIGNED_BYTE, strText.c_str());								// Write The Text To The Screen


			glPopAttrib();

			glMatrixMode(GL_PROJECTION);											// Select The Projection Matrix
			glPopMatrix();															// Restore The Old Projection Matrix
			glMatrixMode(GL_MODELVIEW);												// Select The Modelview Matrix
			glPopMatrix();															// Restore The Old Projection Matrix

			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);



	}

	void SceneFont::setColor(float r, float g, float b, float a)
	{
		m_r = r;
		m_g = g;
		m_b = b;
		m_a = a;
	}
}