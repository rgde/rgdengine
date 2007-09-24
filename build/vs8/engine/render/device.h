#pragma once

#include <engine/core/windows_forward.h>
#include <engine/render/render_forward.h>

#include <boost/noncopyable.hpp>
//struct IDirect3D9;
//struct IDirect3DDevice9;

namespace rgde
{
	namespace render
	{
		class device : boost::noncopyable
		{
			class device_impl;
			typedef boost::scoped_ptr<device_impl> pimpl;
		public:
			explicit device(core::windows::handle hwnd = NULL, bool windowed = true);
			~device();

			bool frame_begin();
			bool frame_end();

			void clear(unsigned int color, float depth = 1.0f);

			bool present();
			
			device_impl& get_impl();
			const device_impl& get_impl() const;

		private:
			core::windows::handle m_hwnd;
			pimpl m_pimpl;
		};
	}
}