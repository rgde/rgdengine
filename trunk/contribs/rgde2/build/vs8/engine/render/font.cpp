#include "stdafx.h"

#include <engine/render/device.h>
#include <engine/render/font.h>

#include <engine/core/math.h>

#include <d3dx9.h>


namespace rgde
{
	namespace render
	{
		font::font(device& dev, size_t height, const std::wstring& name, Weight weigh)
			: m_device(dev), m_font(NULL)
		{
			if (S_OK != D3DXCreateFont(m_device.get_dx_device(), -height, 0, weigh, 1, FALSE, 
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
				5, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &m_font))
			{
				throw std::bad_exception("CFontImpl():Can't create device font object!");
			}
		}	

		font::~font()
		{
			safe_release(m_font);
		}

		void font::render(const std::wstring& text, const math::rect& rc, unsigned int color)
		{
			render(text, rc, color, false, Top | Left | WordBreak);
		}

		void font::render(const std::wstring& text, const math::rect& rc, unsigned int color, bool draw_shadow)
		{
			render(text, rc, color, draw_shadow, Top | Left | WordBreak);
		}

		void do_render(ID3DXFont* fx_font, const std::wstring& text, RECT textLocation, unsigned int color, int flags)
		{
			fx_font->DrawText(NULL, text.c_str(), -1, &textLocation, flags, color);
		}

		void font::render(const std::wstring& text, const math::rect& rc, unsigned int color, bool draw_shadow, int flags)
		{
			unsigned int nShadowDistance	= 2;
			unsigned int nShadowColor = 0xFF000000; //Black

			if (draw_shadow)
			{
				RECT textShadowLocation;
				textShadowLocation.left = (LONG)((rc.position[0] + nShadowDistance));
				textShadowLocation.top = (LONG)((rc.position[1] + nShadowDistance));
				textShadowLocation.right = textShadowLocation.left + (LONG)(rc.size[0]);
				textShadowLocation.bottom = textShadowLocation.top + (LONG)(rc.size[1]);

				do_render(m_font, text, textShadowLocation, nShadowColor, flags);
			}

			RECT textLocation;
			textLocation.left = (LONG)(rc.position[0]);
			textLocation.top = (LONG)(rc.position[1]);
			textLocation.right = textLocation.left + (LONG)(rc.size[0]);
			textLocation.bottom = textLocation.top + (LONG)(rc.size[1]);

			do_render(m_font, text, textLocation, color, flags);
		}

		math::rect font::measure_test( const std::wstring& text, int flags)
		{
			RECT rc	= {0, 0, 0, 200};
			m_font->DrawText(NULL, text.c_str(), -1, &rc, DT_CALCRECT | flags, 0);

			math::rect r((float)rc.left, (float)rc.top, (float)rc.right - rc.left, (float)rc.bottom - rc.top);
			return r;
		}
	}
}