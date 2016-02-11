#pragma once
#include "RLAudio.h"

namespace RustyLib
{
	namespace Audio
	{
		class RLTrack :
			public RLAudio
		{
		private:

			//Volume for Track objects
			static float volume;

		public:
			RLTrack(const std::string& name);
			~RLTrack(void);

			void Initialise(void );

			static float Volume(void );
			static void Volume(const float vol);
		};
	}
}