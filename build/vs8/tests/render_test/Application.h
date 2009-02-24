#pragma once

#include "ArcBall.h"
#include "camera.h"

namespace rgde
{
	class Application : window
	{
	public:
		Application();
		~Application();

		void run();

	protected:
		virtual core::windows::result wnd_proc(ushort message, uint wparam, long lparam );
		void resize_scene(unsigned int width, unsigned int height);
		bool do_events();

		const rgde::math::mat44f& getWorldMatrix() const {return matWorld;}

	private:
		math::mat44f matWorld;
		ArcBall m_arc_ball;
		bool m_active;
		device m_device;
		render::font_ptr m_font;
		render::vertex_buffer_ptr m_vb;
		render::index_buffer_ptr m_ib;
		math::vec3f m_cam_pos;
		Camera m_camera;

		struct line_info
		{
			int start_time;
			int end_time;
			std::string text;
			std::vector<int> line_timings;
			std::vector<int> text_poses;
		};

		std::vector<int> timings;
		std::vector<line_info> lines;


		xml::document doc;
	};
}