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
		bool m_active;
		render::device m_device;
		render::renderer_2d m_batcher;
	};
}