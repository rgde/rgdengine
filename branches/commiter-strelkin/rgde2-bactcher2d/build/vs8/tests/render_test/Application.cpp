#include "stdafx.h"

#include <boost/assign.hpp>

using namespace rgde;
using core::windows::window;
using render::device;
using render::font;
using namespace math;

#include "Application.h"

#include <windows.h>


Application::Application(int x, int y, int w, int h, const std::wstring& title) 
	: m_active(true),
	window(math::vec2i(x, y), math::vec2i(w, h), title, 0, WS_BORDER | WS_CAPTION | WS_SYSMENU),
	m_device(get_handle()),
	//m_font(font::create(m_device, 18, L"Arial", font::heavy)),
	//m_arc_ball(w, h), 
	m_elapsed(0),
	m_karaoke(0), 
	m_sound_system(get_handle())
{
	m_size.w = w;
	m_size.h = h;

	old_x = -1;
	old_y = -1;

	clicked_x = 0;
	clicked_y = 0;

	show();
	update();

	init_game_data();
	init_render_data();
}

Application::~Application()
{
	//m_vb.reset();
	//m_ib.reset();

	delete m_karaoke;
	m_karaoke = 0;
}

void Application::init_render_data()
{
	using namespace rgde::render;

	m_device.set_ztest(true);
	m_device.set_cull_mode(rgde::render::cull_none);
	m_device.set_lighting(false);
	m_device.set_alpha_blend(false);
	m_device.set_alpha_test(false);

	//math::vec3f cam_pos(-5, 0, 0);
	//m_camera.lookAt(cam_pos, (math::vec3f(1,0,0) + cam_pos), math::vec3f(0,1,0));
}

void Application::run()
{	
	m_timer.restart();

	while( is_created() )
	{
		if( !do_events() && m_active)
		{
			m_elapsed = m_timer.elapsed();
			update();
			render();
		}
	}
}

void Application::update()
{
	m_game.do_update((float)m_elapsed);
}

void Application::render()
{
	m_device.frame_begin();
	m_device.clear(m_back_color);

	m_karaoke->render();

	m_device.frame_end();
	m_device.present();
}

core::windows::result Application::wnd_proc(ushort message, uint wparam, long lparam )
{
	switch (message)
	{
	case WM_ACTIVATE:	// Watch For Window Activate Message
		{
			if (!HIWORD(wparam))// Check Minimization State
				m_active = true;
			else
				m_active = false;

			return 0;
		}
	case WM_KEYDOWN:
		{
			if (VK_F5 == wparam)
			{
				init_game_data();
			}
			else if (VK_SPACE == wparam)
			{
				m_karaoke->on_key_pressed(wparam);
			}

			return 0;
		}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		{
			POINT p;
			GetCursorPos(&p);
			clicked_x = p.x;
			clicked_y = p.y;

			int xPos = LOWORD(lparam); 
			int yPos = HIWORD(lparam); 

			m_karaoke->on_mouse_click(xPos, yPos);
		}

		return 0;

	case WM_SIZE:
		{
			resize_scene(LOWORD(lparam), HIWORD(lparam));
			return 0;
		}
		break;

	case WM_MOUSEMOVE:
		{
			int xPos = LOWORD(lparam); 
			int yPos = HIWORD(lparam); 

			
			if ((old_x != -1 || old_y != -1) && ((wparam & MK_LBUTTON) != 0))
			{
				//m_arc_ball.drag(xPos, yPos);
				int dx = xPos - old_x;
				int dy = yPos - old_y;
			}

			old_x = xPos;
			old_y = yPos;
		}
		break;
	}
	return window::wnd_proc(message, wparam, lparam);
}

void Application::resize_scene(unsigned int width, unsigned int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}
}

void Application::init_game_data()
{
	if(m_karaoke)
	{
		delete m_karaoke;
	}

	bool res = m_config.load_file("config.xml");
	m_back_color = xml_color(m_config("root")("app")("background")("color"));
	m_karaoke = new ::game::karaoke(*this);
}


bool Application::do_events()
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