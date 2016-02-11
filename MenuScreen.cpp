#include "StdAfx.h"
#include "MenuScreen.h"

#include "GameScreen.h"

#include "RLKeyboard.h"
using RustyLib::Input::Keyboard;
#include "RLRenderer.h"
using RustyLib::Graphics::Renderer;
#include "RLScreenManager.h"
using RustyLib::Framework::ScreenManagement::ScreenManager;


MenuScreen::MenuScreen(void)
{
}


MenuScreen::~MenuScreen(void)
{
}


Error MenuScreen::Initialise(void )
{
	return Error::RL_NO_ERROR;
}


Error MenuScreen::Exit(void )
{
	return Error::RL_NO_ERROR;
}


void MenuScreen::Update(const float time_step)
{
	if(Keyboard::IsKeyDown(RustyLib::Input::KeyCode::SpaceBar))
	{
		GameScreen *game_screen = new GameScreen();
		ScreenManager::Instance().ChangeScreen(game_screen);
	}
}


void MenuScreen::Render(void )
{
	Renderer *r = &Renderer::Instance();

	r->SetLineColour(0.0f,255.0f,0.0f);

	std::string text1 = "Randomly Generated";
	r->RenderText(text1, 300, 200);

	std::string text2 = "Software Rendered Asteroids on a Plane!";
	r->RenderText(text2, 210, 250);

	std::string text3 = "Press SPACE to begin.";
	r->RenderText(text3, 290, 310);

	std::string text4 = "Press ESC to quit.";
	r->RenderText(text4, 315, 340);
}