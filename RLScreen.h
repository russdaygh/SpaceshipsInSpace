#pragma once

#include "RLError.h"
using RustyLib::Error;

namespace RustyLib
{
	namespace Framework
	{
		namespace ScreenManagement
		{
			class RLScreen
			{
			public:
				RLScreen(void);
				virtual ~RLScreen(void);

				virtual Error Initialise(void ) = 0;
				virtual Error Exit(void ) = 0;

				virtual void Update(const float time_step) = 0;
				virtual void Render(void ) = 0;
			};

			typedef RLScreen Screen;
		}
	}
}

