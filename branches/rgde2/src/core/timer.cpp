#include "stdafx.h"

#include <rgde/core/timer.h>
#include <windows.h>

namespace rgde
{
	namespace core
	{
		namespace utils
		{
			namespace
			{
				int64 get_current_time(const int64& stop_time = 0)
				{					
					if(stop_time != 0)
						return stop_time;
					else
					{
						LARGE_INTEGER time;
						QueryPerformanceCounter(&time);
						return time.QuadPart;
					}					
				}

				void limit_thread_affinity_tocurrent_proc()
				{
					HANDLE hCurrentProcess = GetCurrentProcess();

					// Get the processor affinity mask for this process
					DWORD_PTR dwProcessAffinityMask = 0;
					DWORD_PTR dwSystemAffinityMask = 0;

					if( GetProcessAffinityMask( hCurrentProcess, &dwProcessAffinityMask, 
						&dwSystemAffinityMask ) != 0 && dwProcessAffinityMask )
					{
						// Find the lowest processor that our process is allows to run against
						DWORD_PTR dwAffinityMask = ( dwProcessAffinityMask & ((~dwProcessAffinityMask) + 1 ) );

						// Set this as the processor that our thread must always run against
						// This must be a subset of the process affinity mask
						HANDLE hCurrentThread = GetCurrentThread();
						if( INVALID_HANDLE_VALUE != hCurrentThread )
						{
							SetThreadAffinityMask( hCurrentThread, dwAffinityMask );
							CloseHandle( hCurrentThread );
						}
					}

					CloseHandle( hCurrentProcess );
				}
			}

			timer::timer(bool start)
				:	m_is_stoped(true),
					m_is_using_qpf(true),
					m_ticks_per_second(0),
					m_stop_time(0),
					m_last_elapsed_time(0),
					m_base_time(0)
			{
				limit_thread_affinity_tocurrent_proc();

				// use queryperformancefrequency to get the frequency of the counter
				LARGE_INTEGER ticks_per_sec;
				QueryPerformanceFrequency(&ticks_per_sec);
				m_ticks_per_second = ticks_per_sec.QuadPart;

				if(start)	
					reset();
			}

			void timer::reset()
			{
				int64 time = get_current_time(m_stop_time);
				m_base_time = time;
				m_last_elapsed_time = time;
				m_stop_time = 0;
				m_is_stoped = false;
			}

			void timer::start()
			{
				int64 time = get_current_time();

				if(m_is_stoped)
					m_base_time += time - m_stop_time;

				m_stop_time = 0;
				m_last_elapsed_time = time;
				m_is_stoped = false;
			}

			void timer::stop()
			{
				if(!m_is_stoped)
				{
					int64 time = get_current_time();
					m_stop_time = time;
					m_last_elapsed_time = time;
					m_is_stoped = true;
				}
			}

			double timer::get_absolute_time()
			{
				int64 time = get_current_time();

				double out_time = time / (double)m_ticks_per_second;
				return out_time;
			}

			double timer::get_time()
			{
				int64 time = get_current_time(m_stop_time);

				double app_time = ( time - m_base_time ) / (double)m_ticks_per_second;
				return app_time;
			}

			void timer::get_time_values( double& time, double& absolute_time, float& elapsed_time)
			{
				int64 cur_time = get_current_time(m_stop_time);

				elapsed_time = (float)((cur_time - m_last_elapsed_time) / (double)m_ticks_per_second);
				m_last_elapsed_time = cur_time;

				absolute_time = cur_time / (double)m_ticks_per_second;
				time = (cur_time - m_base_time) / (double)m_ticks_per_second; 
			}


			float timer::get_elapsed_time()
			{
				int64 time = get_current_time(m_stop_time);
				
				float elapsed_time = (float)((time - m_last_elapsed_time) / (double)m_ticks_per_second);
				m_last_elapsed_time = time;

				return elapsed_time;
			}


			bool timer::is_stopped() const
			{
				return m_is_stoped;
			}
		}
	}
}