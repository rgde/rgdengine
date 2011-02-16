#include "stdafx.h"

#include <rgde/core/log.h>
#include <windows.h>

//TODO: move that code to platform lib!
#if DEBUG || _DEBUG
	#define RELEASE_OUT 
#else
	#define RELEASE_OUT return;
#endif

namespace rgde
{
	namespace core
	{
		namespace log
		{
			namespace utils
			{
				void debug_out(const std::wstring& msg)
				{
					RELEASE_OUT;
					OutputDebugStringW(msg.c_str());
				}

				void debug_out(const std::string& msg)
				{
					RELEASE_OUT;
					OutputDebugStringA(msg.c_str());
				}

				void debug_out(const char *format, ...)
				{
					RELEASE_OUT;
					if (!format) return;

					//va_list arglist;
					//int buffing;
					//int retval;
					//va_start(arglist, format);

				}

				void debug_out(const wchar_t *format, ...)
				{
					RELEASE_OUT;
					if (!format) return;

				}
			}
		}
	}
}