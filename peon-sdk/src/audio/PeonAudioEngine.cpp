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
#include "PeonIniConfig.h"
#include "PeonAudioEngine.h"


namespace peon
{

	SoundFxNode::SoundFxNode()
	{
		m_key = 0;
		m_pData = NULL;
		m_channel = -1;
	}

	SoundFxNode::~SoundFxNode()
	{

	}

	MusicNode::MusicNode()
	{
		m_key = 0;
		m_pData = NULL;
	}

	MusicNode::~MusicNode()
	{

	}


	template<> AudioEngine* ISingleton<AudioEngine>::ms_Singleton = 0;

	AudioEngine* AudioEngine::getSingletonPtr(void)
	{
		return ms_Singleton;
	}

	AudioEngine& AudioEngine::getSingleton(void)
	{

		assert( ms_Singleton );

		return ( *ms_Singleton );
	}

	AudioEngine::AudioEngine()
	{


		m_bEnableSound = true;
		m_bEnableMusic = true;

		m_bAudioSupported = true;
		m_bIsEAXSupported = false;


	}

	AudioEngine::~AudioEngine()
	{
		unloadEngine();
	}

	bool AudioEngine::loadEngine( IniConfig* pConfig )
	{
		int audio_rate = 22050;
		Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
		int audio_channels = 2;
		int audio_buffers = 4096;



		// This is where we open up our audio device.
		// Note that this section is for loading up SDL_Mixer for
		// MIDI support
		if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
		{
			FileLogger::getSingleton().logError("AudioEngine", "Error loading SDL_Mixer audio device");
			m_bAudioSupported = false;
			return false;
		}



		return true;
	}

	void AudioEngine::unloadEngine()
	{
		m_oSoundFxList.clear();

		m_oMusicList.clear();

		Mix_HaltMusic();

		Mix_CloseAudio();



	}

	int AudioEngine::loadMusic( int key, const string& strFilename )
	{
		MusicNode* pNode = new MusicNode();
		int status = -1;

		/* Actually loads up the music */
		pNode->m_pData = Mix_LoadMUS(strFilename.c_str());

		if( pNode->m_pData != NULL )
		{
			status = 1;
			pNode->m_key = key;
			m_oMusicList.push_back( pNode );
		}


		return status;
	}

	int AudioEngine::loadSoundFx(int key, const string& strFilename )
	{
		SoundFxNode* pNode = new SoundFxNode();
		int status = -1;

		pNode->m_pData = Mix_LoadWAV( strFilename.c_str() );

		if( pNode->m_pData != NULL )
		{

			status = 1;
			pNode->m_key = key;
			m_oSoundFxList.push_back( pNode );
		}


		return status;

	}

	int AudioEngine::startMusic(int key, bool loop /* = true */)
	{
		int found = -1;
		int counter = 0;
		for( boost::ptr_list<MusicNode>::iterator i = m_oMusicList.begin();
			i != m_oMusicList.end(); ++i)
		{
			if( key == i->m_key )
			{
				if( loop )
				{
					counter = -1;
				}

				Mix_PlayMusic( i->m_pData, counter );

				break;
			}
		}

		return found;
	}

	int AudioEngine::startSoundFx(int key, bool loop /* = false */ )
	{
		int found = -1;
		int counter = 0;
		for( boost::ptr_list<SoundFxNode>::iterator i = m_oSoundFxList.begin();
			i != m_oSoundFxList.end(); ++i)
		{
			if( key == i->m_key )
			{
				if(loop)
				{
					counter = -1;
				}

				i->m_channel = Mix_PlayChannel( -1, i->m_pData, counter );

				break;

			}
		}

		return found;

	}

	int AudioEngine::stopMusic(int key)
	{
		int found = -1;
		for( boost::ptr_list<MusicNode>::iterator i = m_oMusicList.begin();
			i != m_oMusicList.end(); ++i)
		{
			if( key == i->m_key )
			{

				Mix_HaltMusic();
				break;

			}
		}

		return found;
	}

	int AudioEngine::stopSoundFx(int key)
	{
		int found = -1;
		for( boost::ptr_list<SoundFxNode>::iterator i = m_oSoundFxList.begin();
			i != m_oSoundFxList.end(); ++i)
		{
			if( key == i->m_key )
			{
				Mix_HaltChannel( i->m_channel );
				i->m_channel = -1;
				break;

			}
		}

		return found;
	}

}

