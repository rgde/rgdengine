#pragma once

namespace ui
{
	typedef math::vec2f Position;
	typedef math::vec2f Size;

	struct Rect
	{
		Position postion;
		Size	 size;
	};
}

#include "uiEvents.h"
#include "uiControls.h"