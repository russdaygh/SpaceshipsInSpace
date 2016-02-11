#include "StdAfx.h"
#include "RLKeyboard.h"
using RustyLib::Input::RLKeyboard;
using RustyLib::Input::RLKeyCode;


//In-class Initialiser
bool RLKeyboard::keys[NUM_KEYS];


RLKeyboard::RLKeyboard(void)
{
}


RLKeyboard::~RLKeyboard(void)
{
}


void RLKeyboard::HandleKeyDown(const char key)
{
	if(key < NUM_KEYS)
	{
		keys[(int)key] = true;
	}
}


void RLKeyboard::HandleKeyUp(const char key)
{
	if(key < NUM_KEYS)
	{
		keys[(int)key] = false;
	}
}


const bool RLKeyboard::IsKeyDown(const RLKeyCode key)
{
	return keys[(int)key];
}