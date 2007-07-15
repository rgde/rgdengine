#include "stdafx.h"

#include "device.h"

#ifdef _DEBUG
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")
#endif


namespace rgde
{
	namespace render
	{
		device::device(core::windows::handle hwnd, bool windowed)
			: m_pimpl(new device_impl(hwnd, windowed))
		{
		}

		device::~device()
		{
		}

		bool device::frame_begin()
		{
			return m_pimpl->frame_begin();
		}

		bool device::frame_end()
		{
			return m_pimpl->frame_end();
		}

		bool device::present()
		{
			return m_pimpl->present();
		}

		void device::clear(unsigned int color, float depth)
		{
			m_pimpl->clear(color, depth);
		}

		device::device_impl& device::get_impl() 
		{
			return *m_pimpl;
		}

		const device::device_impl& device::get_impl() const
		{
			return *m_pimpl;
		}
	}
}