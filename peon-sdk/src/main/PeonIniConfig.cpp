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
#include "PeonIniConfig.h"

namespace peon
{

	IniConfig::IniConfig(const string& strFile)
	{
		//TCHAR strOutput[MAX_PATH];
		//sprintf(strOutput, "attempting to load ini: %s\n", strFile.c_str());
		//OutputDebugString(strOutput);
		m_strFileName = strFile;

	}

	IniConfig::~IniConfig()
	{

	}

	DWORD IniConfig::getString(const string strSection, const string strKey, const string strDefault, string& strReturn)
	{

		DWORD dwResult = 0;
		TCHAR strTemp[MAX_PATH];
		ZeroMemory( &strTemp, sizeof( strTemp ) );
		dwResult = GetPrivateProfileString(strSection.c_str(),
										strKey.c_str(),
										strDefault.c_str(),
										strTemp,
										MAX_PATH,
										m_strFileName.c_str());

		strReturn = strTemp;


		return dwResult;
	}

	UINT IniConfig::getInt(const string strSectionName, const string strKeyName, int iDefault)
	{

		UINT iResult;

		iResult = GetPrivateProfileInt(strSectionName.c_str(),
									strKeyName.c_str(),
									iDefault,
									m_strFileName.c_str());
		return iResult;
	}

	bool IniConfig::getBool(const string strSection, const string strKey, const string strDefault)
	{

		string strTemp = "";

		getString(strSection, strKey, strDefault, strTemp);

		if(strTemp == "TRUE")
		{
			return true;
		}else
		{
			return false;
		}

	}

	float IniConfig::getFloat(const string strSection, const string strKey, const string strDefault)
	{

		string strTemp;

		getString(strSection, strKey, strDefault, strTemp);

		return (float)strtod(strTemp.c_str(),TEXT('\0'));


	}


}
