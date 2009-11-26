#include "stdafx.h"

#include <engine/render/device.h>
#include <engine/render/font.h>

#include <engine/core/math.h>

namespace rgde
{
	namespace render
	{
		font_ptr font::create(device& dev, size_t height, const std::wstring& name, weight w)
		{
			return font_ptr(
				new font(dev, height, name, w)
				);
		}

		void font::render(const std::wstring& text, const math::rect& rc, unsigned int color)
		{
			render(text, rc, color, false, top | left | word_break);
		}

		void font::render(const std::wstring& text, const math::rect& rc, unsigned int color, bool draw_shadow)
		{
			render(text, rc, color, draw_shadow, top | left | word_break);
		}
	}
}