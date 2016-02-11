#include "StdAfx.h"
#include "RLScreenManager.h"
using RustyLib::Framework::ScreenManagement::RLScreenManager;

#include "RLScreen.h"
using RustyLib::Framework::ScreenManagement::RLScreen;


RLScreenManager::RLScreenManager(void)
	:m_currentScreen(0)
{
}


RLScreenManager::~RLScreenManager(void)
{
	if(m_currentScreen)
	{
		delete m_currentScreen;
		m_currentScreen = 0;
	}
}


void RLScreenManager::operator=(const RLScreenManager& other)
{
	bIsInitialised = other.bIsInitialised;
	m_currentScreen = other.m_currentScreen;
}


void RLScreenManager::Initialise(void )
{
}


RLScreenManager& RLScreenManager::Instance(void )
{
	static RLScreenManager m_ScreenManagerInstance;
	if(m_ScreenManagerInstance.bIsInitialised)
	{
		return m_ScreenManagerInstance;
	}
	else
	{
		m_ScreenManagerInstance.Initialise();
		return m_ScreenManagerInstance;
	}
}


void RLScreenManager::ChangeScreen(RLScreen *new_screen)
{
	if(new_screen)
	{
		if(m_currentScreen)
		{
			m_currentScreen->Exit();
			delete m_currentScreen;
		}
	
		m_currentScreen = new_screen;
		m_currentScreen->Initialise();
	}
}


RLScreen const* RLScreenManager::GetCurrentScreen(void )
{
	if(m_currentScreen)
	{
		return m_currentScreen;
	}
	else
	{
		return 0;
	}
}


void RLScreenManager::Update(const float time_step)
{
	if(m_currentScreen)
	{
		m_currentScreen->Update(time_step);
	}
}


void RLScreenManager::Render(void )
{
	if(m_currentScreen)
	{
		m_currentScreen->Render();
	}
}