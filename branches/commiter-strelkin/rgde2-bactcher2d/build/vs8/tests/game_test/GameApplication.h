#pragma once

namespace rgde
{
	class GameApplication : public core::windows::window
	{
	public:
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

		bool m_active;
		render::device m_device;
		render::renderer_2d m_batcher;
		render::font_ptr m_debug_info;

		//textures
		render::texture_ptr m_ship_texture;
		render::texture_ptr m_alien_texture;
	};
}