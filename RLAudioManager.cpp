#include "StdAfx.h"
#include "RLAudioManager.h"

namespace RustyLib
{
	namespace Audio
	{
		//Constant definitions
		RLAudioManager RLAudioManager::_instance = RLAudioManager();
		const std::string RLAudioManager::CONFIG_FILE = "config\\AudioProperties.cfg";


		RLAudioManager::RLAudioManager(void):
			pCurrentTrack(NULL)
		{
		}


		RLAudioManager::~RLAudioManager(void)
		{
			//Items to be deallocated

			//forgottenAudioList
			std::list<RLAudio*>::iterator iter_ForgottenAudio;

			for(iter_ForgottenAudio = forgottenAudioList.begin();
				iter_ForgottenAudio != forgottenAudioList.end();
				iter_ForgottenAudio++)
			{
				delete(iter_ForgottenAudio._Ptr->_Myval);
			}
			forgottenAudioList.clear();

			//ambienceList
			std::list<RLAmbience*>::iterator iter_Ambience;

			for(iter_Ambience = ambienceList.begin();
				iter_Ambience != ambienceList.end();
				iter_Ambience++)
			{
				delete(iter_Ambience._Ptr->_Myval);
			}
			ambienceList.clear();

			//loadedAudioMap
			std::map<std::string,RLAudioData*>::iterator iter_AudioData;

			for(iter_AudioData = loadedAudioMap.begin();
				iter_AudioData != loadedAudioMap.end();
				iter_AudioData++)
			{
				delete(iter_AudioData._Ptr->_Myval.second);
			}
			loadedAudioMap.clear();

			this->DestroyOpenAL();
		}


		RLAudioManager& RLAudioManager::Instance(void )
		{
			return _instance;
		}


		void RLAudioManager::Initialise(void )
		{
			//Set up device and context
			this->InitialiseOpenAL();
			this->LoadConfigData();
			//Load specified audio data and buffer into OpenAL
			this->LoadAudioData();

			//Pass the Audio class a reference to the Singleton
			//now it is ready for use
			RLAudio::SetAudioManager(RLAudioManager::Instance());
		}


		void RLAudioManager::LoadConfigData(void )
		{
			//Temporary vars for properties
			float masterVol,trackVol,seVol,ambVol;
			std::string cataloguePath;

			//Check the CONFIG_FILE for contents
			std::ifstream configFile;
			configFile.open(CONFIG_FILE);

			if(configFile.is_open())
			{
				//Format is defined as following
				/*
				[DefaultAudioProperties]
				MasterVolume
				value
				MusicVolume
				value
				SoundEffectVolume
				value
				AmbienceVolume
				value

				[LoadingScripts]
				CatalogueFile
				value
				*/

				char *discard = new char[256];//For discarding lines with getline

				//Get [DefaultAudioProperties]
				configFile.getline(discard,256);

				//Get MasterVolume
				configFile.getline(discard,256);
				configFile.operator>>(masterVol);
				configFile.get();//Get EOL

				//Get MusicVolume
				configFile.getline(discard,256);
				configFile.operator>>(trackVol);
				configFile.get();//Get EOL

				//Get SoundEffectVolume
				configFile.getline(discard,256);
				configFile.operator>>(seVol);
				configFile.get();//Get EOL

				//Get AmbienceVolume
				configFile.getline(discard,256);
				configFile.operator>>(ambVol);
				configFile.get();//Get EOL

				//Discard [LoadingScripts] (blank line preceding)
				configFile.get();
				configFile.getline(discard, 256);
		
				//Get CatalogueFile
				configFile.getline(discard, 256);
				configFile.getline(discard, 256);
				cataloguePath = discard;

				configFile.close();
			}
			else
			{
				masterVol = 1.0f;
				trackVol = 1.0f;
				seVol = 1.0f;
				ambVol = 1.0f;
				cataloguePath = "assets\\audio\\AudioCatalogue.cat";
			}

			this->masterVolume = masterVol;
			RLTrack::Volume( trackVol );
			RLSoundEffect::Volume( seVol );
			RLAmbience::Volume( ambVol );

			this->audioCataloguePath = cataloguePath;
		}


		void RLAudioManager::LoadAudioData(void )
		{
			//Open the file
			std::ifstream catFile;
			catFile.open(this->audioCataloguePath);

			if(catFile.is_open())
			{
				//File successfully opened

				//Temporary vars for file extraction
				std::list<std::string> fileNames;
				char *fileStr = new char[256];
				RLAudioData *pAudioData;

				while(catFile.eof() == false)
				{
					catFile.getline(fileStr, 256);

					pAudioData = new RLAudioData(fileStr);

					this->loadedAudioMap[pAudioData->Name()] = pAudioData;
				}

				catFile.close();
			}

			/**********Buffer Data into OpenAL************/
			std::map<std::string, RLAudioData*>::iterator dataIter;

			for(dataIter = loadedAudioMap.begin();
				dataIter != loadedAudioMap.end();
				dataIter++)
			{
				dataIter->second->BufferData();
			}
		}


		const unsigned int RLAudioManager::GetBufferID(const std::string& name)const
		{
			//Attempt retrieval from loadedAudioMap
			std::map<std::string, RLAudioData*>::const_iterator data = loadedAudioMap.find(name);

			//If we haven't had loadedAudioMap.end returned we have a key-value pair
			if(data != loadedAudioMap.end())
			{
				return data->second->GetBufferID();
			}
			else
			{
				//We have an error. either stack an error, return it
				//or throw an exception 
				//Probs stack an error
				return INVALID_BUFFER;
			}
		}


		const float RLAudioManager::GetBufferDuration(const std::string& name)const
		{
			//Attempt retrieval from loadedAudioMap
			std::map<std::string, RLAudioData*>::const_iterator data = loadedAudioMap.find(name);

			//If we haven't had loadedAudioMap.end returned we have a key-value pair
			if(data != loadedAudioMap.end())
			{
				return data->second->GetBufferDuration();
			}
			else
			{
				//We have an error. either stack an error, return it
				//or throw an exception 
				//Probs stack an error
				return INVALID_BUFFER;
			}
		}


		void RLAudioManager::PlayBackgroundMusic(const std::string& name)
		{
			//Find the key-value pair
			std::map<std::string, RLAudioData*>::iterator track = loadedAudioMap.find(name);
			//If the key does not exist in the map it returns loadedAudioMap.end
			if(track != loadedAudioMap.end())
			{
				//We have a successful find
		
				//We should make sure to destroy the current track if one exists
				if(pCurrentTrack)
				{
					delete(pCurrentTrack);
					pCurrentTrack = NULL;
				}

				pCurrentTrack = new RLTrack(name);
				pCurrentTrack->Initialise();
				pCurrentTrack->Play();
			}
			else
			{
				//We have an error, we could return an error enum
				//or throw an exception, although this would make it critical.
			}
		}


		void RLAudioManager::PlayBackgroundMusic(void )
		{
			if(this->pCurrentTrack)
			{
				this->pCurrentTrack->Play();
			}
		}


		void RLAudioManager::PauseBackgroundMusic(void )
		{
			if(pCurrentTrack)
			{
				this->pCurrentTrack->Pause();
			}
		}


		void RLAudioManager::StopBackgroundMusic(void )
		{
			this->pCurrentTrack->Stop();
		}


		void RLAudioManager::RewindBackgroundMusic(void )
		{
			this->pCurrentTrack->Rewind();
		}


		RLSoundEffect* RLAudioManager::PlaySoundEffect(const std::string& name, const bool capturing)
		{
			if(capturing)
			{
				return FireAndReturn(name);
			}
			else
			{
				FireAndForget(name);
				return 0;
			}
		}


		void RLAudioManager::AddToAmbience(const std::string& name)
		{
			//Construct and Initialise
			RLAmbience* amb = new RLAmbience(name);
			amb->Initialise();

			ambienceList.push_back(amb);

			amb->Play();
		}


		void RLAudioManager::ClearAmbience(void )
		{
			std::list<RLAmbience*>::iterator iterAmb;

			for(iterAmb = ambienceList.begin();
				iterAmb != ambienceList.end();
				iterAmb++)
			{
				delete(iterAmb._Ptr->_Myval);
			}
			ambienceList.clear();
		}

		/************PRIVATE METHODS****************/
		void RLAudioManager::FireAndForget(const std::string& name)
		{
			RLSoundEffect *pSE;

			pSE = new RLSoundEffect(name);
			pSE->Initialise();
			pSE->Play();

			this->forgottenAudioList.push_back(pSE);
		}

		RLSoundEffect* RLAudioManager::FireAndReturn(const std::string& name)
		{
			RLSoundEffect *pSE;

			pSE = new RLSoundEffect(name);
			pSE->Initialise();
			pSE->Play();

			return pSE;
		}

		void RLAudioManager::InitialiseOpenAL(void )
		{
			//IF OpenAL is not currently enabled
			if( alIsEnabled(NULL) == AL_FALSE )
			{
				//Enable it
				alEnable(NULL);
			}
			CheckALError();

			//Capture a device (no device specified)
			pALCdevice = alcOpenDevice(NULL);
			CheckALError();
			//if device is valid, create context
			if(pALCdevice)
			{
				pALCcontext = alcCreateContext(pALCdevice, NULL);
				//Set context to current
				alcMakeContextCurrent(pALCcontext);
				CheckALError();
			}
		}

		void RLAudioManager::DestroyOpenAL(void )
		{
			//Disable context
			alcMakeContextCurrent(NULL);
			//Release context(s)
			alcDestroyContext(pALCcontext);
			//Close device
			alcCloseDevice(pALCdevice);

			//Deallocate OpenAL specific memory

			//pALCdevice
			delete(pALCdevice);
			//pALCcontext
			delete(pALCcontext);
		}

		void RLAudioManager::Update(float timeStep)
		{
			//Run through and check on the forgotten and ambient sounds

			//forgottenAudioList
			std::list<RLAudio*>::iterator iter_ForgottenAudio;
			std::list<RLAudio*> deleteThese;

			for(iter_ForgottenAudio = forgottenAudioList.begin();
				iter_ForgottenAudio != forgottenAudioList.end();
				iter_ForgottenAudio++)
			{
				if(!iter_ForgottenAudio._Ptr->_Myval->IsPlaying())
				{
					deleteThese.push_back(iter_ForgottenAudio._Ptr->_Myval);
				}
			}
			//Delete those sources that are finished playing
			iter_ForgottenAudio = deleteThese.begin();
			while(iter_ForgottenAudio != deleteThese.end())
			{
				forgottenAudioList.remove(iter_ForgottenAudio._Ptr->_Myval);
				delete(iter_ForgottenAudio._Ptr->_Myval);
				iter_ForgottenAudio++;
			}
			deleteThese.clear();
		}
	}
}