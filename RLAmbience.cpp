#include "StdAfx.h"
#include "RLAmbience.h"
#include "RLAudioManager.h"

//OpenAL includes
//#include <al\al.h>
#include "AL\al.h"
//#include <al\alut.h>
#include "AL\alut.h"

namespace RustyLib
{
	namespace Audio
	{
		float RLAmbience::volume = 1.0f;

		RLAmbience::RLAmbience(const std::string& name, float interval):
			RLAudio(name),
			playInterval(interval)
		{
		}


		RLAmbience::~RLAmbience(void)
		{
			this->BaseDestructor();
		}


		void RLAmbience::Initialise(void )
		{
			this->BaseInitialise();

			//Set gain to RLAmbience volume
			alSourcef(alSourceID, AL_GAIN, RLAmbience::volume);

			if(playInterval > 0.0f)
			{
				timeLeft = playInterval;
			}
			else
			{
				playInterval = audioMgr->GetBufferDuration(name);
			}
		}


		void RLAmbience::Update(float timeStep)
		{
			timeLeft -= timeStep;

			if(timeLeft <= 0.0f)
			{
				this->Play();//OpenAl play method restarts the source
				timeLeft = playInterval;
			}
		}


		float RLAmbience::Volume(void )
		{
			return volume;
		}


		void RLAmbience::Volume(float vol)
		{
			if(vol >= 0.0f)
				volume = vol;
			else
				volume = 0.0f;
		}
	}
}