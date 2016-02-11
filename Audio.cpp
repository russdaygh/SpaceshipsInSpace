// RustyAL.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Audio.h"
#include "RLAudioManager.h"
#include "RLSoundEffect.h"
using RustyLib::Audio::SoundEffect;

namespace RustyLib
{
	namespace Audio
	{
		bool InitialiseRustyAL(void )
		{
			//Establish a reference to the instance of AudioManager
			RLAudioManager *audio;
			audio = &RLAudioManager::Instance();
			if(audio)
			{
				audio->Initialise();
				return true;
			}
			else
			{
				return false;
			}
		}


		void PlayBackgroundMusic(std::string name)
		{
			//Establish a reference to the instance of AudioManager
			RLAudioManager *audio;
			audio = &RLAudioManager::Instance();
			if(audio)
			{
				audio->PlayBackgroundMusic(name);
			}
		}


		void PlayBackgroundMusic(void )
		{
			//Establish a reference to the instance of AudioManager
			RLAudioManager *audio;
			audio = &RLAudioManager::Instance();
			if(audio)
			{
				audio->PlayBackgroundMusic();
			}
		}


		void PauseBackgroundMusic(void )
		{
			//Establish a reference to the instance of AudioManager
			RLAudioManager *audio;
			audio = &RLAudioManager::Instance();
			if(audio)
			{
				audio->PauseBackgroundMusic();
			}
		}


		void StopBackgroundMusic(void )
		{
			//Establish a reference to the instance of AudioManager
			RLAudioManager *audio;
			audio = &RLAudioManager::Instance();
			if(audio)
			{
				audio->StopBackgroundMusic();
			}
		}


		SoundEffect* PlaySoundEffect(std::string name, bool capture)
		{
			//Establish a reference to the instance of AudioManager
			RLAudioManager *audio;
			audio = &RLAudioManager::Instance();
			if(audio)
			{
				return (SoundEffect*)audio->PlaySoundEffect(name, capture);
			}
			else
			{
				return 0;
			}
		}


		void AddToAmbience(std::string name)
		{
			//Establish a reference to the instance of AudioManager
			RLAudioManager *audio;
			audio = &RLAudioManager::Instance();
			if(audio)
			{
				audio->AddToAmbience(name);
			}
		}


		void ClearAmbience(void )
		{
			//Establish a reference to the instance of AudioManager
			RLAudioManager *audio;
			audio = &RLAudioManager::Instance();
			if(audio)
			{
				audio->ClearAmbience();
			}
		}


		void Update(float timeStep)
		{
			//Establish a reference to the instance of AudioManager
			RLAudioManager *audio;
			audio = &RLAudioManager::Instance();
			if(audio)
			{
				audio->Update(timeStep);
			}
		}
	}
}