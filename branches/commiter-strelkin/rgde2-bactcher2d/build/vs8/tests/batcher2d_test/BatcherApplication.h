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
		render::device m_device;
	};
}