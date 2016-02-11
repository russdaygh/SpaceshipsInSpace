#include "StdAfx.h"
#include "RLAudio.h"

//Local includes
#include "RLAudioManager.h"
#include "RLAudioUtilities.h"

//OpenAL includes
//#include <al\al.h>
#include "AL\al.h"
//#include <al\alut.h>
#include "AL\alut.h"

namespace RustyLib
{
	namespace Audio
	{
		//Static definition
		RLAudioManager* RLAudio::audioMgr;

		RLAudio::RLAudio(const std::string& name)
			: name(name)
		{
		}


		RLAudio::~RLAudio(void)
		{
			this->BaseDestructor();
		}


		void RLAudio::BaseInitialise(void )
		{
			//Allocate a source in OpenAL
			alGenSources(1, &alSourceID);

			//Get the BufferID
			alBufferID = audioMgr->GetBufferID(name);

			//Link to buffer
			alSourcei(alSourceID, AL_BUFFER, alBufferID);
			CheckALError();
		}


		void RLAudio::BaseDestructor(void )
		{
			this->Stop();
			alDeleteSources(1, &alSourceID);
		}


		void RLAudio::SetAudioManager(RLAudioManager& rRLAudioMgr)
		{
			RLAudio::audioMgr = &rRLAudioMgr;
		}


		void RLAudio::Play(void )
		{
			alSourcePlay(alSourceID);
		}


		void RLAudio::Pause(void )
		{
			alSourcePause(alSourceID);
		}


		void RLAudio::Stop(void )
		{
			alSourceStop(alSourceID);
		}


		void RLAudio::Rewind(void )
		{
			alSourceRewind(alSourceID);
		}


		const bool RLAudio::IsPlaying(void )const
		{
			ALint playing = AL_STOPPED;
			alGetSourcei(alSourceID, AL_SOURCE_STATE, &playing);
			return (playing == AL_PLAYING);
		}
	}
}