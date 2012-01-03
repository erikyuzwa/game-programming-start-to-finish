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
#include "PeonFileLogger.h"

namespace peon
{
	template<> FileLogger* ISingleton<FileLogger>::ms_Singleton = 0;

	FileLogger* FileLogger::getSingletonPtr(void)
    {
        return ms_Singleton;
    }

    FileLogger& FileLogger::getSingleton(void)
    {
        assert( ms_Singleton );
		return ( *ms_Singleton );

    }


	FileLogger::FileLogger()
	{

	}

	FileLogger::~FileLogger()
	{
		closeLogStream();
	}

	bool FileLogger::openLogStream(const string& strName)
	{
		m_log_file.open( strName.c_str() );
		m_strLogName = strName;

		logInfo("FileLogger", "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++");
        logInfo("FileLogger", "+                          Peon Game SDK - Event log                           +");
        logInfo("FileLogger", "+                       (http://www.wazooinc.com/peon-sdk)                     +");
        logInfo("FileLogger", "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
        logInfo("FileLogger", "FileLogger singleton created.");


		return true;
	}

	void FileLogger::closeLogStream()
	{

		logInfo("FileLogger", "FileLogger singleton destroyed.");
		logInfo("FileLogger", "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-++");
        logInfo("FileLogger", "+                         Peon Game SDK - Event log                            +");
        logInfo("FileLogger", "+                      (http://www.wazooinc.com/peon-sdk)                      +");
        logInfo("FileLogger", "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");



		m_log_file.close();


	}

	void FileLogger::logDebug(const string& strObject, const string& strText)
	{
		string strLogString;
		TCHAR strOutput[MAX_PATH];


		sprintf(strOutput, "[%s]| DEBUG | %s", strObject.c_str(), strText.c_str() );
		strLogString = strOutput;


		writeToLogStream( strLogString );

	}

	void FileLogger::logError(const string& strObject, const string& strText)
	{
		string strLogString;
		TCHAR strOutput[MAX_PATH];

		sprintf(strOutput, "[%s] | ERROR | %s", strObject.c_str(), strText.c_str() );
		strLogString = strOutput;


		writeToLogStream( strLogString );
	}

	void FileLogger::logInfo( const string& strObject, const string& strText)
	{
		string strLogString;
		TCHAR strOutput[MAX_PATH];

		sprintf(strOutput, "[%s] | INFO | %s", strObject.c_str(), strText.c_str() );
		strLogString = strOutput;


		writeToLogStream( strLogString );

	}

	void FileLogger::logFatal( const string& strObject, const string& strText)
	{

		string strLogString;
		TCHAR strOutput[MAX_PATH];


		sprintf(strOutput, "[%s] | FATAL | %s", strObject.c_str(), strText.c_str() );
		strLogString = strOutput;


		writeToLogStream( strLogString );

	}

	void FileLogger::writeToLogStream(const string& strText)
	{

		using namespace std;

		// Write time into log
		struct tm *pTime;
		time_t ctTime; time(&ctTime);
		pTime = localtime( &ctTime );

		// write date
        m_log_file << setfill('0') << setw(2) << pTime->tm_mday << '/' <<
            setfill('0') << setw(2) << 1 + pTime->tm_mon << '/' <<
            setw(4) << (1900 + pTime->tm_year) << ' ';

		m_log_file << setw(2) << setfill('0') << pTime->tm_hour
			<< ":" << setw(2) << setfill('0') << pTime->tm_min
			<< ":" << setw(2) << setfill('0') << pTime->tm_sec << ": " << strText << endl;

		// Flush stcmdream to ensure it is written (incase of a crash, we need log to be up to date)
		m_log_file.flush();

	}
}