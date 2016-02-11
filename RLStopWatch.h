#pragma once

namespace RustyLib
{
	namespace Framework
	{
		class RLStopWatch
		{
		private:
			double m_StartTime;
			double m_PauseTime;
			bool   m_IsPaused;

		public:
			RLStopWatch(void );
			~RLStopWatch(void );

			void operator=(const RLStopWatch&);

			void Pause(void );
			void UnPause(void );
			double GetTime(void );
			void Reset(void );
		};

		typedef RLStopWatch StopWatch;
	}
}