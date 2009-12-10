#include "stdafx.h"

#include <boost/assign.hpp>

#include <d3dx9.h>

using namespace rgde;
using core::windows::window;
using render::device;
using render::font;
using namespace math;

#include "application.h"
#include <rgde/render/surface.h>

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


color_vertex cube_vb[24] =			// Vertex Array
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

	m_camera = new scene::camera();

	resize_scene(w, h);
	
	init_game_data();
	init_render_data();

	m_cam_controller = scene::free_camera::create(m_camera);

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

	using namespace rgde::render;

	m_box = mesh::create_box(m_device, 1, 1, 1);
	//vertex_declaration_ptr decl = vertex_declaration::create(m_device, custom_vertex_desc, 3);

	//{
	//	m_vb = vertex_buffer::create
	//		(
	//		m_device, decl, 
	//		sizeof(cube_vb), 
	//		resource::default, 
	//		buffer::write_only
	//		);

	//	void *vertices = m_vb->lock( 0, sizeof(cube_vb), 0 );
	//	memcpy( vertices, cube_vb, sizeof(cube_vb) );
	//	m_vb->unlock();
	//}

	//{
	//	m_ib = index_buffer::create(m_device, sizeof(cube_ib), false, resource::default, buffer::write_only);
	//	void *indices = m_ib->lock( 0, sizeof(cube_ib), 0 );
	//	memcpy( indices, cube_ib, sizeof(cube_ib) );
	//	m_ib->unlock();
	//}

	m_font = font::create(m_device, 17, L"Arial");

	texture_ptr t = texture::create(m_device, m_filesystem.open_read("sprites/test01.jpg"));

	sprite s;
	s.texture = t;

	render::view_port vp;
	m_device.get_viewport(vp);

	s.x = vp.width - 80;
	s.y = vp.height - 80;
	s.w = 80;
	s.h = 80;
	s.color = math::color::White;

	m_batcher2d.draw(s);
	
	m_device.set_ztest(true);
	m_device.set_cull_mode(rgde::render::cull_none);
	m_device.set_lighting(false);
	m_device.set_alpha_blend(false);
	m_device.set_alpha_test(false);

	math::vec3f cam_pos(-5, 0, 0);
	m_camera->look_at(cam_pos, (math::vec3f(1,0,0) + cam_pos), math::vec3f(0,1,0));
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

	m_device.set_transform( render::projection_transform, m_camera->get_proj_matrix());
	m_device.set_transform( render::view_transform, m_camera->get_view_matrix());

	m_device.set_ztest(true);
	m_device.set_cull_mode(rgde::render::cull_none);
	m_device.set_lighting(false);
	m_device.set_alpha_blend(false);
	m_device.set_alpha_test(false);

	//m_device.set_stream_source( 0, m_vb, sizeof(color_vertex) );
	//m_device.set_index_buffer(m_ib);

	//m_device.draw(render::triangle_list, 0, 0, 24, 0, 12);

	m_box->render(m_device);
	
	m_batcher2d.render();

	m_font->render(L"render test:", math::rect(5,5, 300, 30), math::color::White, true);


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
			if ('Q' == wparam || 'q' == wparam)
				exit(0);

			if (VK_ESCAPE == wparam)
				exit(0);

			if ('w' == wparam || 'W' == wparam)
			{
				m_cam_controller->move_forward(0.1f);
			}
			else if ('s' == wparam || 'S' == wparam)
			{
				m_cam_controller->move_back(0.1f);
			}
			else if ('a' == wparam || 'A' == wparam)
			{
				m_cam_controller->move_left(0.1f);
			}
			else if ('d' == wparam || 'D' == wparam)
			{
				m_cam_controller->move_right(0.1f);
			}

			return 0;
		}
	//case WM_LBUTTONDOWN:
	//case WM_LBUTTONDBLCLK:
	//	{
	//		POINT p;
	//		GetCursorPos(&p);
	//		clicked_x = p.x;
	//		clicked_y = p.y;

	//		int xPos = LOWORD(lparam); 
	//		int yPos = HIWORD(lparam); 
	//	}

	//	return 0;

	case WM_SIZE:
		{
			resize_scene(LOWORD(lparam), HIWORD(lparam));
			return 0;
		}
		break;

	case WM_MOUSEWHEEL:
		{
			float delta = (short)HIWORD((DWORD)wparam);
			delta /= 80.0f;
			m_cam_controller->move_forward(delta);
		}
		break;

	case WM_MOUSEMOVE:
		{
			int xPos = LOWORD(lparam); 
			int yPos = HIWORD(lparam);

			if ((old_x != -1 || old_y != -1) /*&& ((wparam & MK_LBUTTON) != 0)*/)
			{
				int dx = xPos - old_x;
				int dy = yPos - old_y;

				m_cam_controller->rotate_right(dx/200.0f);
				m_cam_controller->rotate_up(dy/200.0f);
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
	if (height==0) // Prevent A Divide By Zero By
	{
		height=1; // Making Height Equal One
	}

	if (m_camera)
		m_camera->set_projection(45.0f, (float)width/(float)height, 0.1f, 100.0f);
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