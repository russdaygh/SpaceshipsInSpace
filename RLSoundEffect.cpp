#include "StdAfx.h"
#include "RLSoundEffect.h"

//OpenAL includes
//#include <al\al.h>
#include "AL\al.h"

namespace RustyLib
{
	namespace Audio
	{
		float RLSoundEffect::volume = 1.0f;

		RLSoundEffect::RLSoundEffect(const std::string& name)
			: RLAudio(name)
		{
		}


		RLSoundEffect::~RLSoundEffect(void)
		{
			this->BaseDestructor();
		}


		void RLSoundEffect::Initialise(void )
		{
			this->BaseInitialise();

			//Set gain to RLSoundEffect::volume
			alSourcef(alSourceID, AL_GAIN, RLSoundEffect::volume);
		}


		float RLSoundEffect::Volume(void )
		{
			return volume;
		}


		void RLSoundEffect::Volume(float vol)
		{
			if(vol >= 0.0f)
				volume = vol;
			else
				volume = 0.0f;
		}
	}
}