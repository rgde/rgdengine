#include "stdafx.h"

#include <rgde/core/log.h>
#include <windows.h>

//TODO: move that code to platform lib!
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