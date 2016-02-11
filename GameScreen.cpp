#include "StdAfx.h"
#include "GameScreen.h"

#include "GameOverScreen.h"
#include "LuaUtilities.h"


GameScreen::GameScreen(void)
{
}


GameScreen::~GameScreen(void)
{
}


Error GameScreen::Initialise(void )
{
	//Get config details
	LuaUtils::LoadFile("config/game_screen_config.ini");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	MAX_ASTEROIDS = LuaUtils::GetInt("MAX_ASTEROIDS");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	MAX_ASTEROID_SIZE = LuaUtils::GetInt("MAX_ASTEROID_SIZE");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}
	ASTEROID_SPAWN_INTERVAL = LuaUtils::GetFloat("ASTEROID_SPAWN_INTERVAL");
	if(LuaUtils::GetErrorCode()){return Error::RL_ERROR;}

	LuaUtils::ClearLuaStack();

	play_region.Initialise(
		Vector3(0.0f,0.0f,-127.0f),
		Vector3(800.0f,600.0f,127.0f));
	asteroid_spawn_region.Initialise(
		Vector3(-MAX_ASTEROID_SIZE*2,-MAX_ASTEROID_SIZE*2,-MAX_ASTEROID_SIZE*2),
		Vector3(800.0f+(MAX_ASTEROID_SIZE*2),600.0f+(MAX_ASTEROID_SIZE*2),MAX_ASTEROID_SIZE*2));

	player.Initialise();

	asteroid_spawn_countdown = ASTEROID_SPAWN_INTERVAL;
	asteroid_spawn_side = 0;
}


Error GameScreen::Exit(void )
{
	player.Exit();

	std::list<Asteroid*>::iterator iter = asteroids.begin();
	while(iter != asteroids.end())
	{
		Asteroid* ast = iter._Ptr->_Myval;

		ast->Exit();
		delete ast;

		iter++;
	}
	asteroids.clear();

	return Error::RL_NO_ERROR;
}


void GameScreen::Update(const float time_step)
{
	player.Update(time_step);

	bool game_over = false;

	//Update asteroids checking them against player and lasers
	BSphere player_sphere = player.GetBSphere();
	std::list<Asteroid*>::iterator iter = asteroids.begin();
	while(iter != asteroids.end())
	{
		Asteroid* ast = iter._Ptr->_Myval;

		ast->Update(time_step);

		//Check asteroid is in the play region
		//if not reset it
		if(!play_region.Contains(ast->GetBSphere()))
		{
			//Move the asteroid elsewhere
			RepositionAsteroid(ast);
		}
		//check against the player
		if(player_sphere.Contains(ast->GetBSphere()))
		{
			//player.AddScore(ast->GetBSphere().GetRadius());
			game_over = true;
		}

		iter++;
	}

	//Do some laser-asteroid checking
	player.CheckLaserAsteroidCollisions(&asteroids);

	asteroid_spawn_countdown -= time_step;

	if(asteroid_spawn_countdown < 0 && asteroids.size() < MAX_ASTEROIDS)
	{
		asteroid_spawn_countdown = ASTEROID_SPAWN_INTERVAL;

		SpawnAsteroid();
	}

	if(game_over)
	{
		GameOverScreen *gameover_screen = new GameOverScreen();
		gameover_screen->Initialise(player.GetScore());
		RustyLib::Framework::ScreenManagement::ScreenManager::Instance().ChangeScreen(
					gameover_screen);
	}
}


void GameScreen::Render(void )
{
	player.Render();

	//Update asteroids
	std::list<Asteroid*>::iterator iter = asteroids.begin();
	while(iter != asteroids.end())
	{
		Asteroid* ast = iter._Ptr->_Myval;

		ast->Render();

		iter++;
	}
}


void GameScreen::SpawnAsteroid(void )
{
	//1: Find a position outside of the screen
	srand(timeGetTime());

	float minX=0.0f,minY=0.0f,maxX=0.0f,maxY=0.0f;

	if(asteroid_spawn_side == LEFT)
	{
		//Asteroid X pos must be above ast_spawn_min
		//less than play_region_min
		//and Y can be anywhere between ast_spawn_min and max
		asteroid_spawn_side++;
		minX = asteroid_spawn_region.GetMin().x-MAX_ASTEROID_SIZE;
		maxX = play_region.GetMin().x+1.0f;//Prevents divide by zero
		minY = asteroid_spawn_region.GetMin().y;
		maxY = asteroid_spawn_region.GetMax().y;
	}
	else if(asteroid_spawn_side == RIGHT)
	{
		//Asteroid X pos must be above play_region_max
		//less than ast_spawn_min
		//and Y can be anywhere between ast_spawn_min and max
		asteroid_spawn_side++;
		minX = play_region.GetMax().x+MAX_ASTEROID_SIZE;
		maxX = asteroid_spawn_region.GetMin().x;
		minY = asteroid_spawn_region.GetMin().y;
		maxY = asteroid_spawn_region.GetMax().y;
	}
	else if(asteroid_spawn_side == TOP)
	{
		//Asteroid Y pos must be less than play_region_min
		//more than ast_spawn_min
		//and X can be anywhere between ast_spawn_min and max
		asteroid_spawn_side++;
		minX = asteroid_spawn_region.GetMin().x;
		maxX = asteroid_spawn_region.GetMax().x;
		minY = asteroid_spawn_region.GetMin().y;
		maxY = play_region.GetMin().y+MAX_ASTEROID_SIZE;
	}
	else//asteroid_spawn_side == BOTTOM
	{
		//Asteroid Y pos must be above play_region_max
		//less than ast_spawn_max
		//and X can be anywhere between ast_spawn_min and max
		asteroid_spawn_side = LEFT;
		minX = asteroid_spawn_region.GetMin().x;
		maxX = asteroid_spawn_region.GetMax().x;
		minY = play_region.GetMax().y+MAX_ASTEROID_SIZE;
		maxY = asteroid_spawn_region.GetMax().y;
	}
	//Now rand up a position within those bounds
	Asteroid *ast = new Asteroid();
	//Initialise gives it the max size by default
	//it also sets up a velocity
	ast->Initialise();
	//Set the pos
	ast->SetPosition(Vector3(
		(rand() % (int)maxX) + minX,
		(rand() % (int)maxY) + minY,
		0.0f));

	//3: place it on the list
	asteroids.push_back(ast);
}


void GameScreen::RepositionAsteroid(Asteroid *ast)
{
	//Player needs moving
	Vector3 position = ast->GetPosition();
	float bsphere_radius = ast->GetBSphere().GetRadius();
	Vector3 min = play_region.GetMin();
	Vector3 max = play_region.GetMax();

	if(position.x < min.x - bsphere_radius)
	{
		position.x = max.x + (bsphere_radius-1.0f);
	}
	if(position.x > max.x + bsphere_radius)
	{
		position.x = min.x - (bsphere_radius+1.0f);
	}
	if(position.y < min.y - bsphere_radius)
	{
		position.y = max.y + (bsphere_radius-1.0f);
	}
	if(position.y > max.y + bsphere_radius)
	{
		position.y = min.y - (bsphere_radius+1.0f);
	}
	ast->SetPosition(position);
}