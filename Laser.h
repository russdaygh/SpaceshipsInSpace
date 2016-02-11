#pragma once

#include "GameObject.h"

class Laser : public GameObject
{
private:
	float size;

public:
	Laser(void);
	~Laser(void);

	Error Initialise(void );
	Error Initialise(Vector3& pos, float rot, float radius, float velocity);
	Error Exit(void );

	void Update(const float time_step);
	void Render(void );
};

