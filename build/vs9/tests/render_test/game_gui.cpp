#include "stdafx.h"
#include "karaoke.h"
#include "game_gui.h"

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
		karaoke::line_info song_line = m_game.get_line(line);
		m_text = song_line.get_world(world);
		m_time = song_line.line_timings[world];
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
			float score = m_game.on_world_clicked(*this);
			m_text = boost::lexical_cast<std::wstring>(score);
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
		float dt = m_time - m_game.get_time();

		if(!m_clicked)
			m_color = m_game.get_world_color(dt);

		m_font->render(m_text, m_rect, m_color, 
			true, render::font::vcenter | render::font::center);

		if(!m_clicked)
		{
			karaoke::batcher2d& b = m_game.get_batcher();
			b.draw(m_rect, m_game.get_world_bkcolor(dt));
		}
	}

	void score::render()
	{
		int score = m_game.get_game_score();
		std::wstring score_txt = L"Score:" + boost::lexical_cast<std::wstring>(score);
		const math::color text_color = m_game.get_score_color();
		m_font->render(score_txt, m_rect, text_color, false, render::font::center|render::font::vcenter);

		karaoke::batcher2d& b = m_game.get_batcher();
		b.draw(m_rect, m_game.get_score_bkcolor());
	}

	//////////////////////////////////////////////////////////////////////////

	mult::mult(karaoke& k) 
		: sprite(k.get_mult_rect(), k)
	{
		m_font = k.get_mult_font();
	}

	void mult::render()
	{
		int value = m_game.get_score_multiplier();
		std::wstring text = L"x" + boost::lexical_cast<std::wstring>(value);
		const math::color color = m_game.get_mult_color();
		m_font->render(text, m_rect, color, false, render::font::vcenter|render::font::center);

		karaoke::batcher2d& b = m_game.get_batcher();
		b.draw(m_rect, m_game.get_mult_bkcolor());
	}

}