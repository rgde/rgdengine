#include "stdafx.h"

#include "GameApplication.h"


namespace rgde
{
	GameApplication::GameApplication(int x, int y, int w, int h, const std::wstring& title)
		: m_active(true)
		, window(math::vec2i(x, y), math::vec2i(w, h), title, 0, WS_BORDER | WS_CAPTION | WS_SYSMENU)
		, m_device(get_handle())
		, m_batcher(m_device)
	{
		show();
		update();

		m_device.set_lighting(false);
		m_device.set_alpha_blend(false);
		m_device.set_alpha_test(false);

		m_debug_info = render::font::create(m_device, 12, L"Arial");
	}

	GameApplication::~GameApplication()
	{
	}

	void GameApplication::update_frame()
	{
	}

	void GameApplication::init_game_data()
	{
		m_ship_texture = render::base_texture::create(m_device, L"TestInput/SpaceShip.png");
		m_alien_texture = render::base_texture::create(m_device, L"TestInput/Alien.png");
	}

	void GameApplication::render_frame()
	{
		m_device.clear(math::color::Black);
		m_device.frame_begin();

		m_device.frame_end();
		m_device.present();
	}

	void GameApplication::run()
	{
		while( is_created() )
		{
			if( !do_events() && m_active)
			{
				update_frame();
				render_frame();
			}
		}
	}

	core::windows::result GameApplication::wnd_proc(ushort message, uint wparam, long lparam )
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

	bool GameApplication::do_events()
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