#pragma once

//STL includes
#include <list>
#include <map>
#include <string>

//OpenAL includes
//#include <al\al.h>
#include "AL\al.h"
//#include <al\alc.h>
#include "AL\alc.h"
//#include <al\alu.h>
#include "AL\alu.h"
//#include <al\alut.h>
#include "AL\alut.h"

//Local includes
#include "RLAudioUtilities.h"
#include "RLAudioData.h"
#include "RLAudioTypes.h"

namespace RustyLib
{
	namespace Audio
	{
		//Primary Class for managing audio and hiding API
		class RLAudioManager
		{
		private:
			//Static instance of CAudioManager
			static RLAudioManager _instance;

			//Associative array of AudioData
			std::map<std::string,RLAudioData*> loadedAudioMap;
			//Audio config file path
			const static std::string CONFIG_FILE;
			//Audio Catalogue File Path (read from CONFIG_FILE)
			std::string audioCataloguePath;

			//Pointer to the current background track
			RLTrack* pCurrentTrack;

			//List of audio that has been "fired and forgotten"
			std::list<RLAudio*> forgottenAudioList;

			//List of ambient effects currently in play
			std::list<RLAmbience*> ambienceList;

			//Master volume
			float masterVolume;

			/*******Open AL Specific Data***********/

			ALCdevice *pALCdevice;

			ALCcontext *pALCcontext;

			//Listener Properties
			float ListenerPosition[3];
			float ListenerOrientation[6];

			//Play Sound Effect Methods
			//Will add audio to internal monitoring list
			void FireAndForget(const std::string& name);
			//Will return a reference for another object to track
			RLSoundEffect* FireAndReturn(const std::string& name);

			//OpenAL Initialisation Method
			void InitialiseOpenAL(void );
			//OpenAL De-Initialisation Method
			void DestroyOpenAL(void );

			/********PRIVATE INITIALISATION METHODS*******/
			//Load config data specified in the CONFIG_FILE
			void LoadConfigData(void );
			//Load audio data as specified in catalogue file
			void LoadAudioData(void );

			//Explicitly disallow the copy constructor and equals assignment
			RLAudioManager(const RLAudioManager&);
			RLAudioManager& operator=(const RLAudioManager&);

		public:
			RLAudioManager(void);
			~RLAudioManager(void);

			//Singleton Access Method
			static RLAudioManager& Instance(void );

			/*******Initialisation Method(s)********/
			//Will prepare and intialise OpenAL and load all required data
			void Initialise(void );

			/********Methods for creating sources********/
			const unsigned int GetBufferID(const std::string& name)const;
			/********Methods for getting buffer duration********/
			const float GetBufferDuration(const std::string& name)const;

			/******Background music methods**********/
			//Changes current looped track to name
			void PlayBackgroundMusic(const std::string& name);
			//Continues play of current looped track
			void PlayBackgroundMusic(void );
			//Pauses play of current looped track
			void PauseBackgroundMusic(void );
			//Stops current looped track from playing
			void StopBackgroundMusic(void );
			//Rewinds current looped track
			void RewindBackgroundMusic(void );

			/*******Sound Effect Methods*********/
			//Plays a sound effect, will return reference if capture = true
			RLSoundEffect* PlaySoundEffect(const std::string& name, const bool capturing = false);

			/*********Ambience Methods***********/
			//Will and an ambient effect to be processed
			void AddToAmbience(const std::string& name);
			//Will remove all ambient effects currently playing
			void ClearAmbience(void );

			/*********Update Method***********/
			//Core update method (allows tracking of ambient and forgotten sounds)
			void Update(float timeStep);
		};
	}
}