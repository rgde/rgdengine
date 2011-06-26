#include "stdafx.h"

#include <rgde/core/debug.h>

namespace rgde
{
	namespace debug
	{
		void pring(const char* str) 
		{
			OutputDebugStringA(str);
		}

		void pring(const wchar_t* str)
		{
			OutputDebugStringW(str);
		}
	}

	namespace core
	{
		// use QueryPerformanceFrequency to get the frequency of the counter
		int64 get_ticks_per_second()
		{
			LARGE_INTEGER ticks_per_sec;
			QueryPerformanceFrequency(&ticks_per_sec);
			return ticks_per_sec.QuadPart;
		}				

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
	}
}