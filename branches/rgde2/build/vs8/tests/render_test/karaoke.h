#pragma once

#include "game.h"

namespace rgde
{
	class Application;
}

namespace audio
{
	class system;
}

namespace game
{
	class karaoke : rgde::game::object
	{
		friend class sprite;
		typedef std::list<sprite*> sprites;
		sprites m_sprites;

	public:
		enum game_state
		{
			state_menu, 
			state_game,
			state_song_selection,
			state_options
		};

		karaoke(rgde::Application& app);
		~karaoke();

		void start_game();

		void set_state(game_state state);
		game_state get_state() const;

		void render();
		void update(float dt);

		void on_mouse_click(int x, int y);
		void on_key_pressed(int key);

		int get_game_score() const {return m_game_score;}
		void add_score(int v) {m_game_score += v;}
		int get_score_multiplier() const {return m_score_multiplier;}

		rgde::render::device& get_render_device();

		size_t get_cur_line() const {return cur_line;}
		size_t get_cur_symbol() const {return cur_symbol;}

		struct line_info
		{
			int start_time;
			int end_time;
			std::wstring text;
			std::vector<int> line_timings;
			std::vector<int> text_poses;

			inline std::wstring get_world(size_t i) const 
			{
				if (i >= text_poses.size() || text.empty())
					return std::wstring(L"");

				size_t start = std::min((size_t)text_poses[i], text.size()-1);

				size_t end = i+1 == text_poses.size() ? text.size()-1
					: text_poses[i+1];

				return std::wstring(&text[start], &text[end]);
			}
		};

		void swith_pause();

		const line_info& get_line(int line) const {return lines[line];}
		rgde::render::font_ptr get_worlds_font() {return m_worlds_font;}

		const rgde::math::color& get_song_color() const {return m_song_color;}

		const rgde::math::color& get_worlds_color() const {return m_worlds_color;}

		rgde::render::font_ptr get_score_font() const {return m_score_font;}
		const rgde::math::color& get_score_color() const {return m_score_color;}

		const rgde::math::rect& get_score_rect() const {return m_score_rect;}

		float get_min_vert_speed() const {return m_min_vert_speed;}
		float get_max_vert_speed() const {return m_max_vert_speed;}

		int get_worlds_min_height() const {return m_worlds_min_height;}
		int get_worlds_max_height() const {return m_worlds_max_height;}

	protected:
		void load_game_data();
		void render_text_line(std::wstring text, size_t pos, rgde::math::rect& r, rgde::math::color c);
		void switch_line();
		void switch_world();
		void create_sprites();

	private:
		game_state m_state;
		bool m_pause;
		int m_game_score;
		int m_score_multiplier;

		int first_line_height;
		int second_line_height;

		int m_worlds_min_height;
		int m_worlds_max_height;

		float m_min_vert_speed;
		float m_max_vert_speed;

		bool m_randomize_order;

		rgde::render::font_ptr m_font;
		rgde::math::color m_song_color;

		rgde::render::font_ptr m_worlds_font;
		rgde::math::color m_worlds_color;

		rgde::render::font_ptr m_score_font;
		rgde::math::color m_score_color;

		rgde::math::rect m_score_rect;

		bool first_update;
		float timer_offset;

		audio::system& m_sound_system;

		float m_cur_time;
		int m_cur_symbol_total;

		size_t cur_line;
		size_t cur_symbol;


		std::vector<int> m_timings;
		std::vector<line_info> lines;

		xml::document doc;

		rgde::Application& m_app;
	};
}