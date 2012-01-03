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

#ifndef __PEONFILELOGGER_H_
#define __PEONFILELOGGER_H_


#include "peonstdafx.h"

#include "PeonISingleton.h"

using namespace std;


namespace peon {

	/**
	*
	* This object is just used to output any needed log messages to
	* a file. We COULD make this fancier (like a primitive Log4J
	* solution) by perhaps creating a LogInterface superclass, which
	* we can then derive FileLogger from. This would then allow us
	* to also create an SMTPLogger or HTMLLogger for example (to output
	* our log messages over SMTP or into a nice and purty HTML document).
	*
	* I made derived it from the ISingleton object in order to be
	* accessible pretty much everywhere in our game and/or Peon.
	*
	*/
	class PEONSDK_API FileLogger : public ISingleton<FileLogger>
	{

	protected:

		/** the file handle */
		ofstream	m_log_file;

		/** name of the file */
		string			m_strLogName;


	protected:

		/**
		* This method is responsible for writing out text to our
		* logfile. It should not be called directly, so it's
		* declared as protected.
		* \param strText - our desired text to append
		* \return void
		*/
		void writeToLogStream(const string& strText);


	public:

		/**
		* Default Constructor - the preferred method
		*/
		FileLogger();


		/**
		* Destructor
		*/
		~FileLogger();

		/** Override standard Singleton retrieval.
        */
        static FileLogger& getSingleton(void);

        /** Override standard Singleton retrieval.
        */
        static FileLogger* getSingletonPtr(void);

		/**
		* This method simply opens the file handle and prepares the
		* log file for writing
		* \param strName - our desired filename
		* \return true if we succeeded, error code otherwise
		*/
		bool openLogStream(const string& strName);

		/**
		* This method is responsible for shutting down our logging
		* and closing any file handles
		*/
		void closeLogStream();

		/**
		* This method logs a string setting it to "Info" mode. It then
		* compares it with the internal log setting to see if it should
		* be recorded or not.
		* \param strObject - object making logging call
		* \param strText - text to output
		*/
		void logInfo ( const string& strObject, const string& strText);

		/**
		* This method logs a string setting it to "Debug" mode. It then
		* compares it with the internal log setting to see if it should
		* be recorded or not.
		* \param strObject - object making logging call
		* \param strText - text to output
		*/
		void logDebug(const string& strObject, const string& strText);

		/**
		* This method logs a string setting it to "Error" mode. It then
		* compares it with the internal log setting to see if it should
		* be recorded or not.
		* \param strObject - object making logging call
		* \param strText - text to output
		*/
		void logError(const string& strObject, const string& strText);

		/**
		* This method logs a string setting it to "Fatal" mode. It then
		* compares it with the internal log setting to see if it should
		* be recorded or not.
		* \param strObject - object making logging call
		* \param strText - text to output
		*/
		void logFatal( const string& strObject, const string& strText);



	};
}

#endif
