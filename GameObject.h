#pragma once

#include "RLError.h"
using RustyLib::Error;
#include "RLVector.h"
using RustyLib::Vector3;

class GameObject
{
protected:
	Vector3 position;
	Vector3 velocity;
	float   rotation;

public:
	GameObject(void);
	~GameObject(void);

	virtual Error Initialise(void ) = 0;
	virtual Error Exit(void ) = 0;

	virtual void Update(const float time_step);
	virtual void Render(void ) = 0;

	Vector3& GetPosition(void );
	void SetPosition(Vector3& pos);

	Vector3& GetVelocity(void );
	void SetVelocity(Vector3& vel);

	float GetRotation(void );
	void SetRotation(float rot);
};

