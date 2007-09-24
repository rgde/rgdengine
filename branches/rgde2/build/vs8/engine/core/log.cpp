#include "stdafx.h"

#include <engine/core/log.h>
#include <windows.h>

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