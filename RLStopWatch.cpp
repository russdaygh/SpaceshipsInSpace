#include "StdAfx.h"
#include "RLStopWatch.h"
using RustyLib::Framework::RLStopWatch;


RLStopWatch::RLStopWatch(void )
	:m_IsPaused(false)
{
	Reset();
}


RLStopWatch::~RLStopWatch(void )
{
}


void RLStopWatch::operator=(const RLStopWatch& other)
{
	m_StartTime = other.m_StartTime;
	m_PauseTime = other.m_PauseTime;
	m_IsPaused  = other.m_IsPaused;
}


void RLStopWatch::Pause(void )
{
	if(m_IsPaused)
	{
		return;
	}
	else
	{
		m_IsPaused = true;
		m_PauseTime = (double)timeGetTime();
	}
}


void RLStopWatch::UnPause(void )
{
	if(!m_IsPaused)
	{
		return;
	}
	else
	{
		m_IsPaused = false;
		m_StartTime = ((double)timeGetTime()) - m_PauseTime;
	}
}


double RLStopWatch::GetTime(void )
{
	if(m_IsPaused)
	{
		return m_PauseTime - m_StartTime;
	}
	else
	{
		return ((double)timeGetTime()) - m_StartTime;
	}
}


void RLStopWatch::Reset(void )
{
	m_StartTime = (double)timeGetTime();
}