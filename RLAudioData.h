#pragma once

namespace RustyLib
{
	namespace Audio
	{
		class RLAudioData
		{
		private:

			const static std::string PATH_PREFIX;

			std::string name;
			std::string path;

			unsigned int alBufferID;

			float duration;

		public:
			RLAudioData(const std::string& pathStr);
			~RLAudioData(void);

			void BufferData(void );
			const unsigned int GetBufferID(void )const;

			const std::string& Name(void )const;
			const float GetBufferDuration(void )const;
		};
	}
}

