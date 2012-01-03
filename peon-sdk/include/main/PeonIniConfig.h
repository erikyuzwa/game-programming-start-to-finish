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
#ifndef __PEONINICONFIGREADER_H_
#define __PEONINICONFIGREADER_H_

// Precompiler options
#include "peonstdafx.h"

using namespace std;


namespace peon
{
	/* This object is useful for loading and storing any information
     * we might need for our game contained in an .INI file. We can
     * store anything from our wanted renderer, to our application window
     * size, to even some basic scripting elements.
     *
     * Note that this object is completely Win32-specific. It is using
     * the GetPrivateProfile* family of API functions which I think are
     * only available on Windows.
	 */
	class PEONSDK_API IniConfig
	{

	protected:
		/** our INI filename */
		string	m_strFileName;

	public:

		/**
		* Constructor
		* \param strFile - path to INI file
		*/
		IniConfig(const string& strFile);

		/**
		* Destructor
		*/
		~IniConfig();

		/**
		* \brief This method is responsible for grabbing any string data from our
		* INI file
		* \param sSection our INI section
		* \param sKey our key name
		* \param sDefault our default key value
		* \param sReturn a string object to contain our resulting data
		* \return the size of our data string
		*/
		DWORD getString(const string sSection, const string sKey, const string sDefault, string& sReturn);

		/**
		* \brief This method is just responsible for grabbing the UINT value
		* from our INI file
		* \param sSection section name
		* \param sKeyName key name
		* \param keyval default key value
		* \return our returned key value
		*/
		UINT getInt(const string sSection, const string sKeyName, int keyval);

		/**
		* \brief This method is responsible for grabbing any boolean information
		* stored in our INI file
		* \param sSection section name
		* \param sKeyName key name
		* \param sDefault default key value "TRUE" or "FALSE"
		* \return our actual key value
		*/
		bool getBool(const string sSection, const string sKeyName, const string sDefault);

		/**
		* \brief This method is responsible for grabbing any float information stored
		* in our INI file
		* \param sSection section name
		* \param sKeyName key name
		* \param sDefault default value of float (ie. "1.0f")
		* \return our determined float value
		*/
		float getFloat(const string sSection, const string sKeyName, const string sDefault);

	};

}



#endif