#pragma once

#include "camera.h"
#include "game.h"
#include "sprite.h"
#include "mesh.h"
#include "console.h"

namespace rgde
{
	struct size_t
	{
		float w, h;
	};

	class application : public core::windows::window
	{
	public:
		application(int x, int y, int w, int h, const std::wstring& title);
		~application();

		void run();

		const size_t& get_size() const {return m_size;} 

		render::device& get_render_device() { return m_device; }

		xml::document& get_config() {return m_config;}

		audio::system& get_audio_system() {return  m_sound_system;}

	protected:
		//virtual long wnd_proc(ushort message, uint wparam, long lparam );
		
		// window events
		bool on_resize(unsigned int width, unsigned int height);
		bool on_input_event(const core::windows::input_event& e);
		
		bool do_events();

		void init_game_data();
		void init_render_data();

		void test_collada_read();

		void update();
		void render();

	private:
		core::vfs::system m_filesystem;
		xml::document m_config;
		render::device m_device;
		audio::system m_sound_system;
		game::system m_game;

		game::console m_console;
					
		size_t m_size;

		struct scene_obj
		{
			math::mat44f world_tm;
			render::mesh_ptr mesh;
		};

		std::vector<scene_obj> m_scene_objects;
		
		render::mesh_ptr m_box;

		int old_x;
		int old_y;

		int clicked_x;
		int clicked_y;

		double m_elapsed;
		boost::timer m_timer;

		math::color m_back_color;	
		render::canvas m_batcher2d;

		render::font_ptr m_font;

		scene::camera_ptr m_camera;
		scene::free_camera m_camera_controller;
	};
}