#include "stdafx.h"
#include "karaoke.h"
#include "Application.h"

#include "game_gui.h"
#include "main_menu.h"


using namespace rgde;

namespace game
{
	karaoke::karaoke(Application& app)
		: m_app(app)
		, m_cur_time(0)
		, cur_line(0)
		, cur_symbol(0)
		, m_cur_symbol_total(0)
		, m_sound_system(app.get_audio_system())
		, first_update(true)
		, m_game_score(0)
		, m_pause(false)
		, m_state(state_menu)
		, m_drawer2d(app.get_render_device())
	{

		rgde::render::device& dev = app.get_render_device();

		xml::node game_cfg = app.get_config()("root")("game");

		m_mech.read(game_cfg("mech"));

		
		m_worlds_font = xml_font(dev, game_cfg("worlds")("font"));
		m_font = xml_font(dev, game_cfg("song")("font"));

		m_song_color = xml_color(game_cfg("song")("color"));
		m_worlds_color = xml_color(game_cfg("worlds")("color"));
		
		m_worlds_bkcolor = xml_color(game_cfg("worlds")("bkcolor"));


		m_score_font = xml_font(dev, game_cfg("score")("font"));
		m_score_rect = xml_rect(game_cfg("score")("rect"));
		m_score_color = xml_color(game_cfg("score")("color"));
		m_score_bkcolor = xml_color(game_cfg("score")("bkcolor"));

		first_line_height = game_cfg("song")("lines")["height0"].as_int();
		second_line_height = game_cfg("song")("lines")["height1"].as_int();

		xml::node worlds = game_cfg("worlds");

		m_worlds_min_height = worlds("height")["min"].as_int();
		m_worlds_max_height = worlds("height")["max"].as_int();

		m_randomize_order = worlds("randomize_order")["value"].as_int() > 0;

		m_min_vert_speed = worlds("vertical_speed")["min"].as_float();
		m_max_vert_speed = worlds("vertical_speed")["max"].as_float();

		m_mult_font = xml_font(dev, game_cfg("mult")("font"));
		m_mult_rect = xml_rect(game_cfg("mult")("rect"));
		m_mult_color = xml_color(game_cfg("mult")("color"));
		m_mult_bkcolor = xml_color(game_cfg("mult")("bkcolor"));

		{
			xml::node progress_node = game_cfg("progress");

			m_progress_color = xml_color(progress_node("color"));
			m_progress_bkcolor = xml_color(progress_node("bkcolor"));
			m_progress_rect = xml_rect(progress_node("rect"));

			xml::node marker_node = progress_node("marker");

			m_progress_marker_color = xml_color(marker_node("color"));
			m_progress_marker_rect = xml_rect(marker_node("rect"));
		}

		load_game_data();
	}

	void karaoke::start_game()
	{
		cur_symbol = 0;
		cur_line = 0;
		m_score_multiplier = 1;
		m_game_score = 0;

		new score(*this);
		new mult(*this);

		m_sound_system.play(0);
		create_sprites();
	}

	rgde::render::device& karaoke::get_render_device() {return m_app.get_render_device();}

	void karaoke::render()
	{
		if (m_pause)
		{
			return;		
		}

		m_drawer2d.update();
		m_drawer2d.render();
		m_drawer2d.clear();

		{// render progress line
			karaoke::batcher2d& b = get_batcher();
			rgde::math::rect progress_rect = m_progress_rect;

			float progress =  m_cur_time/m_song_length;

			progress = progress > 1.0f ? 1.0f : progress;

			b.draw(progress_rect, m_progress_bkcolor);

			rgde::math::rect cur_progress_rect = progress_rect;
			cur_progress_rect.w *= progress;
			b.draw(cur_progress_rect, m_progress_color);

			float marker_pos = m_progress_rect.w*progress;
			rgde::math::rect progress_marker_rect = m_progress_marker_rect;

			progress_marker_rect.x += m_progress_rect.x + marker_pos - progress_marker_rect.w*0.5f;
			progress_marker_rect.y = m_progress_rect.y - (progress_marker_rect.h - m_progress_rect.h)*0.5;

			b.draw(progress_marker_rect, m_progress_marker_color);
		}

		const line_info& line = lines[cur_line%lines.size()];
		const std::wstring& text = line.text;
		size_t text_pos = line.text_poses[cur_symbol];

		float window_width = m_app.get_size().w;

		render_text_line(text, 
			std::min(text_pos, text.size()), 
			math::rect(0,first_line_height, window_width, 100), 
			m_song_color);

		const line_info& second_line = lines[(cur_line+1)%lines.size()];
		const std::wstring& second_text = second_line.text;

		render_text_line(second_text, 
			second_text.size(), 
			math::rect(0,second_line_height, window_width, 100), 
			math::color(m_song_color, 0.2f));

		typedef std::list<sprite*>::iterator sit;
		for(sit it = m_sprites.begin(); it != m_sprites.end(); ++it)
			(*it)->render();
	}

	void karaoke::render_text_line(std::wstring text, size_t pos, rgde::math::rect& r, 
									rgde::math::color c)
	{
		std::wstring part_text = std::wstring(text.begin(), text.begin() + pos);

		math::rect text_rect = m_font->measure_test(text);
		
		math::rect new_rect = r;
		new_rect.x = (r.w - r.x)/2 - text_rect.w/2;

		math::rect shadow_rect = new_rect;

		shadow_rect.x += 2;
		shadow_rect.y += 2;
		
		{
			const math::color shadow_color(c * 0.5f, c.a/255.0f);

			m_font->render(text, shadow_rect, shadow_color, false);
			m_font->render(part_text,new_rect,c, false);
		}
	}

	float karaoke::get_time() const
	{
		return m_cur_time;
	}

	rgde::math::color karaoke::get_world_color(float dt) const
	{
		const grade& g = m_mech.get_grade(dt);
		return g.color;
	}

	rgde::math::color karaoke::get_world_bkcolor(float dt) const
	{
		const grade& g = m_mech.get_grade(dt);
		return g.bkcolor;
	}

	float karaoke::on_world_clicked(song_world& world)
	{
		float dt = world.get_time() - m_cur_time;
		const grade& g = m_mech.get_grade(dt);
		float resulting_score = g.score;
		
		m_score_multiplier += g.mult;
		m_score_multiplier = m_score_multiplier > 0 ? m_score_multiplier : 1;

		add_score(resulting_score*m_score_multiplier);

		return resulting_score;
	}

	void karaoke::update(float dt)
	{
		if (m_pause)
			return;

		if (first_update)
		{
			first_update = false;
			timer_offset = dt;

			start_game();
		}

		float dtf = (dt-timer_offset) - m_cur_time/1000;

		{
			sprites temp_copy = m_sprites;
			typedef std::list<sprite*>::iterator sit;
			for(sit it = temp_copy.begin(); it != temp_copy.end(); ++it)
				(*it)->update(dtf);
		}		

		m_sound_system.update((int)dtf);

		m_cur_time = (dt-timer_offset)*1000;

		size_t timing_index = m_cur_symbol_total-cur_line%lines.size();

		if( timing_index < m_timings.size() && m_cur_time >= m_timings[timing_index] )
		{
			switch_world();
		}		
	}

	void karaoke::load_game_data()
	{
		xml::document songs;
		if (!songs.load_file("songs.xml"))
			return;

		xml::node song_node = songs("songs").first_child();
		if (!song_node)
			return;

		std::string lyrics_file_name = song_node("lyrics")["file"].value();
		std::string audio_db_file_name = song_node("sounds")["file"].value();

		//doc.load_file("Ryan Adams - Desire.xml");
		doc.load_file(lyrics_file_name.c_str());

		//m_sound_system.load("audiodb.xml");
		m_sound_system.load(audio_db_file_name.c_str());

		m_song_length = doc("root")("track_length")["value"].as_int();

		for(xml::node l = doc("root")("text"); 
			l.next_sibling("text"); 
			l = l.next_sibling("text"))
		{
			std::wstring text = xml::as_utf16(l.first_attribute().value());

			line_info li;
			li.text = text;
			
			li.text_poses.push_back(0);

			for(int pos = text.find(L" ", 0); pos != -1; pos = text.find(L" ", pos+1))
			{
				li.text_poses.push_back(pos);
			}

			li.text_poses.push_back((int)text.size());

			lines.push_back(li);
		}

		int line_count = 0;

		for(xml::node timings = doc("root")("sync"); 
			timings.next_sibling("sync"); 
			timings = timings.next_sibling("sync"))
		{
			std::string text = timings.first_attribute().value();

			using namespace std;
			using namespace boost;
			tokenizer<> tok(text);

			int cur_time = 0;

			line_info& li = lines[line_count];

			for(tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg)
			{
				cur_time = boost::lexical_cast<int>(*beg)*10;
				m_timings.push_back(cur_time);
				li.line_timings.push_back(cur_time);
			}

			line_count++;
		}
	}

	void karaoke::on_mouse_click(int x, int y)
	{
		if (m_pause)
			return;
		//switch_world();
		{
			sprites temp_copy = m_sprites;
			typedef std::list<sprite*>::iterator sit;
			for(sit it = temp_copy.begin(); it != temp_copy.end(); ++it)
			{
				sprite* s = *it;
				if (s->get_rect().is_inside(math::vec2f((float)x,(float)y)))
					s->on_click();
			}
		}
	}

	void karaoke::switch_world()
	{
		cur_symbol++;
		m_cur_symbol_total++;
		const line_info& line = lines[cur_line%lines.size()];
		if (line.text_poses.size() == cur_symbol)
		{
			switch_line();
		}
	}

	void karaoke::switch_line()
	{
		cur_symbol = 0;
		cur_line++;

		create_sprites();
	}

	void karaoke::create_sprites()
	{
		const line_info& l = get_line(cur_line);

		//float step = 640.0f/l.text_poses.size();
		float width = m_app.get_size().w - 40;

		int dh = m_worlds_max_height - m_worlds_min_height;

		bool do_rand = m_randomize_order;

		std::vector<int> poses;
		poses.reserve(l.text_poses.size());
		
		for(size_t i = 0; i < l.text_poses.size(); ++i)
		{
			std::wstring text = get_line(cur_line).get_world(i);
			if (!text.empty())
				poses.push_back(i);
		}

		if (m_randomize_order)
			std::random_shuffle(poses.begin(), poses.end());

		std::vector<float> sizes(poses.size());

		render::font_ptr font = get_worlds_font();
		float total_size = 0;

		for(size_t i = 0; i < poses.size(); ++i)
		{
			int line = cur_line;
			int world = (int)poses[i];
			std::wstring text = get_line(line).get_world(world);
			
			math::rect text_rect = font->measure_test(text);
			sizes[i] = text_rect.w;
			total_size += text_rect.w;
		}

		for(size_t i = 0; i < poses.size(); ++i)
		{
			sizes[i] = sizes[i]/total_size;
		}

		float cur_dw = 20;

		for(size_t i = 0;  i < poses.size(); ++i)
		{
			int line = cur_line;
			int world = (int)poses[i];

			float r = math::Math::unitRandom();

			float sector_width = width * sizes[i];
			float world_width = total_size * sizes[i];

			world_width += world_width*0.1f;

			float dw = (sector_width-world_width)*0.5f;

			
			new song_world(line, 
				world, 
				math::rect(cur_dw+dw, m_worlds_min_height + dh*r, world_width, 30.0f),
				*this);

			cur_dw += sector_width;
		}
	}

	void karaoke::swith_pause()
	{
		m_pause = !m_pause;
	}

	void karaoke::on_key_pressed(int key)
	{
		swith_pause();
	}

	karaoke::~karaoke()
	{
		m_sound_system.stop_all();

		sprites temp_copy = m_sprites;
		typedef std::list<sprite*>::iterator sit;
		for(sit it = temp_copy.begin(); it != temp_copy.end(); ++it)
		{
			sprite* s = *it;
			delete s;
		}
	}

	void karaoke::set_state(game_state state)
	{
		m_state = state;

		switch (state)
		{
		case state_menu:
			break;
		case state_song_selection:
			break;
		}
	}

	karaoke::game_state karaoke::get_state() const
	{
		return m_state;
	}
}