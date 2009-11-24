#pragma once

namespace rgde
{
	class BatcherApplication : public core::windows::window
	{
	public:
		BatcherApplication(int x, int y, int w, int h, const std::wstring& title);
		~BatcherApplication();

		void run();
	protected:
		virtual core::windows::result wnd_proc(ushort message, uint wparam, long lparam );
		bool do_events();
	private:
		bool m_active;
		math::color m_clear_color;
		render::device m_device;
	};
}