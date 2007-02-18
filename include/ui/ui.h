#pragma once

namespace ui
{
	typedef gmtl::Vec2f Position;
	typedef gmtl::Vec2f Size;

	struct Rect
	{
		Position postion;
		Size	 size;
	};
}

#include "uiEvents.h"
#include "uiControls.h"