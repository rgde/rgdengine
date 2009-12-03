#include "stdafx.h"

#include <boost/assign.hpp>


using namespace rgde;
using core::windows::window;
using render::device;
using render::font;
using namespace math;

#include "application.h"
#include <rgde/render/surface.h>


application::application(int x, int y, int w, int h, const std::wstring& title) 
	: m_active(true)
	, window(math::vec2i(x, y), math::vec2i(w, h), title, 0, WS_BORDER | WS_CAPTION | WS_SYSMENU)
	, m_device(get_handle())
	, m_elapsed(0)
	, m_sound_system(get_handle())
	, m_batcher2d(m_device)
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

	xml::document doc;
	if (doc.load(m_filesystem.open_read("audiodb.xml")))
	{
		xml::node audio_db = doc("AudioTagDatabase");
		
		if (m_sound_system.load(audio_db))
			m_sound_system.play("music1");
	}	
}

application::~application()
{
	//m_vb.reset();
	//m_ib.reset();
}

void application::init_render_data()
{
	using namespace rgde::render;

	m_device.set_ztest(true);
	m_device.set_cull_mode(rgde::render::cull_none);
	m_device.set_lighting(false);
	m_device.set_alpha_blend(false);
	m_device.set_alpha_test(false);

	m_font = font::create(m_device, 17, L"Arial");

	texture_ptr t = texture::create(m_device, m_filesystem.open_read("sprites/test01.jpg"));

	sprite s;
	s.texture = t;
	s.pos = math::vec2f(200,100);
	s.size= math::vec2f(400,400);
	s.color = math::color::White;

	m_batcher2d.draw(s);
	//__asm nop;
	//math::vec3f cam_pos(-5, 0, 0);
	//m_camera.lookAt(cam_pos, (math::vec3f(1,0,0) + cam_pos), math::vec3f(0,1,0));
}

void application::run()
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

void application::update()
{
	m_game.do_update((float)m_elapsed);

	m_batcher2d.update();

	m_sound_system.update((float)m_elapsed);
}

void application::render()
{
	m_device.frame_begin();
	m_device.clear(m_back_color);

	m_batcher2d.render();

	m_font->render(L"Render test:", math::rect(5,5, 300, 30), math::color::White.data, true);

	m_device.frame_end();
	m_device.present();
}

core::windows::result application::wnd_proc(ushort message, uint wparam, long lparam )
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
			}
			else if (VK_SPACE == wparam)
			{
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

void application::resize_scene(unsigned int width, unsigned int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}
}

void application::init_game_data()
{
	m_back_color = rgde::math::color(128, 130, 160, 255);
}

bool application::do_events()
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