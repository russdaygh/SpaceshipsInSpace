#pragma once

#include "GameObject.h"

#include "RLBoundingSphere.h"
using RustyLib::Physics::BSphere;

class Asteroid : public GameObject
{
private:
	//Image vars
	Vector3 *m_hull;
	int num_verts;
	static Vector3 colour;
	Error HullGenerate(void );
	bool IsLeftTurn(const Vector3& p1, const Vector3& p2, const Vector3& p3);

	//Movement vars
	static float MIN_VELOCITY;
	static float MAX_VELOCITY;
	float rot_velocity;
	static float MIN_ROT_VELOCITY;
	static float MAX_ROT_VELOCITY;
	void SetVelocities(void );

	//Size vars
	float size;
	static float MIN_SIZE;
	static float MAX_SIZE;

	//Collision vars
	BSphere b_sphere;

public:
	Asteroid(void);
	~Asteroid(void);

	static Error ClassInitialise(void );
	Error Initialise(void );
	Error Initialise(float size);
	Error Exit(void );

	void Update(const float time_step);
	void Render(void );

	BSphere& GetBSphere(void );
};

