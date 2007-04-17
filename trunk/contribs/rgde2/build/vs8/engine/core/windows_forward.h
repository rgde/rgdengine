#pragma once

#include <engine/core/math_forward.h>
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

			typedef math::vec2i point;
			typedef math::vec2i size;
		}
	}
}
