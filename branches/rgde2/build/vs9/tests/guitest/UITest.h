#pragma once

#include <guilib.h>
#include <rgde/core/windows.h>
#include <rgde/render/device.h>
#include <rgde/core/file_system.h>

#include <boost/timer.hpp>

namespace rgde
{
	class ui_test_application : public core::windows::window
	{
	public:
		ui_test_application(int x, int y, int w, int h, const std::wstring& title);
		~ui_test_application();

		void run();

		virtual void createGUISystem();
		virtual void resetGUISystem();

		bool handleMouseMove(int x, int y);
		bool handleMouseWheel(int diff);
		bool handleMouseButton(gui::EventArgs::MouseButtons btn, gui::EventArgs::ButtonState state);
		virtual bool handleKeyboard(unsigned int key, gui::EventArgs::ButtonState state);
		bool handleChar(unsigned int);

		void handleViewportChange();

		void load(const std::string& xml);

		virtual bool OnResetDevice();
		virtual void OnLostDevice();

	protected:
		void log(gui::LogLevel level, const std::string& message);
		void reinit();

		void resize_scene(unsigned int width, unsigned int height);
		bool do_events();

		void update(float delta);
		void render();

		virtual long wnd_proc(uint message, uint wparam, long lparam );

	protected:
		std::string m_filename;

		render::device m_render_device;
		core::vfs::system m_filesystem;

		gui::Renderer* m_render;
		gui::System* m_system;

		unsigned int m_framecount;

		bool m_active;
		double m_elapsed;
		boost::timer m_timer;

		gui::FontPtr m_font;
	};
}