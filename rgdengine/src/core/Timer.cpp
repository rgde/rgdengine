/// @20.06.2005
/// Timer implementation

#include "precompiled.h"

#include "../include/core/Timer.h"
#include <windows.h>

//-----------------------------------------------------------------------------
namespace core
{
	Timer::Timer()
	{
		Stopped = true;

		BaseTime = 0;
		LastTime = 0;
		StopTime = 0;

		TicksPerSecond = 0;

		LARGE_INTEGER ticksPerSecond;
		QueryPerformanceFrequency(&ticksPerSecond);
		TicksPerSecond = ticksPerSecond.QuadPart;
	}

	//-----------------------------------------------------------------------------
	void Timer::start()
	{
		LARGE_INTEGER time;

		if( 0 != StopTime )
			time.QuadPart = StopTime;
		else
			QueryPerformanceCounter(&time);

		if( Stopped )
			BaseTime += time.QuadPart - StopTime;

		StopTime = 0;
		LastTime = time.QuadPart;
		Stopped = false;
	}

	//-----------------------------------------------------------------------------
	void Timer::stop()
	{
		if( Stopped )
		{
			LARGE_INTEGER time;

			if( 0 != StopTime )
				time.QuadPart = StopTime;
			else
				QueryPerformanceCounter(&time);

			StopTime = time.QuadPart;
			LastTime = time.QuadPart;
			Stopped = true;
		}
	}

	//-----------------------------------------------------------------------------
	void Timer::reset()
	{
		LARGE_INTEGER time;

		if( 0 != StopTime )
			time.QuadPart = StopTime;
		else
			QueryPerformanceCounter(&time);

		BaseTime = time.QuadPart;
		LastTime = time.QuadPart;
		StopTime = time.QuadPart;

		Stopped = false;
	}

	//-----------------------------------------------------------------------------
	void Timer::advance()
	{
		StopTime += TicksPerSecond/10;
	}

	//-----------------------------------------------------------------------------
	float Timer::getAbsoluteTime()
	{
		LARGE_INTEGER time;

		if( 0 != StopTime )
			time.QuadPart = StopTime;
		else
			QueryPerformanceCounter(&time);

		// Calculate the absolute time
		return time.QuadPart / (float)TicksPerSecond;
	}

	//-----------------------------------------------------------------------------
	float Timer::getCurrentTime()
	{
		LARGE_INTEGER time;

		if( 0 != StopTime )
			time.QuadPart = StopTime;
		else
			QueryPerformanceCounter(&time);

		// Calculate a current time
		return (time.QuadPart-BaseTime) / (float)TicksPerSecond;
	}

	//-----------------------------------------------------------------------------
	float Timer::getElapsedTime()
	{
		LARGE_INTEGER time;

		if( 0 != StopTime )
			time.QuadPart = StopTime;
		else
			QueryPerformanceCounter(&time);

		// Calculate a elapsed time
		float elapsed = float(time.QuadPart-LastTime) / (float)TicksPerSecond;

		LastTime = time.QuadPart;
	
		return elapsed;
	}

	//-----------------------------------------------------------------------------
	bool Timer::isStoped()
	{
		return Stopped;
	}
}