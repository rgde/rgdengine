#include "stdafx.h"

#include <rgde/core/log.h>
#include <windows.h>

//#if _SECURE_SCL == 1
//int 5;
//#endif

namespace rgde
{
	namespace core
	{
		namespace log
		{
			namespace utils
			{
				void output_debug_string(const std::wstring& msg)
				{
					OutputDebugStringW(msg.c_str());
				}

				void output_debug_string(const std::string& msg)
				{
					OutputDebugStringA(msg.c_str());
				}
			}
		}
	}
}