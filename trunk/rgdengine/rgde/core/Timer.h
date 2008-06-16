/// @20.06.2005
/// Related methods associated with timer.
#pragma once

#include <rgde/base/singelton.h>

//-----------------------------------------------------------------------------
namespace core
{
	class Timer
	{
	public:
		Timer();
	
		/// Starts timer
		void start();

		/// Stop/pause timer
		void stop();

		/// Resets all time values to zero
		void reset();

		/// Advance a timer by 0.1 seconds
		void advance();

		/// Returns the absolute system time
		float getAbsoluteTime();

		/// Returns the current time
		float getCurrentTime();

		/// Returns a time that elapsed between getElapsedTime() calls
		float getElapsedTime();

		/// Indicates that a timer is stopped or paused
		bool isStoped();

	protected:
		bool Stopped;

		__int64 BaseTime;
		__int64 LastTime;
		__int64 StopTime;
		__int64 TicksPerSecond;
	};

	typedef base::TSingelton<Timer> TheTimer;
}