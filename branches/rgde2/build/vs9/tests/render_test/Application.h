#pragma once

#include "ArcBall.h"
#include "camera.h"
#include "game.h"

#include "audio/audio.h"

namespace rgde
{
	inline math::color xml_color(xml::node n);
	inline render::font_ptr xml_font(render::device& dev, xml::node n);
	inline math::rect xml_rect(xml::node n);

	inline math::rect xml_rect(xml::node n)
	{
		float x, y, w, h;
		sscanf_s(n["value"].value(), "%f %f %f %f", &x, &y, &w, &h);
		return math::rect(x, y, w, h);
	}

	math::color xml_color(xml::node n)
	{
		int r, g, b, a;
		sscanf_s(n["value"].value(), "%d %d %d %d", &r, &g, &b, &a);
		return math::color((uchar)r, (uchar)g, (uchar)b, (uchar)a);
	}

	render::font_ptr xml_font(render::device& dev, xml::node n)
	{
		std::wstring name = xml::as_utf16(n["name"].value());
		int size = n["size"].as_int();
		std::string style = n["style"].value();

		render::font::weight w = render::font::normal;

		if (style == "bold")
			w = render::font::bold;

		return render::font::create(dev, size, name, w);
	}

	struct size
	{
		float w, h;
	};


	class Application : public core::windows::window
	{
	public:
		Application(int x, int y, int w, int h, const std::wstring& title);
		~Application();

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
	};
}