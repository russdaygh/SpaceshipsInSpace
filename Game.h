#pragma once

#include "RustyLib.h"
using RustyLib::Error;
using RustyLib::Vector3;

class StarField;

class Game
{
private:
	float target_delta;

	StarField *star_field;

public:
	Game(void );
	~Game(void );

	Error Initialise(void );
	Error Exit(void );

	void Update(float time_step);
	void Render(void );
};

