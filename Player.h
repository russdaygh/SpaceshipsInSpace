#pragma once

#include "GameObject.h"

#include "RLBoundingSphere.h"
using RustyLib::Physics::BSphere;
#include "RLAABoundingBox.h"
using RustyLib::Physics::AABBox;

class Asteroid;
class Laser;

class Player : public GameObject
{
private:
	//Visual attributes
	Vector3 verts[4];
	Vector3 colour;
	float size;

	//Movement attributes
	Vector3 acceleration;
	float thrust_accel;
	float thrust;
	float MAX_THRUST;
	float rot_accel;
	float friction;
	float MAX_VELOCITY;

	//Movement keys
	char left_key;
	char right_key;
	char thrust_key;
	char fire_key;

	//Laser vars
	std::list<Laser*> lasers;
	void FireLaser(void );
	float LASER_RADIUS;
	float LASER_VELOCITY;
	float curr_laser_time;
	float LASER_RECHARGE_TIME;

	//Collision vars
	AABBox play_region;
	BSphere b_sphere;

	//score attributes
	int score;

public:
	Player(void);
	~Player(void);

	Error Initialise(void );
	Error Exit(void );

	void Update(const float time_step);
	void Render(void );

	//Collision Methods
	BSphere& GetBSphere(void );
	void CheckLaserAsteroidCollisions(std::list<Asteroid*>* asteroids);

	void AddScore(int points);
	const int GetScore(void )const;
};

