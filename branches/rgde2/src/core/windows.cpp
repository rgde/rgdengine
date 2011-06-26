#include "stdafx.h"

#include <rgde/core/windows.h>
#include <rgde/core/math.h>

#include <windows.h>

namespace rgde
{
	namespace core
	{
		namespace windows
		{
			static LRESULT __stdcall dispatch( HWND hWnd, ushort message, uint wParam, long lParam );
			static	std::map<HWND, window*>	m_map;

			window::window(handle external_handle)
				: m_hwnd(external_handle)
				, m_using_external_handle(true)
				, m_active(false)
			{
			}

			window::window(const std::wstring& title)
				: m_window_title(title)
				, m_class_name(title + L"_class")
				, m_using_external_handle(false)
				, m_active(false)				  
			{
				m_hwnd.vp = 0;

				if (register_class())
				{
					math::vec2i p(50,50);
					math::vec2i s(640, 480);
					create(p,s, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
				}
			}

			window::window(const math::vec2i& pos, const math::vec2i& s, const std::wstring& title)
				: m_window_title(title)
				, m_class_name(title + L"_class")
				, m_using_external_handle(false)
				, m_active(false)
			{
				m_hwnd.vp = 0;

				if (register_class())
				{
					create(pos,s, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
				}
			}

			window::window(const math::vec2i& pos, const math::vec2i& s, const std::wstring& title, handle parent_handle, ulong style)
				: m_window_title(title)
				, m_class_name(title + L"_class")
				, m_using_external_handle(false)
				, m_active(false)
			{	
				m_hwnd.vp = 0;

				if (register_class())
				{
					create(pos,s, style);
				}
			}

			window::~window()
			{
				if (0 != m_hwnd && !m_using_external_handle)
					DestroyWindow((HWND)m_hwnd.vp);

				HINSTANCE instance = GetModuleHandle(NULL);

				if (instance && !m_using_external_handle)
					UnregisterClass(m_class_name.c_str(), instance);
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

			bool window::register_class()
			{
				assert(!m_using_external_handle && "Invalid call RegisterClass while using external window handler!");

				HINSTANCE instance = GetModuleHandle(NULL);

				WNDCLASS wc = {0};				
				wc.lpfnWndProc = (WNDPROC)dispatch;				
				wc.hInstance = instance;
				wc.hCursor = LoadCursor(NULL,IDC_ARROW);
				wc.hbrBackground = (HBRUSH)( COLOR_WINDOW );
				wc.lpszClassName = m_class_name.c_str();

				return (0 != RegisterClass (&wc));
			}
			

			bool window::create(const math::vec2i& pos, const math::vec2i& s, ulong style)
			{
				assert(!m_using_external_handle && "Invalid call CreateWindow while using external window handler!");

				m_hwnd.vp = CreateWindowEx (
					0,
					m_class_name.c_str(),
					m_window_title.c_str(),
					style,
					pos[0],//CW_USEDEFAULT,//0
					pos[1],//CW_USEDEFAULT,//0
					s[0],
					s[1],
					//s[0]/*640*/+2*GetSystemMetrics(SM_CXSIZEFRAME),
					//s[1]/*480*/+2*GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION),
					NULL,
					NULL,
					NULL,/* hInstance: Windows NT/2000/XP: This value is ignored. */
					this);

				if( !m_hwnd )
				{
					//LPVOID lpMsgBuf;
					//DWORD last_err  = GetLastError();
					//if(last_err)
					//{
					//	//FormatMessage( 
					//	//	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
					//	//	FORMAT_MESSAGE_IGNORE_INSERTS,NULL,last_err, 0, (LPTSTR) &lpMsgBuf,0,NULL );			
					//	//// Free the buffer.
					//	//MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
					//	//LocalFree( lpMsgBuf );
					//}		
					return false;
				}	
				return true;
			}

			bool window::show() 
			{
				return set_state(SW_SHOW);
			}

			bool window::hide() 
			{
				return set_state(SW_HIDE);
			}

			long window::wnd_proc(ushort message, uint wParam, long lParam )
			{ 
				switch ()
				{
				case WM_ACTIVATE:	// Watch For Window Activate Message
					{
						// Check Minimization State
						m_active = !HIWORD(wparam);
						bool was_handled = m_active ? on_activate() : on_deactive();
						if (was_handled)
							return 0;
					}
					break;
				case WM_SIZE:
					if (on_resize(LOWORD(lparam), HIWORD(lparam)))
						return 0;
					break;
				}
				return DefWindowProc((HWND)m_hwnd.vp, message, wParam, lParam );
			}
			
			//std::vector<window*> window::ms_windows;
			//std::map<handle, window*>	window::m_map;

			LRESULT __stdcall dispatch( HWND hWnd, ushort message, uint wParam, long lParam )
			{
				std::map<HWND,window*>::iterator it = m_map.find(hWnd);

				if( it == m_map.end() )
				{
					if( message == WM_NCCREATE || message == WM_CREATE )
					{
						window* w = (window*)((CREATESTRUCT*)lParam)->lpCreateParams;
						w->m_hwnd.vp = hWnd;
						std::pair<HWND,window*> value(hWnd, w);
						m_map.insert(value);
						it = m_map.find(hWnd);
					}

					///* Bred */
					if( message == WM_GETMINMAXINFO ) 
						return DefWindowProc(hWnd, message, wParam, lParam);
				}

				if(it == m_map.end())
					return DefWindowProc(hWnd, message, wParam, lParam);

				window *w = it->second;
				long r = w->wnd_proc(message,wParam,lParam);

				if( message == WM_DESTROY )
				{
					//SetWindowLongW()
					w->m_hwnd.vp = 0;
					m_map.erase(it);					
				}

				return (LRESULT)r;
			}
		}
	}
}