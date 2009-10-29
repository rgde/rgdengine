/// @20.06.2005
/// Related methods associated with timer.
#pragma once

#include <rgde/base/singelton.h>

//-----------------------------------------------------------------------------
namespace core
{
	class timer
	{
	public:
		timer();
	
		/// Starts timer
		void start();

		/// Stop/pause timer
		void stop();

		/// Resets all time values to zero
		void reset();

		/// Advance a timer by 0.1 seconds
		void advance();

		/// Returns the absolute system time
		float get_absolute_time();

		/// Returns the current time
		float get_current_time();

		/// Returns a time that elapsed between get_elapsed() calls
		float get_elapsed();

		/// Indicates that a timer is stopped or paused
		bool is_stoped();

	protected:
		bool Stopped;

		__int64 BaseTime;
		__int64 LastTime;
		__int64 StopTime;
		__int64 TicksPerSecond;
	};

	typedef base::singelton<timer> TheTimer;
}