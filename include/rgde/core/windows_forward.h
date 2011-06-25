#pragma once

#include <rgde/core/math/forward.h>
#include <windows.h>

namespace rgde
{
	namespace core
	{
		namespace windows
		{
			typedef HWND handle;
			typedef LRESULT result;

			class window;
			typedef boost::shared_ptr<window> window_ptr;
		}
	}
}
