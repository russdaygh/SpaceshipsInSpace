#pragma once

#include "RLScreen.h"
using RustyLib::Framework::ScreenManagement::Screen;

class SplashScreen : public Screen
{
public:
	SplashScreen(void);
	~SplashScreen(void);

	Error Initialise(void );
	Error Exit(void );

	void Update(const float time_step);
	void Render(void );
};

