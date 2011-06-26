#include "stdafx.h"

#include <rgde/core/timer.h>

namespace rgde
{
	namespace core
	{
		timer::timer(bool start)
			:	m_is_stoped(true),
				m_is_using_qpf(true),
				m_ticks_per_second(get_ticks_per_second()),
				m_stop_time(0),
				m_last_elapsed_time(0),
				m_base_time(0)
		{
			if(start)
				reset();
		}

		void timer::reset()
		{
			int64 time = m_stop_time ? m_stop_time : get_current_time();
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
			int64 time = m_stop_time ? m_stop_time : get_current_time();

			double app_time = ( time - m_base_time ) / (double)m_ticks_per_second;
			return app_time;
		}

		void timer::get_time_values( double& time, double& absolute_time, float& elapsed_time)
		{
			int64 cur_time = m_stop_time ? m_stop_time : get_current_time();

			elapsed_time = (float)((cur_time - m_last_elapsed_time) / (double)m_ticks_per_second);
			m_last_elapsed_time = cur_time;

			absolute_time = cur_time / (double)m_ticks_per_second;
			time = (cur_time - m_base_time) / (double)m_ticks_per_second; 
		}

		float timer::get_elapsed_time()
		{
			int64 time = m_stop_time ? m_stop_time : get_current_time();
			
			float elapsed_time = (float)((time - m_last_elapsed_time) / (double)m_ticks_per_second);
			m_last_elapsed_time = time;

			return elapsed_time;
		}
	}
}