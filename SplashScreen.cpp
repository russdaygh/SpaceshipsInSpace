#include "StdAfx.h"
#include "SplashScreen.h"

#include "MenuScreen.h"

#include "RLScreenManager.h"
using RustyLib::Framework::ScreenManagement::ScreenManager;


SplashScreen::SplashScreen(void)
{
}


SplashScreen::~SplashScreen(void)
{
}


Error SplashScreen::Initialise(void )
{
	return Error::RL_NO_ERROR;
}


Error SplashScreen::Exit(void )
{
	return Error::RL_NO_ERROR;
}


void SplashScreen::Update(const float time_step)
{
	MenuScreen *menu_screen = new MenuScreen();
	ScreenManager::Instance().ChangeScreen(menu_screen);
}


void SplashScreen::Render(void )
{
}