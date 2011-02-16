#pragma once

#include <string>

namespace rgde
{
	namespace core
	{
		namespace log
		{
			namespace utils
			{
				void debug_out(const std::wstring& msg);
				void debug_out(const std::string& msg);
				void debug_out(const char *format, ...);
				void debug_out(const wchar_t *format, ...);
			}
		}
	}
}