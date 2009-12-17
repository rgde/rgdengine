#pragma once

#include "device.h"

namespace rgde
{
	namespace render
	{
		class font::font_impl
		{
		public:
			font_impl(device& dev, size_t height, const std::wstring& name, weight w)
				: m_device(dev), m_font(NULL)
			{
				if (S_OK != D3DXCreateFont(m_device.get_impl().get_dx_device(), -(int)height, 0, w, 1, FALSE, 
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
					ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, name.c_str(), &m_font))
				{
					throw std::bad_exception("CFontImpl():Can't create device font object!");
				}
			}	

			~font_impl()
			{
				safe_release(m_font);
			}

			void do_render(ID3DXFont* dx_font, const std::wstring& text, RECT textLocation, unsigned int color, int flags)
			{
				dx_font->DrawText(NULL, text.c_str(), -1, &textLocation, flags, color);
			}

			math::rect measure_text( const std::wstring& text, int flags = 0)
			{
				RECT rc	= {0, 0, 0, 200};
				if (m_font)
					m_font->DrawText(NULL, text.c_str(), -1, &rc, DT_CALCRECT | flags, 0);

				math::rect r((float)rc.left, (float)rc.top, (float)rc.right - rc.left, (float)rc.bottom - rc.top);
				return r;
			}

			void render(const std::wstring& text, const math::rect& rc, unsigned int color, bool draw_shadow, int flags)
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

			const ID3DXFont* get_dx_font() {return m_font;}

		private:
			device& m_device;
			ID3DXFont* m_font;
		};
	}
}