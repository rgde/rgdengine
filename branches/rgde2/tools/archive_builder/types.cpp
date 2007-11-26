#include "stdafx.h"

#include "utility.h"

namespace rgde
{
	namespace filesystem
	{
		std::wstring file_info::get_time_string() const
		{
			enum {BUFFER_SIZE = 128};

			wchar_t wtimebuf[128];
			_wctime_s(wtimebuf, BUFFER_SIZE, &last_write_time);

			return std::wstring(wtimebuf);
		}
	}
}