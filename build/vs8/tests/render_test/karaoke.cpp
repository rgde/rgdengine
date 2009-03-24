#include "stdafx.h"
#include "karaoke.h"
#include "Application.h"
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

using namespace rgde;

namespace game
{
	karaoke::karaoke(Application& app)
		: m_app(app), m_cur_time(0)
		, m_cur_symbol_total(0)
	{
		m_font = render::font::create(m_app.get_render_device(), 30, L"Arial", render::font::heavy);
		load_game_data();

		cur_symbol = 0;
		cur_line = 0;
	}

	void karaoke::render()
	{
		const line_info& line = lines[cur_line%lines.size()];
		const std::wstring& text = line.text;
		size_t text_pos = line.text_poses[cur_symbol];

		render_text_line(text, 
			std::min(text_pos, text.size()), 
			math::rect(10,10,620,100), 
			math::color::Red);

		const line_info& second_line = lines[(cur_line+1)%lines.size()];
		const std::wstring& second_text = second_line.text;

		render_text_line(second_text, 
			second_text.size(), 
			math::rect(10,40,620,100), 
			math::color(math::color::Red, 0.2f));
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
		m_cur_time = dt*1000;

		if(m_cur_time >= m_timings[m_cur_symbol_total] )
		{
			switch_world();
		}
	}

	void karaoke::load_game_data()
	{
		doc.load_file("sample.xml");

		xml::node timings = doc("Song")("Timings");
		{
			std::string text = timings.first_child().value();

			using namespace std;
			using namespace boost;
			tokenizer<> tok(text);

			int cur_time = 0;

			for(tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg)
			{
				cur_time += boost::lexical_cast<int>(*beg);
				m_timings.push_back(cur_time);
			}
		}

		for(xml::node l = doc("Song")("Text")("Line"); 
			l.next_sibling("Line"); 
			l = l.next_sibling("Line"))
		{
			std::wstring text = xml::as_utf16(l.first_child().value());

			line_info li;
			li.text = text;
			
			li.text_poses.push_back(0);

			for(int pos = text.find(L" ", 0); pos != -1; pos = text.find(L" ", pos+1))
			{
				li.text_poses.push_back(pos);
			}

			li.text_poses.push_back(text.size());

			lines.push_back(li);
		}		
	}

	void karaoke::on_mouse_click(int x, int y)
	{
		switch_world();
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
	}

	void karaoke::on_key_pressed(int key)
	{
		switch_line();	
	}
}