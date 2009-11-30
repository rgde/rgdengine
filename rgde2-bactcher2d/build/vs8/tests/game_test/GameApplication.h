#pragma once

namespace rgde
{
	struct AlienUnit
	{
		AlienUnit();
		float velocity;
		render::primitives_2d::sprite_desc sprite;
	};

	class GameApplication : public core::windows::window
	{
	public:
		typedef std::vector<AlienUnit> aliens_vector;
		typedef aliens_vector::iterator aliens_iter;

		GameApplication(int x, int y, int w, int h, const std::wstring& title);
		~GameApplication();

		void run();
	protected:		
		bool do_events();
		core::windows::result wnd_proc(ushort message, uint wparam, long lparam );
	private:
		void init_game_data();
		void update_frame();
		void render_frame();

		void add_unit(AlienUnit& unit);

		bool m_active;
		render::device m_device;
		render::renderer_2d m_batcher;
		render::font_ptr m_debug_info;

		//textures
		render::texture_ptr m_ship_texture;
		render::texture_ptr m_alien_texture;

		aliens_vector m_aliens;
	};
}