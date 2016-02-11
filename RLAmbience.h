#pragma once
#include "RLAudio.h"

namespace RustyLib
{
	namespace Audio
	{
		class RLAmbience :
			public RLAudio
		{
		private:

			//Volume for Ambience objects
			static float volume;

			//Play interval
			float playInterval;
			//Time left before re playing
			float timeLeft;

		public:
			RLAmbience(const std::string& name, float interval = 0.0f);
			~RLAmbience(void);

			void Initialise(void );

			void Update(float timeStep);

			static float Volume(void );
			static void Volume(const float vol);
		};
	}
}