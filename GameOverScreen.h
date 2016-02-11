#pragma once

#include "RLScreen.h"
using RustyLib::Framework::ScreenManagement::Screen;

class GameOverScreen : public Screen
{
private:
	int player_score;

public:
	GameOverScreen(void);
	~GameOverScreen(void);

	Error Initialise(void );
	Error Initialise(const int score);
	Error Exit(void );

	void Update(const float time_step);
	void Render(void );
};

