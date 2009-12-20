#include "stdafx.h"

#include <rgde/render/device.h>
#include <rgde/render/font.h>

#include <rgde/core/math.h>

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

		void font::render(const std::string& text, const math::rect& rc, unsigned int color)
		{
			render(std::wstring(text.begin(), text.end()), rc, color, false);
		}

		void font::render(const std::string& text, const math::rect& rc, unsigned int color, bool draw_shadow)
		{
			render(std::wstring(text.begin(), text.end()), rc, color, draw_shadow);
		}

		void font::render(const std::string& text, const math::rect& rc, unsigned int color, bool draw_shadow, int flags)
		{
			render(std::wstring(text.begin(), text.end()), rc, color, draw_shadow, flags);
		}

		math::rect font::measure_text( const std::string& text, int flags)
		{
			return measure_text(std::wstring(text.begin(), text.end()), flags);
		}

		void font::render(const std::string& text, float x, float y, unsigned int color)
		{
			render(text, math::rect(x, y, 0,0), color, false, 0);
		}

		void font::render(const std::wstring& text, float x, float y, unsigned int color)
		{
			render(text, math::rect(x, y, 0,0), color, false, 0);
		}
	}
}