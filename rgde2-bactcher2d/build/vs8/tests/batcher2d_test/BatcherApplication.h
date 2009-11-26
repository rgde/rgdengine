#pragma once

namespace rgde
{
	class BatcherApplication : public core::windows::window
	{
	public:
		BatcherApplication(int x, int y, int w, int h, const std::wstring& title);
		~BatcherApplication();

		void run();
		void add_sprite(render::primitives_2d::sprite_desc& sprite);
	protected:
		virtual core::windows::result wnd_proc(ushort message, uint wparam, long lparam );
		bool do_events();
		void render_frame();
		void init_render_data();
	private:
		bool m_active;
		render::device m_device;
		render::renderer_2d* m_batcher;
		std::vector<render::primitives_2d::sprite_desc*> sprites;
	};
}