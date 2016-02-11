#include "StdAfx.h"
#include "GameOverScreen.h"


#include "RLKeyboard.h"
using RustyLib::Input::Keyboard;
#include "RLRenderer.h"
using RustyLib::Graphics::Renderer;
#include "RLScreenManager.h"
using RustyLib::Framework::ScreenManagement::ScreenManager;


#include "GameScreen.h"


GameOverScreen::GameOverScreen(void)
{
}


GameOverScreen::~GameOverScreen(void)
{
}


Error GameOverScreen::Initialise(void )
{
	return Error::RL_NO_ERROR;
}


Error GameOverScreen::Initialise(const int score)
{
	player_score = score;

	return Error::RL_NO_ERROR;
}


Error GameOverScreen::Exit(void )
{
	return Error::RL_NO_ERROR;
}


void GameOverScreen::Update(const float time_step)
{
	if(Keyboard::IsKeyDown(RustyLib::Input::KeyCode::SpaceBar))
	{
		//Restart again
		GameScreen *game_screen = new GameScreen();
		ScreenManager::Instance().ChangeScreen(game_screen);
	}
}


void GameOverScreen::Render(void )
{
	Renderer *r = &Renderer::Instance();

	r->SetLineColour(0.0f,255.0f,0.0f);

	std::string text = "You Lost Fool!";
	r->RenderText(text, 330, 250);

	std::ostringstream sstr;
	sstr << "Score: " << player_score;	
	r->RenderText(sstr.str(), 355, 280);

	std::string text2 = "Press SPACE to restart.";
	r->RenderText(text2, 290, 310);

	std::string text3 = "Press ESC to quit.";
	r->RenderText(text3, 320, 340);
}