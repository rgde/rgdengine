#include "stdafx.h"
#include <rgde/core/windows.h>

static	std::map<HWND, rgde::core::windows::internal::impl*>	m_map;
LRESULT CALLBACK dispatch_proc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

namespace rgde {
namespace core {
namespace windows {

	namespace internal{	

		class impl
		{
		public:
			impl(window& wnd, HWND external, const wchar_t* title)
			: owner(wnd), m_external_hwnd(external), m_class_name(title),
			m_hwnd(0), m_window_title(title)
			{
				if (!m_external_hwnd) {
					m_class_name += L"_class";
					math::vec2i p(50,50);
					math::vec2i s(640, 480);
					create(p,s, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
				}

				owner.m_hwnd.vp = m_hwnd;
			}

			~impl()	{
				owner.m_hwnd.vp = 0;

				HINSTANCE instance = GetModuleHandle(NULL);

				if (!m_external_hwnd) {
					if (0 != m_hwnd) DestroyWindow(m_hwnd);
					//if (instance) 
					UnregisterClassW(m_class_name.c_str(), instance);
				}
			}

			bool create(const math::vec2i& pos, const math::vec2i& s, ulong style)
			{
				assert(!m_external_hwnd && "Invalid call CreateWindow while using external window handler!");
			
				HINSTANCE instance = GetModuleHandle(NULL);

				WNDCLASS wc = {0};				
				wc.lpfnWndProc = (WNDPROC)dispatch_proc;				
				wc.hInstance = instance;
				wc.hCursor = LoadCursor(NULL,IDC_ARROW);
				wc.hbrBackground = (HBRUSH)( COLOR_WINDOW );
				wc.lpszClassName = m_class_name.c_str();
				RegisterClassW (&wc);

				m_hwnd = CreateWindowExW (
					0,
					m_class_name.c_str(),
					m_window_title.c_str(),
					style,
					pos[0],	pos[1],
					s[0], s[1],
					NULL, NULL,
					instance,/* hInstance: Windows NT/2000/XP: This value is ignored. */
					this
					); 

				if( !m_hwnd )
				{
					LPVOID lpMsgBuf;
					DWORD last_err  = GetLastError();
					if(last_err)
					{
						FormatMessage( 
							FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
							FORMAT_MESSAGE_IGNORE_INSERTS,NULL,last_err, 0, (LPTSTR) &lpMsgBuf,0,NULL );			
						// Free the buffer.
						MessageBoxW( NULL, (LPCTSTR)lpMsgBuf, L"Error", MB_OK | MB_ICONINFORMATION );
						LocalFree( lpMsgBuf );
					}		
				}	

				owner.m_hwnd.vp = m_hwnd;
				return m_hwnd != 0;
			}


			LRESULT wnd_proc( uint message, uint wparam, long lparam )
			{
				return owner.wnd_proc(message, wparam, lparam );
			}

			window& owner;
			HWND m_hwnd;
			HWND m_external_hwnd;
			std::wstring m_class_name;
			std::wstring m_window_title;
		};
	}

	using namespace internal;

	window::window(handle external_handle)
		: m_hwnd(external_handle)
		, m_using_external_handle(true)
		, m_active(false)
	{
		m_impl = new impl(*this, (HWND)m_hwnd.vp, L"must_never_be_used");
	}

	window::window(const std::wstring& title)
		: m_window_title(title)
		, m_using_external_handle(false)
		, m_active(false)				  
	{
		m_impl = new impl(*this, 0, m_window_title.c_str());
	}

	window::window(const math::vec2i& pos, const math::vec2i& s, const std::wstring& title)
		: m_window_title(title)
		, m_using_external_handle(false)
		, m_active(false)
	{
		m_impl = new impl(*this, 0, m_window_title.c_str());
	}

	window::window(const math::vec2i& pos, const math::vec2i& s, const std::wstring& title, handle parent_handle, ulong style)
		: m_window_title(title)
		, m_using_external_handle(false)
		, m_active(false)
	{	
		m_impl = new impl(*this, 0, m_window_title.c_str());
	}

	window::~window()
	{
		delete m_impl;
	}

	bool window::set_state(int state)
	{
		return !m_using_external_handle ? (TRUE == ShowWindow((HWND)m_hwnd.vp, state )) : false;
	}

	bool window::update()
	{
		return !m_using_external_handle ? (TRUE == UpdateWindow( (HWND)m_hwnd.vp )) : true;
	}


	math::vec2i window::size() const
	{
		RECT rc;
		GetWindowRect((HWND)m_hwnd.vp, &rc);
		return math::vec2i(rc.right - rc.left, rc.bottom - rc.top);
	}

	math::vec2i window::position() const
	{
		RECT rc;
		GetWindowRect((HWND)m_hwnd.vp, &rc);
		return math::vec2i(rc.left, rc.top);
	}

	bool window::move(const math::vec2i& p, const math::vec2i& s)
	{
		if (m_using_external_handle)
			return false;

		return TRUE == MoveWindow((HWND)m_hwnd.vp, 
			p[0], p[1],
			s[0],
			s[1],
			FALSE);		// repaint window 
	}

	bool window::position(const math::vec2i& p)
	{
		if (m_using_external_handle)
			return false;

		return move(p,size());
	}

	bool window::size(const math::vec2i& s)
	{
		if (m_using_external_handle)
			return false;

		return move(position(),s);
	}

	bool window::show() 
	{
		return set_state(SW_SHOW);
	}

	bool window::hide() 
	{
		return set_state(SW_HIDE);
	}

	long window::wnd_proc(uint message, uint wparam, long lparam )
	{ 
		switch (message)
		{
		case WM_ACTIVATE:	// Watch For Window Activate Message
			{
				// Check Minimization State
				m_active = !HIWORD(wparam);
				bool was_handled = m_active ? on_activate() : on_deactivate();
				if (was_handled)
					return 0;
			}
			break;
		case WM_SIZE:
			if (on_resize(LOWORD(lparam), HIWORD(lparam)))
				return 0;
			break;
		}
		return DefWindowProc((HWND)m_hwnd.vp, message, wparam, lparam );
	}

}
}
}

LRESULT CALLBACK dispatch_proc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	using namespace rgde;
	std::map<HWND,core::windows::internal::impl*>::iterator it = m_map.find(hWnd);

	core::windows::internal::impl *w = 0;

	if( it == m_map.end() )
	{
		if( message == WM_NCCREATE || message == WM_CREATE )
		{
			w = (core::windows::internal::impl*)((CREATESTRUCT*)lParam)->lpCreateParams;
			w->m_hwnd = hWnd;
			std::pair<HWND,core::windows::internal::impl*> value(hWnd, w);
			m_map.insert(value);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	else 
	{
		w = it->second;
	}

	assert(w && "Invalid windows pointer!");

	long r = w->wnd_proc(message,wParam,lParam);

	if( message == WM_DESTROY )
	{
		w->m_hwnd = 0;
		m_map.erase(it);
	}

	return (LRESULT)r;
}