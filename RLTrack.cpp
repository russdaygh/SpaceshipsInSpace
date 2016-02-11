#include "StdAfx.h"
#include "RLTrack.h"

//OpenAL includes
//#include <al\al.h>
#include "AL\al.h"

namespace RustyLib
{
	namespace Audio
	{
		float RLTrack::volume = 1.0f;

		RLTrack::RLTrack(const std::string& name)
			: RLAudio(name)
		{
		}


		RLTrack::~RLTrack(void)
		{
			this->BaseDestructor();
		}


		void RLTrack::Initialise(void )
		{
			this->BaseInitialise();

			//Set Gain to RLTrack::volume
			alSourcef(alSourceID, AL_GAIN, RLTrack::volume);
			//RLTracks should loop until changed
			alSourcei(alSourceID, AL_LOOPING, AL_TRUE);
		}


		float RLTrack::Volume(void )
		{
			return volume;
		}


		void RLTrack::Volume(float vol)
		{
			if(vol >= 0.0f)
				volume = vol;
			else
				volume = 0.0f;
		}
	}
}