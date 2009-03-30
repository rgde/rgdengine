#include "stdafx.h"
#include "karaoke.h"
#include "Application.h"


using namespace rgde;

namespace game
{
	sprite::sprite(const rgde::math::rect& r, karaoke& k)
		: m_game(k), m_rect(r), m_visible(true)
	{
		m_handle = m_game.m_sprites.insert(m_game.m_sprites.end(), this);
	}

	sprite::~sprite()
	{
		m_game.m_sprites.erase(m_handle);
	}

	score::score(karaoke& k) 
		: sprite(k.get_score_rect(), k)
	{
		m_font = k.get_score_font();
	}


	song_world::song_world(int line, int world, rgde::math::rect& r, karaoke& k)
		: sprite(r, k)
		, m_clicked(false)
		, m_line(line)
		, m_world(world)
		, m_font(k.get_worlds_font())
	{		
		m_text = m_game.get_line(line).get_world(world);
		m_vert_move_dir = math::Math::unitRandom() >= 0.5f ? 1 : -1;
		float rand = math::Math::unitRandom();
		float min_speed = k.get_min_vert_speed();
		float max_speed = k.get_max_vert_speed();
		m_vert_move_speed = min_speed + (max_speed-min_speed)*rand;
	}

	void song_world::on_click()
	{
		if (!m_clicked)
		{
			m_clicked = true;
			m_game.add_score(10);
			m_text = boost::lexical_cast<std::wstring>(10);
		}
	}

	void song_world::update(float dt)
	{		
		if (m_line < m_game.get_cur_line())
			hide();

		if (!is_visible())
			delete this;

		if (m_clicked)
			return;

		m_rect.y += dt*m_vert_move_speed*m_vert_move_dir;

		if (m_rect.y >= m_game.get_worlds_max_height())
		{
			m_rect.y = m_game.get_worlds_max_height();
			m_vert_move_speed = -m_vert_move_speed;
		}
		else if (m_rect.y <= m_game.get_worlds_min_height())
		{
			m_rect.y = m_game.get_worlds_min_height();
			m_vert_move_speed = -m_vert_move_speed;
		}
	}

	void song_world::render()
	{
		m_font->render(m_text, m_rect, m_game.get_worlds_color(), true, 
			render::font::top | render::font::center);
	}


	void score::render()
	{
		int score = m_game.get_game_score();
		std::wstring score_txt = boost::lexical_cast<std::wstring>(score);
		const math::color text_color = m_game.get_score_color();//(130, 120, 100, 255);
		m_font->render(score_txt, m_rect, text_color, false, render::font::left|render::font::top);
	}

	//////////////////////////////////////////////////////////////////////////

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
	{

		rgde::render::device& dev = app.get_render_device();

		xml::node game_cfg = app.get_config()("root")("game");		

		m_score_font = xml_font(dev, game_cfg("score")("font"));
		m_worlds_font = xml_font(dev, game_cfg("worlds")("font"));
		m_font = xml_font(dev, game_cfg("song")("font"));

		m_song_color = xml_color(game_cfg("song")("color"));
		m_worlds_color = xml_color(game_cfg("worlds")("color"));
		m_score_color = xml_color(game_cfg("score")("color"));

		m_score_rect = xml_rect(game_cfg("score")("rect"));

		first_line_height = game_cfg("song")("lines")["height0"].as_int();
		second_line_height = game_cfg("song")("lines")["height1"].as_int();

		xml::node worlds = game_cfg("worlds");

		m_worlds_min_height = worlds("height")["min"].as_int();
		m_worlds_max_height = worlds("height")["max"].as_int();

		m_randomize_order = worlds("randomize_order")["value"].as_int() > 0;

		m_min_vert_speed = worlds("vertical_speed")["min"].as_float();
		m_max_vert_speed = worlds("vertical_speed")["max"].as_float();

		load_game_data();

		cur_symbol = 0;
		cur_line = 0;

		new score(*this);
	}

	rgde::render::device& karaoke::get_render_device() {return m_app.get_render_device();}

	void karaoke::render()
	{
		if (m_pause)
		{
			return;
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

	void karaoke::update(float dt)
	{
		if (m_pause)
		{
			return;
		}

		if (first_update)
		{
			first_update = false;
			timer_offset = dt;

			m_sound_system.play(0);
			create_sprites();
		}

		{
			sprites temp_copy = m_sprites;
			typedef std::list<sprite*>::iterator sit;
			for(sit it = temp_copy.begin(); it != temp_copy.end(); ++it)
				(*it)->update(dt);
		}


		float dtf = (dt-timer_offset) - m_cur_time/1000;

		m_sound_system.update((int)dtf);

		m_cur_time = (dt-timer_offset)*1000;

		if(m_cur_time >= m_timings[m_cur_symbol_total-cur_line%lines.size()] )
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

		float step = 640.0f/l.text_poses.size();

		int dh = m_worlds_max_height - m_worlds_min_height;

		bool do_rand = m_randomize_order;

		std::vector<int> poses(l.text_poses.size());
		
		for(size_t i = 0; i < poses.size(); ++i)
			poses[i] = i;

		if (m_randomize_order)
			std::random_shuffle(poses.begin(), poses.end());

		for(size_t i = 0;  i < poses.size(); ++i)
		{
			float r = math::Math::unitRandom();
			new song_world(cur_line, 
				(int)poses[i], 
				math::rect(20+i*step, m_worlds_min_height + dh*r, step, 40.0f), *this);
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
}