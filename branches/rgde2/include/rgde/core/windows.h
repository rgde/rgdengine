#pragma once

#include <rgde/core/windows_forward.h>
#include <rgde/core/types.h>

namespace rgde
{
	namespace core
	{
		namespace windows
		{
			class window
			{
			public:
				struct input_event {
					enum event_type{
						mouse_move,
						mouse_button, 
						mouse_scrool,
						key_pressed, 
						text_input
					};
					enum action_type {
						up, down, move
					};
					// to handle several input devices case or multy touch events
					int device_index; 
					int keycode;
					int value;
					event_type event;
					action_type action;
				};

				explicit window(handle external_handle);
				explicit window(const std::wstring& title = L"Window");
				window(const math::vec2i& pos, const math::vec2i& s, const std::wstring& title = L"Window");
				window(const math::vec2i& pos, const math::vec2i& s, const std::wstring& title, handle parent_handle, 
					ulong style/* = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN*/);
				virtual ~window();

				virtual handle system_handle() const { return m_hwnd; }
				bool is_created() const {return m_hwnd;}				
				
				// set platform dependent state
				bool set_state(int state);
				bool show();
				bool hide();

				bool position(const math::vec2i& p);
				math::vec2i position() const;

				bool size(const math::vec2i& s);
				math::vec2i size() const;

				bool move(const math::vec2i& p, const math::vec2i& s);

				bool update();
				
				bool active() const {return m_active;}

				const std::wstring& title() const {return m_window_title;}
				const std::wstring& class_name() const {return m_class_name;}

			protected:
				bool create(const math::vec2i& pos, const math::vec2i& s, ulong style);
				bool register_class();

				// window event handlers, return 'false' if event wasn't handled
				virtual bool on_activate() {return false;}
				virtual bool on_deactivate() {return false;}
				virtual bool on_resize(int w, int h) {return false;}
				virtual bool on_input_event(const input_event& e) {return false;}

				virtual long wnd_proc(ushort message, uint wparam, long lparam );

			protected:
				handle		m_hwnd;

				std::wstring m_class_name;
				std::wstring m_window_title;

				bool m_active;

				bool m_using_external_handle;
			};

		}
	}
}