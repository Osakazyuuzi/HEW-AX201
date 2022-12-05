#include "Timer.h"

//--- •Ï”éŒ¾
Frame Timer::m_fps;
Time Timer::m_deltaTime;
DWORD Timer::m_preExec;

void Timer::Init(Frame fps)
{
	SetFPS(fps);
	timeBeginPeriod(1);
	m_preExec = timeGetTime();
}

void Timer::Uninit()
{
	timeEndPeriod(1);
}

bool Timer::Update()
{
	DWORD time = timeGetTime();
	DWORD diff = time - m_preExec;
	if (diff >= 1000 / 60)
	{
		m_deltaTime = timeGetTime();
		m_preExec = time;
		return true;
	}
	return false;
}

Time Timer::ConvertFrameToTime(Frame frame)
{
	return static_cast<float>(frame) / m_fps;
}

Frame Timer::ConvertTimeToFrame(Time time)
{
	return static_cast<Frame>(time * m_fps);
}