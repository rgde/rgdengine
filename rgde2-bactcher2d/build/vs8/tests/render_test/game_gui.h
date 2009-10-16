#pragma once

namespace game
{
	class karaoke;

	class sprite
	{
		friend karaoke;
		typedef std::list<sprite*>::iterator handle;
	public:
		sprite(const rgde::math::rect& r, karaoke& k);
		~sprite();

		virtual void render() {}

		const rgde::math::rect& get_rect() const {return m_rect;}

		virtual void on_click(){}
		virtual void on_hover(){}

		virtual void update(float dt) {}


		void hide() {m_visible = false;}
		void show() {m_visible = true;}

		bool is_visible() const {return m_visible;}

	protected:
		rgde::math::rect m_rect;
		handle m_handle;
		bool m_visible;

		rgde::math::color m_color;

		karaoke& m_game;
	};

	class score : public sprite
	{
	public:
		score(karaoke& k);

	private:
		void render();

	private:
		rgde::render::font_ptr m_font;
	};

	class mult : public sprite
	{
	public:
		mult(karaoke& k);

	private:
		void render();

	private:
		rgde::render::font_ptr m_font;
	};

	class song_world : public sprite
	{
	public:
		song_world(int line, int world, rgde::math::rect& r, karaoke& k);

		void on_click();
		void update(float dt);
		void render();

		float get_time() const {return m_time;}

	private:
		int m_vert_move_dir;
		float m_vert_move_speed;
		bool m_clicked;
		int m_line;
		int m_world;
		std::wstring m_text;
		float m_time;
		rgde::render::font_ptr m_font;
	};
}