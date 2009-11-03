#include "precompiled.h"

#include <rgde/core/timer.h>
#include <windows.h>

//-----------------------------------------------------------------------------
namespace core
{
	timer::timer()
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
	void timer::start()
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
	void timer::stop()
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
	void timer::reset()
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
	void timer::advance()
	{
		StopTime += TicksPerSecond/10;
	}

	//-----------------------------------------------------------------------------
	float timer::get_absolute_time() const
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
	float timer::get_current_time() const
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
	float timer::get_elapsed() const
	{
		LARGE_INTEGER time;

		if( 0 != StopTime )
			time.QuadPart = StopTime;
		else
			QueryPerformanceCounter(&time);

		// Calculate a elapsed time
		float elapsed = float(time.QuadPart-LastTime) / (float)TicksPerSecond;

		return elapsed;
	}
	//-----------------------------------------------------------------------------
	float timer::get_elapsed()
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
	bool timer::is_stoped() const
	{
		return Stopped;
	}
}