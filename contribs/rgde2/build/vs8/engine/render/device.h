#pragma once

#include <engine/render/render_forward.h>
#include <boost/noncopyable.hpp>

struct IDirect3D9;
struct IDirect3DDevice9;

namespace rgde
{
	namespace render
	{
		class device : boost::noncopyable
		{
		public:
			explicit device(core::windows::handle hwnd = NULL, bool windowed = true);
			~device();

			bool begin();
			bool end();

			void clear(unsigned int color, float depth = 1.0f);

			bool present();
			
			IDirect3DDevice9* get_dx_device() const {return m_device;}

		private:
			core::windows::handle m_hwnd;
			IDirect3D9* m_d3d;
			IDirect3DDevice9* m_device;
		};
	}
}