#pragma once

#include <rgde/core/windows_forward.h>
#include <rgde/core/types.h>

namespace rgde
{
	namespace core
	{
		namespace windows
		{
			namespace internal {class impl;}
			class window
			{
				//class impl;
				friend class internal::impl;
			public:
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

			protected:
				bool create(const math::vec2i& pos, const math::vec2i& s, ulong style);

				// window event handlers, return 'false' if event wasn't handled
				virtual bool on_activate() {return false;}
				virtual bool on_deactivate() {return false;}
				virtual bool on_resize(int w, int h) {w;h;return false;}
				virtual bool on_input_event(const input_event& e) {e;return false;}

				virtual long wnd_proc(uint message, uint wparam, long lparam );

			protected:
				internal::impl*	m_impl;
				handle	m_hwnd;

				std::wstring m_window_title;

				bool m_active;

				bool m_using_external_handle;
			};
		}
	}
}