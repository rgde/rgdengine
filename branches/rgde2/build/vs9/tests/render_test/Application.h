#pragma once

#include "ArcBall.h"
#include "camera.h"
#include "game.h"

namespace rgde
{
	struct size
	{
		float w, h;
	};


	class application : public core::windows::window
	{
	public:
		application(int x, int y, int w, int h, const std::wstring& title);
		~application();

		void run();

		const size& get_size() const {return m_size;} 

		render::device& get_render_device()
		{
			return m_device;
		}

		xml::document& get_config() {return m_config;}

		audio::system& get_audio_system() {return  m_sound_system;}

	protected:
		virtual core::windows::result wnd_proc(ushort message, uint wparam, long lparam );
		void resize_scene(unsigned int width, unsigned int height);
		bool do_events();

		void init_game_data();
		void init_render_data();

		void update();
		void render();

	private:
		xml::document m_config;
		game::system m_game;
		//ArcBall m_arc_ball;
		bool m_active;
		render::device m_device;

		//math::vec2i m_size;
		size m_size;

		//render::font_ptr m_font;
		//render::vertex_buffer_ptr m_vb;
		//render::index_buffer_ptr m_ib;

		//math::vec3f m_cam_pos;
		//camera m_camera;

		int old_x;
		int old_y;

		int clicked_x;
		int clicked_y;

		double m_elapsed;
		boost::timer m_timer;

		math::color m_back_color;

		audio::system m_sound_system;
		core::vfs::system m_vfs;
	};
}