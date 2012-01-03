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

#ifndef __PEONAUDIOENGINE_H_
#define __PEONAUDIOENGINE_H_

#include "PeonISingleton.h"

using namespace std;

namespace peon
{

	/**
	* This object is used to handle and encapsulate basic sound effects in a game.
	* Load up a blaster, level-up or any other sound here
	*/
	class PEONSDK_API SoundFxNode
	{
	public:
		/** the primary "id" handle of this sound effect */
		int m_key;

		/** the pointer to the actual sound data */
		Mix_Chunk* m_pData;

		/** the channel this sound effect is using */
		int m_channel;

	public:
		/**
		* Constructor
		*/
		SoundFxNode();

		/**
		* Destructor
		*/
		~SoundFxNode();
	};

	/**
	* This object is used to handle and encapsulate basic music media.
	* Background music tracks, etc., should probably be using this
	* object.
	*/
	class PEONSDK_API MusicNode
	{
	public:
		/** the primary handle "id" of this music track */
		int m_key;

		/** the actual data of this music track */
		Mix_Music* m_pData;

	public:
		/**
		* Constructor
		*/
		MusicNode();

		/**
		* Destructor
		*/
		~MusicNode();
	};

	/**
	* This object is used to encapsulate our audio hardware. It should
	* be able to play both MIDI tunes (yes people still use 'em) and
	* OGG/WAV files.
	*
	* The goal of the object is to scan and load up all the desired
	* media for your game using the .INI configuration file passed
	* into this object.
	*
	*/
	class PEONSDK_API AudioEngine : public ISingleton<AudioEngine>
	{

	protected:


		/** do we enable/disable sound? Note this only affects OGG and WAV playback */
		bool		m_bEnableSound;

		/** do we enable/disable music? Note this only affects MIDI playback */
		bool		m_bEnableMusic;

		/** is our audio hardware even supported? */
		bool		m_bAudioSupported;

		/** Just a boolean value if EAX is supported or not by the audio layer */
		bool		m_bIsEAXSupported;

		/** a list of our MIDI / MP3 / OGG data */
		boost::ptr_list<MusicNode> m_oMusicList;

		/** a list of our WAV sound effect data */
		boost::ptr_list<SoundFxNode> m_oSoundFxList;


	public:
		/**
		* Constructor
		*/
		AudioEngine();

		/**
		* Destructor
		*/
		~AudioEngine();

		/** Override standard Singleton retrieval.
		*/
		static AudioEngine& getSingleton(void);

		/** Override standard Singleton retrieval.
		*/
		static AudioEngine* getSingletonPtr(void);


		/**
		* This method loads and instantiates the subsystems necessary in
		* SDL_Mixer and OpenAL to get a device working for each. If the
		* initialization fails, then you can decide if you want to just
		* disable all sound, or quit the app entirely.
		* \param pConfig - The INI information
		* \return true if audio loaded properly
		*/
		bool loadEngine( IniConfig* pConfig );

		/**
		* This method frees up our allocated audio resources
		*/
		void unloadEngine();

		/**
		* This method makes the necessary calls to load up a
		* Mix_Music instance which is used for playback of
		* MIDI / OGG / MP3 files
		* \param int - key to store this music data as
		* \param strFilename - path to the MIDI file
		* \return error code of any problems
		*/
		int loadMusic( int key, const string& strFilename );

		/**
		* This method makes the necessary calls to load up a
		* Mix_Chunk instance which is used for playback of
		* MIDI files
		* \param int key - key to store this sound data as
		* \param strFilename - path to the WAV file
		* \return error code of any problems
		*/
		int loadSoundFx( int key, const string& strFilename );

		/**
		* This method is used to start the desired sound
		* effect using the handle / key.
		* \param key - int value of the key to play
		* \param loop - true / false
		* \return error code of any problems
		*/
		int startSoundFx( int key, bool loop = false );

		/**
		* This method is used to start the desired music
		* track using the handle / key.
		* \param key - int value of the key to play
		* \param loop - true / false
		* \return error code of any problems
		*/
		int startMusic( int key, bool loop = true );

		/**
		* This method stops the desired playback of the
		* sound effect
		* \param key - the int value of the key to stop
		* \return error code of any problems
		*/
		int stopSoundFx( int key );

		/**
		* This method stops the desired playback of the
		* music track
		* \param key - the int value of the key to stop
		* \return error code of any problems
		*/
		int stopMusic( int key );

		/**
		* This method just enables the global audio mask
		*/
		void enableSound(){ m_bEnableSound = true; }

		/**
		* This method disables the global audio mask
		*/
		void disableSound(){ m_bEnableSound = false; }

		/**
		* This method enables the global music mask
		*/
		void enableMusic(){ m_bEnableMusic = true; }

		/**
		* This method disables the global music mask
		*/
		void disableMusic(){ m_bEnableMusic = false; }

	};
}

#endif
