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

#include "terrain.h"

application::application(int x, int y, int w, int h, const std::wstring& title) 
	: m_active(true)
	, window(math::vec2i(x, y), math::vec2i(w, h), title, 0, WS_BORDER | WS_CAPTION | WS_SYSMENU)
	, m_device(get_handle(), m_filesystem)
	, m_elapsed(0)
	, m_sound_system(get_handle())
	, m_batcher2d(m_device)
	, m_console(m_device)
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

	test_collada_read();


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

	//m_box = mesh::create_box(m_device, 1, 1, 1);
	terrain_container::terrain ter(1,1,256,256);
	m_box = mesh::create_terrain_chunk(m_device,ter,0,0,1);

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

	m_device.set_tex_adressu(0, render::mode_wrap);
	m_device.set_tex_adressv(0, render::mode_wrap);

	m_box->render();
	
	m_batcher2d.render();

	m_font->render(L"render test:", math::rect(5,5, 300, 30), math::color::White, true);

	m_device.frame_end();
	m_device.present();
}

core::windows::result application::wnd_proc(ushort message, uint wparam, long lparam )
{
	static bool rm_down = false;

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

	case WM_RBUTTONDOWN:
		rm_down = true;
		break;
	case WM_RBUTTONUP:
		rm_down = false;
		break;
	case WM_MOUSEMOVE:		
		{
			int xPos = LOWORD(lparam); 
			int yPos = HIWORD(lparam);

			if (rm_down && (old_x != -1 || old_y != -1) /*&& ((wparam & MK_LBUTTON) != 0)*/)
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

//////////////////////////////////////////////////////////////////////////

//#include "collada.h"

void application::test_collada_read()
{
	xml::document dae_doc;
	if (dae_doc.load(m_filesystem.open_read("models/MS-00X Experimental/models/MS-00X Experimental.dae")))
	{		
		//() - get node
		//[] - get attribute

		xml::node collada_node = dae_doc("COLLADA");

		xml::node asset_node = collada_node("COLLADA");

		if (xml::node library_images_node = collada_node("library_images"))
		{
			for(xml::node n = library_images_node("image");n;n = n.next_sibling())
			{
				std::string id = n["id"].value();
				std::string name = n["name"].value();
				std::string load_path = n("init_from").first_child().value();

				__asm nop;
			}
		}

		if (xml::node library_materials_node = collada_node("library_materials"))
		{
			for(xml::node n = library_materials_node("material");n;n = n.next_sibling())
			{ 
				std::string id = n["id"].value();
				std::string name = n["name"].value();
				std::string effect_instance = n("instance_effect")["url"].value();
				__asm nop;
			}

		}

		if (xml::node library_effects_node = collada_node("library_effects"))
		{
			for(xml::node n = library_effects_node("effect");n;n = n.next_sibling())
			{ 
				std::string id = n["id"].value();
				std::string name = n["name"].value();
				__asm nop;
			}
		}

		if(xml::node library_geometries_node = collada_node("library_geometries"))
		{
			for(xml::node n = library_geometries_node("geometry");n;n = n.next_sibling())
			{ 
				std::string id = n["id"].value();
				std::string name = n["name"].value();

				for(xml::node mesh_node = n("mesh"); mesh_node; mesh_node = mesh_node.next_sibling())
				{
					xml::node source_node = mesh_node("source");
					std::string source_id = source_node["id"].value();

					xml::node array_node = source_node("float_array");
					std::string array_id = array_node["id"].value();
					int cout = array_node["count"].as_int();

					const char* str_data = array_node.first_child().value();

					__asm nop;
				}

				__asm nop;
			}
		}

		xml::node library_nodes_node = collada_node("library_nodes");
		xml::node library_cameras_node = collada_node("library_cameras");
		xml::node library_visual_scenes_node = collada_node("library_visual_scenes");

		__asm nop;
	}
}