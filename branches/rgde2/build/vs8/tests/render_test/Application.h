#pragma once

#include "ArcBall.h"
#include "camera.h"
#include "karaoke.h"
#include "game.h"

namespace rgde
{
	class Application : core::windows::window
	{
	public:
		Application();
		~Application();

		void run();

		render::device& get_render_device()
		{
			return m_device;
		}

	protected:
		virtual core::windows::result wnd_proc(ushort message, uint wparam, long lparam );
		void resize_scene(unsigned int width, unsigned int height);
		bool do_events();

		void init_render_data();

		void update();
		void render();

		void draw_cube();

	private:
		game::system m_game;
		ArcBall m_arc_ball;
		bool m_active;
		render::device m_device;

		render::font_ptr m_font;
		render::vertex_buffer_ptr m_vb;
		render::index_buffer_ptr m_ib;

		math::vec3f m_cam_pos;
		Camera m_camera;

		int old_x;
		int old_y;

		int clicked_x;
		int clicked_y;

		double m_elapsed;
		boost::timer m_timer;

		::game::karaoke *m_karaoke;
	};
}