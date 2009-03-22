#include "stdafx.h"
#include "karaoke.h"
#include "Application.h"

using namespace rgde;

namespace game
{
	karaoke::karaoke(Application& app)
		: m_app(app)
	{
		m_font = render::font::create(m_app.get_render_device(), 30, L"Arial", render::font::heavy);
		load_game_data();

		cur_symbol = 0;
		cur_line = 0;
	}

	void karaoke::render()
	{
		xml::node l = doc("Song")("Text")("Line");
		for(int i = 1; i < cur_line && l.next_sibling("Line"); ++i, l = l.next_sibling("Line"));
		const std::wstring text = xml::as_utf16(l.first_child().value());

		render_text_line(text, 
			std::min(cur_symbol, text.size()), 
			math::rect(10,10,620,100), 
			math::color::Red);
	}

	void karaoke::render_text_line(std::wstring text, size_t pos, rgde::math::rect& r, 
									rgde::math::color c)
	{
		std::wstring part_text = std::wstring(text.begin(), text.begin() + pos);

		math::rect text_rect = m_font->measure_test(text);
		
		math::rect new_rect = r;
		new_rect.x = (r.w - r.x)/2 - text_rect.w/2;

		const math::color shadow_color(c * 0.5f, 1.0f);

		m_font->render(text, new_rect, shadow_color, true);
		m_font->render(part_text,new_rect,c, true);
	}

	void karaoke::update(float dt)
	{

	}

	void karaoke::load_game_data()
	{
		doc.load_file("sample.xml");
	}

	void karaoke::on_mouse_click(int x, int y)
	{
		cur_symbol++;
	}

	void karaoke::on_key_pressed(int key)
	{
		cur_symbol = 0;
		cur_line++;		
	}
}