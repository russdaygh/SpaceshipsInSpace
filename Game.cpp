#include "StdAfx.h"
#include "Game.h"

using RustyLib::Input::Keyboard;
using RustyLib::Framework::ScreenManagement::ScreenManager;
using RustyLib::Framework::ScreenManagement::Screen;

#include "Asteroid.h"
#include "SplashScreen.h"
#include "StarField.h"


Game::Game(void)
	:star_field(0)
{
}


Game::~Game(void)
{
}


Error Game::Initialise(void )
{
	//Get asteroids to read in their stuff
	if(Asteroid::ClassInitialise())
	{
		return Error::RL_ERROR;
	}

	//Set up the various items needed
	//star_field = new StarField();
	//star_field->Initialise(Vector3::VectorZero(), Vector3(800.0f,600.0f,1.0f), 500);

	SplashScreen *start_screen = new SplashScreen();
	ScreenManager::Instance().ChangeScreen(start_screen);

	return Error::RL_NO_ERROR;
}


Error Game::Exit(void )
{
	//star_field->Exit();
	delete star_field;
	return Error::RL_NO_ERROR;
}


void Game::Update(float time_step)
{
	if(Keyboard::IsKeyDown(RustyLib::Input::KeyCode::Escape))
	{
		//Figure out how to quit program
		exit(0);
	}
	else
	{
		ScreenManager::Instance().Update(time_step);
		//star_field->Update(time_step);
	}
}


void Game::Render(void )
{
	//Render starfield first to make sure it is in the background
	//star_field->Render();
	ScreenManager::Instance().Render();
}
