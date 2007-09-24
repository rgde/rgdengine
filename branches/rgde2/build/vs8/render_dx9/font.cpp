#include "stdafx.h"

#include "font.h"

namespace rgde
{
	namespace render
	{
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////

		font::font(device& dev, size_t height, const std::wstring& name, Weight weigh)
			: m_pimpl(new font_impl(dev, height, name, weigh))
		{
		}

		font::~font()
		{
		}

		const device& font::get_device() const 
		{
			return m_pimpl->get_device();
		}

		device& font::get_device() 
		{
			return  m_pimpl->get_device();
		}

		void font::render(const std::wstring& text, const math::rect& rc, unsigned int color, bool draw_shadow, int flags)
		{
			m_pimpl->render(text, rc, color, draw_shadow, flags);
		}

		math::rect font::measure_test( const std::wstring& text, int flags)
		{
			return m_pimpl->measure_test(text, flags);
		}

		font::font_impl& font::get_impl()
		{
			return *m_pimpl;
		}

		const font::font_impl& font::get_impl() const
		{
			return *m_pimpl;
		}
		//////////////////////////////////////////////////////////////////////////
	}
}