#pragma once


namespace RustyLib
{
	namespace Audio
	{
		class RLAudioManager;

		class RLAudio
		{
		protected:
			//Reference to AudioManager for all Audio objects to utilise
			static RLAudioManager *audioMgr;

			//Initialisation method for base attributes
			void BaseInitialise(void );
			//De-Initialisation method for base attributes
			void BaseDestructor(void );

			//Source ID for OpenAL
			unsigned int alSourceID;
			//Name and ID of buffer data
			const std::string name;
			unsigned int alBufferID;

		public:
			//Sets the static AudioManager reference
			static void SetAudioManager(RLAudioManager& rAudioMgr);

			RLAudio(const std::string& name);
			~RLAudio(void);

			void Play(void );
			void Pause(void );
			void Stop(void );
			void Rewind(void );

			const bool IsPlaying(void )const;
		};
	}
}