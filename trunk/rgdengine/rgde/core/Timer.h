/// @20.06.2005
/// Related methods associated with timer.
#pragma once

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
		float absolute_time() const;

		/// Returns the current time
		float time() const;

		/// Returns a time that elapsed between elapsed() calls
		float elapsed();
		float elapsed() const;

		/// Indicates that a timer is stopped or paused
		bool is_stoped() const;

	protected:
		bool stopped;

		__int64 BaseTime;
		__int64 LastTime;
		__int64 StopTime;
		__int64 TicksPerSecond;
	};
}