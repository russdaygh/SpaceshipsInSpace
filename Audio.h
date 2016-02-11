#pragma once
/******RustyAL Main header*******
Will include static methods to utilise the library
It will provide methods of playing sounds through
the AudioManager whilst obfuscating the implementation
details */
#include "RLSoundEffect.h"
using RustyLib::Audio::SoundEffect;

#include "stdafx.h"

namespace RustyLib
{
	namespace Audio
	{
		/********SETUP/INITIALISATION METHODS***********/
		//Returns true if successfully initialised
		bool InitialiseRustyAL(void );

		/********BACKGROUND/MUSIC METHODS***********/
		//Plays the named track on loop at music volume
		void PlayBackgroundMusic(std::string name);

		//Continues playback of the current background music
		void PlayBackgroundMusic(void );

		//Pauses play of current background music
		void PauseBackgroundMusic(void );

		//Stops play of current background music
		void StopBackgroundMusic(void );

		/**********SOUND_EFFECT METHODS**********/
		//Plays the specified SoundEffect, will return a reference if capture = true
		SoundEffect* PlaySoundEffect(std::string name, bool capture = false);

		/**********AMBIENCE METHODS************/
		//Adds a named audio item to the ambience
		void AddToAmbience(std::string name);

		//Clears the ambient items
		void ClearAmbience(void );

		/**********UPDATE METHODS************/
		//Core update method (allows tracking of ambience and forgotten sounds
		void Update(float timeStep);
	}
}