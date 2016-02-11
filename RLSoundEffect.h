#pragma once
#include "RLAudio.h"

namespace RustyLib
{
	namespace Audio
	{
		class RLSoundEffect :
			public RLAudio
		{
		private:

			//Volume for SoundEffect objects
			static float volume;

		public:
			RLSoundEffect(const std::string& name);
			~RLSoundEffect(void);

			void Initialise(void );

			static float Volume(void );
			static void Volume(const float vol);
		};

		typedef RLSoundEffect SoundEffect;
	}
}