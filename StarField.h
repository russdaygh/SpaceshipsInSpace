#pragma once

#include "GameObject.h"

#include "RLVector.h"
using RustyLib::Vector3;

class StarField : public GameObject
{
private:
	Vector3 star_colour;
	int NUM_STARS;
	Vector3* stars;

	void GenerateStars(const Vector3& min, const Vector3& max);

public:
	StarField(void);
	~StarField(void);

	Error Initialise(void );
	Error Initialise(const Vector3& min, const Vector3& max, int num_stars);
	Error Exit(void );

	void Update(const float time_step);
	void Render(void );
};

