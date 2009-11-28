#pragma once

namespace rgde
{
	class BatcherApplication : public core::windows::window
	{
	public:
		typedef render::primitives_2d::sprite_desc sprite_desc;

		BatcherApplication(int x, int y, int w, int h, const std::wstring& title);
		~BatcherApplication();

		void run();
		void add_sprite(const sprite_desc& sprite);

	protected:		
		bool do_events();
		void render_frame();
		void init_render_data();

		core::windows::result wnd_proc(ushort message, uint wparam, long lparam );

	private:
		bool m_active;
		render::device m_device;
		render::renderer_2d m_batcher;
		render::texture_ptr tex;
		
		// not used:
		//std::vector<render::primitives_2d::sprite_desc*> sprites; 
	};
}