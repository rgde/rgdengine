#include "stdafx.h"

#include <boost/assign.hpp>

using namespace rgde;
using core::windows::window;
using render::device;
using render::font;
using namespace math;

#include "Application.h"

#include <windows.h>

using rgde::render::vertex_element;
vertex_element custom_vertex_desc[] = 
{
	{0, 0,  vertex_element::float3,   vertex_element::default_method, vertex_element::position, 0}, 
	{0, 12, vertex_element::color4ub, vertex_element::default_method, vertex_element::color,	0},
	vertex_element::end_element
};


struct color_vertex				// Our new vertex struct
{
	float x, y, z;			// 3D position
	ulong color;			// Hex Color Value
};

unsigned short cube_ib[] = 
{
	0,1,2,		1,3,2,
	4,5,6,		5,7,6,
	8,9,10,		9,11,10,
	12,13,14,	13,15,14,
	16,17,18,	17,19,18,
	20,21,22,	21,23,22
};

color_vertex cube_geom[] =			// Vertex Array
{	
	// Front Blue Color
	{-1.0f, 1.0f,-1.0f,  0xF00FF00F },
	{ 1.0f, 1.0f,-1.0f,  0xF00FF00F },
	{-1.0f,-1.0f,-1.0f,  0xF00FF00F },
	{ 1.0f,-1.0f,-1.0f,  0xF00FF00F },
	// Back Orange Color
	{-1.0f, 1.0f, 1.0f,  0xFFF8800F },
	{-1.0f,-1.0f, 1.0f,  0xFFF8800F },
	{ 1.0f, 1.0f, 1.0f,  0xFFF8800F },
	{ 1.0f,-1.0f, 1.0f,  0xFFF8800F },
	// Top Red Color
	{-1.0f, 1.0f, 1.0f,  0xFFF0000F },
	{ 1.0f, 1.0f, 1.0f,  0xFFF0000F },
	{-1.0f, 1.0f,-1.0f,  0xFFF0000F },
	{ 1.0f, 1.0f,-1.0f,  0xFFF0000F },
	// Bottom Yellow Color
	{-1.0f,-1.0f, 1.0f,  0xFFFFF00F },
	{-1.0f,-1.0f,-1.0f,  0xFFFFF00F },
	{ 1.0f,-1.0f, 1.0f,  0xFFFFF00F },
	{ 1.0f,-1.0f,-1.0f,  0xFFFFF00F },
	// Right Blue Color
	{ 1.0f, 1.0f,-1.0f,  0xF0000FFF },
	{ 1.0f, 1.0f, 1.0f,  0xF0000FFF },
	{ 1.0f,-1.0f,-1.0f,  0xF0000FFF },
	{ 1.0f,-1.0f, 1.0f,  0xF0000FFF },
	// Left Violet Color
	{-1.0f, 1.0f,-1.0f,  0xFFF00FFF },
	{-1.0f,-1.0f,-1.0f,  0xFFF00FFF },
	{-1.0f, 1.0f, 1.0f,  0xFFF00FFF },
	{-1.0f,-1.0f, 1.0f,  0xFFF00FFF }
};


Application::Application(int x, int y, int w, int h, const std::wstring& title) 
	: m_active(true),
	window(math::vec2i(x, y), math::vec2i(w, h), title, 0, WS_BORDER | WS_CAPTION | WS_SYSMENU),
	m_device(get_handle()),
	m_font(font::create(m_device, 18, L"Arial", font::heavy)),
	m_arc_ball(w, h), 
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
	m_vb.reset();
	m_ib.reset();

	delete m_karaoke;
	m_karaoke = 0;
}

void Application::init_render_data()
{
	using namespace rgde::render;
	vertex_declaration_ptr decl = vertex_declaration::create(m_device, custom_vertex_desc, 3);

	{
		m_vb = vertex_buffer::create
			(
			m_device, decl, 
			sizeof(cube_geom), 
			resource::default, 
			buffer::write_only
			);

		void *pVertices = m_vb->lock( 0, sizeof(cube_geom), 0 );
		memcpy( pVertices, cube_geom, sizeof(cube_geom) );
		m_vb->unlock();
	}

	{
		m_ib = index_buffer::create(m_device, sizeof(cube_ib), false, resource::default, buffer::write_only);
		void *indices = m_ib->lock( 0, sizeof(cube_ib), 0 );
		memcpy( indices, cube_ib, sizeof(cube_ib) );
		m_ib->unlock();
	}

	m_device.set_ztest(true);
	m_device.set_cull_mode(rgde::render::cull_none);
	m_device.set_lighting(false);
	m_device.set_alpha_blend(false);
	m_device.set_alpha_test(false);

	math::vec3f cam_pos(-5, 0, 0);
	m_camera.lookAt(cam_pos, (math::vec3f(1,0,0) + cam_pos), math::vec3f(0,1,0));
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
	m_device.set_transform( render::view_transform, m_camera.getViewMatrix());
	m_device.set_transform( render::world_transform, m_arc_ball.get_matrix());	// Set The Transformation

	m_device.frame_begin();
	//m_device.clear(color::Black);
	m_device.clear(color(60,30,120,255));

	draw_cube();

	m_karaoke->render();

	//m_font->render(L"Hello", rect(10,100,100,100),color::Red, true);

	m_device.frame_end();
	m_device.present();
}

void Application::draw_cube()
{
	m_device.set_stream_source( 0, m_vb, sizeof(color_vertex) );
	m_device.set_index_buffer(m_ib);

	//m_device.draw(render::triangle_list, 0, 0, 24, 0, 12);

	//m_device.draw( render::triangle_strip,  0, 2 ); // Draw Front
	//m_device.draw( render::triangle_strip,  4, 2 ); // Draw Back
	//m_device.draw( render::triangle_strip,  8, 2 ); // Draw Top
	//m_device.draw( render::triangle_strip,  12, 2 ); // Draw Bottom
	//m_device.draw( render::triangle_strip,  16, 2 ); // Draw Right
	//m_device.draw( render::triangle_strip,  20, 2 ); // Draw Left
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
			//keys[wParam] = true;
			if (VK_UP == wparam)
			{
				//m_cam_pos += math::vec3f(0.05f,0,0);
			}
			else if (VK_F5 == wparam)
			{
				init_game_data();
			}
			else if (VK_DOWN == wparam)
			{
				//m_cam_pos -= math::vec3f(0.05f,0,0);
			}
			else if (VK_SPACE == wparam)
			{
				m_karaoke->on_key_pressed(wparam);
			}


			return 0;
		}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		SetCapture( get_handle() );
		ShowCursor(FALSE);
		{
			POINT p;
			GetCursorPos(&p);
			clicked_x = p.x;
			clicked_y = p.y;

			int xPos = LOWORD(lparam); 
			int yPos = HIWORD(lparam); 

			m_arc_ball.click(xPos, yPos);
			m_karaoke->on_mouse_click(xPos, yPos);
		}
		return 0;

	case WM_LBUTTONUP:
		{
			SetCursorPos(clicked_x, clicked_y);
			ShowCursor(TRUE);
			ReleaseCapture();
		}
		return 0;


	case WM_KEYUP:
		{
			return 0;
		}

	case WM_SIZE:
		{
			resize_scene(LOWORD(lparam), HIWORD(lparam));
			return 0;
		}
	case WM_MOUSEWHEEL:
		{
			float delta = (short)HIWORD((DWORD)wparam);//120.0f;
			delta /= 80.0f;
			m_camera.goForward(-delta);
		}
		break;

	case WM_MOUSEMOVE:
		{
			int xPos = LOWORD(lparam); 
			int yPos = HIWORD(lparam); 

			
			if ((old_x != -1 || old_y != -1) && ((wparam & MK_LBUTTON) != 0))
			{
				m_arc_ball.drag(xPos, yPos);
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

	m_camera.setProjection(45.0f, (float)width/(float)height, 0.1f, 100.0f);
	m_device.set_transform( render::projection_transform, m_camera.getProjMatrix());
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