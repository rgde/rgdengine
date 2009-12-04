#include "stdafx.h"

#include "GameApplication.h"


namespace rgde
{
	AlienUnit::AlienUnit()
	{
		using namespace gmtl::Math;
		using namespace math;

		velocity = rangeRandom(5.0f, 25.0f);

		sprite.pos = vec2f(rangeRandom(0.0f, 800.0f), rangeRandom(-128.0f, 728.0f));
		sprite.size = vec2f(rangeRandom(46.0f, 87.0f), rangeRandom(46.0f, 87.0f));
		sprite.spin = rangeRandom(0.f,360.f);
		sprite.color = color(255, 255, 255, 255);
	}

	GameApplication::GameApplication(int x, int y, int w, int h, const std::wstring& title)
		: m_active(true)
		, window(math::vec2i(x, y), math::vec2i(w, h), title, 0, WS_BORDER | WS_CAPTION | WS_SYSMENU)
		, m_device(get_handle())
		, m_batcher(m_device)
	{
		show();
		update();
		ShowCursor(false);

		init_game_data();

		m_debug_info = render::font::create(m_device, 12, L"Arial");
	}

	GameApplication::~GameApplication()
	{
	}

	void GameApplication::init_game_data()
	{
		using namespace math;

		m_aliens.clear();

		m_ship_texture = render::base_texture::create(m_device, L"TestInput/SpaceShip.png");
		m_alien_texture = render::base_texture::create(m_device, L"TestInput/Alien.png");

		ship.color = color(255, 255, 255, 255);
		ship.pos = vec2f(400.0f, 200.0f);
		ship.size = vec2f(64.0f, 256.0f);
		ship.texture = m_ship_texture;

		for (int i=0; i<20; i++)
		{
			AlienUnit unit;
			unit.sprite.texture = m_alien_texture;
			add_unit(unit);
		}
	}

	void GameApplication::update_frame()
	{
		using namespace render;

		m_batcher.clear_all();

		for(aliens_iter it = m_aliens.begin(); it != m_aliens.end(); ++it)
		{
			primitives_2d::sprite_desc &sprite = it->sprite;
			sprite.pos += math::vec2f(0, 0.05); 

			if(sprite.pos[1] >600)
			{
				sprite.pos[1] = - 50;
			}

			m_batcher.add_sprite(sprite);
		}		
		m_batcher.add_sprite(ship);	
	} 

	void GameApplication::render_frame()
	{
		m_device.clear(math::color::Black);
		m_device.frame_begin();

			m_batcher.render_all();

		m_device.frame_end();
		m_device.present();
	}

	void GameApplication::add_unit(AlienUnit& unit)
	{
		m_aliens.push_back(unit);
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
			float xPos = LOWORD(lparam); 
			float yPos = HIWORD(lparam); 
			ship.pos = math::vec2f(xPos, yPos);
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