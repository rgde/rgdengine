#include "stdafx.h"

#include "BatcherApplication.h"

namespace rgde
{
	BatcherApplication::BatcherApplication(int x, int y, int w, int h, const std::wstring& title)
		: m_active(true)
		, window(math::vec2i(x, y), math::vec2i(w, h), title, 0, WS_BORDER | WS_CAPTION | WS_SYSMENU)
		, m_device(get_handle())
		, m_batcher(m_device)
	{
		show();
		update();

		init_render_data();		
	}

	void BatcherApplication::init_render_data()
	{
		m_device.set_lighting(false);
		m_device.set_alpha_blend(false);
		m_device.set_alpha_test(false);

		using namespace math;

		tex = render::base_texture::create(m_device, L"sprites/test1.jpg");

		for (int i = 1; i < 200; i++)
		{
			sprite_desc sprite;
			sprite.texture = tex;
			sprite.size = vec2f(20, 20);
			sprite.pos = vec2f(i, i);
			add_sprite(sprite);
		}
	}

	BatcherApplication::~BatcherApplication()
	{
	}

	void BatcherApplication::run()
	{
		while( is_created() )
		{
			if( !do_events() && m_active)
			{
				render_frame();
			}
		}
	}

	void BatcherApplication::render_frame()
	{
		m_device.clear(math::color::Black);
		m_device.frame_begin();

			m_batcher.render_all();

		m_device.frame_end();
		m_device.present();
	}

	void BatcherApplication::add_sprite(const sprite_desc &sprite)
	{
		m_batcher.add_sprite(sprite);
	}

	core::windows::result BatcherApplication::wnd_proc(ushort message, uint wparam, long lparam )
	{
		switch (message)
		{
		case WM_ACTIVATE:	// Watch For Window Activate Message
			m_active = !HIWORD(wparam);// Check Minimization State
			return 0;

		case WM_KEYDOWN:
		case WM_SIZE:
			return 0;

		case WM_MOUSEMOVE:
			break;
		}
		return window::wnd_proc(message, wparam, lparam);
	}

	bool BatcherApplication::do_events()
	{
		MSG msg = {0};
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
			return true;
		}
		return false;
	}
}