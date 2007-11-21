#include "stdafx.h"

#include "font.h"

namespace rgde
{
	namespace render
	{
		//////////////////////////////////////////////////////////////////////////
		font::font(device& dev, size_t height, const std::wstring& name, weight w)
			: m_pimpl(new font_impl(dev, height, name, w)), 
			device_object(dev, true)
		{
		}

		font::~font()
		{
		}

		void font::render(const std::wstring& text, const math::rect& rc, unsigned int color, bool draw_shadow, int flags)
		{
			m_pimpl->render(text, rc, color, draw_shadow, flags);
		}

		math::rect font::measure_test( const std::wstring& text, int flags)
		{
			return m_pimpl->measure_test(text, flags);
		}
		//////////////////////////////////////////////////////////////////////////
	}
}