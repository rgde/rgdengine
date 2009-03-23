#pragma once

#include "game.h"

namespace rgde
{
	class Application;
}

namespace game
{
	class karaoke : rgde::game::object
	{
	public:
		karaoke(rgde::Application& app);

		void render();
		void update(float dt);

		void on_mouse_click(int x, int y);
		void on_key_pressed(int key);

	protected:
		void load_game_data();
		void render_text_line(std::wstring text, size_t pos, rgde::math::rect& r, rgde::math::color c);

	private:
		rgde::render::font_ptr m_font;

		size_t cur_line;
		size_t cur_symbol;

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

		rgde::Application& m_app;
	};
}