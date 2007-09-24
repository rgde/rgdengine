#include "stdafx.h"

#include <engine/render/device.h>
#include <engine/render/font.h>

#include <engine/core/math.h>

namespace rgde
{
	namespace render
	{
		void font::render(const std::wstring& text, const math::rect& rc, unsigned int color)
		{
			render(text, rc, color, false, Top | Left | WordBreak);
		}

		void font::render(const std::wstring& text, const math::rect& rc, unsigned int color, bool draw_shadow)
		{
			render(text, rc, color, draw_shadow, Top | Left | WordBreak);
		}
	}
}