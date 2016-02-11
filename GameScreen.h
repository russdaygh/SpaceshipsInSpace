#pragma once

#include "RLScreen.h"
using RustyLib::Framework::ScreenManagement::Screen;

#include "RLAABoundingBox.h"
using RustyLib::Physics::AABBox;

#include "Asteroid.h"
#include "Player.h"

class GameScreen : public Screen
{
private:
	//Define the play region
	AABBox play_region;
	AABBox asteroid_spawn_region;

	Player player;

	//Asteroid vars
	std::list<Asteroid*> asteroids;
	int asteroid_spawn_side;//Iterates between 1 and 4 for each side
	int MAX_ASTEROIDS;
	float MAX_ASTEROID_SIZE;
	float asteroid_spawn_countdown;
	enum{LEFT,TOP,RIGHT,BOTTOM};
	float ASTEROID_SPAWN_INTERVAL;
	void SpawnAsteroid(void );
	void RepositionAsteroid(Asteroid *ast);

public:
	GameScreen(void);
	~GameScreen(void);

	Error Initialise(void );
	Error Exit(void );

	void Update(const float time_step);
	void Render(void );
};

