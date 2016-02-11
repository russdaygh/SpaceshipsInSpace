#pragma once

#include "RLScreen.h"
using RustyLib::Framework::ScreenManagement::Screen;

class MenuScreen : public Screen
{
public:
	MenuScreen(void);
	~MenuScreen(void);

	Error Initialise(void );
	Error Exit(void );

	void Update(const float time_step);
	void Render(void );
};

