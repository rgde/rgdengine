#include "stdafx.h"

#include <rgde/core/windows.h>
#include <rgde/core/math.h>

namespace rgde
{
	namespace core
	{
		namespace windows
		{
			window::window(handle external_handle)
				: m_hwnd(external_handle)
				, m_parent_hwnd(0)
				, m_instance(0)
				, m_using_external_handle(true)
			{
				m_instance = GetModuleHandle(NULL);
			}

			window::window(const std::wstring& title)
				: m_hwnd(0), m_window_title(title), m_class_name(title + L"_class"),
				  m_parent_hwnd(0), m_instance(0)
				  , m_using_external_handle(false)
			{
				m_instance = GetModuleHandle(NULL);
				if (register_class())
				{
					math::vec2i p(50,50);
					math::vec2i s(640, 480);
					create(p,s, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
				}
			}

			window::window(const math::vec2i& pos, const math::vec2i& s, const std::wstring& title)
				: m_hwnd(0), m_window_title(title), m_class_name(title + L"_class"),
				m_parent_hwnd(0), m_instance(0) 
				, m_using_external_handle(false)
			{
				m_instance = GetModuleHandle(NULL);
				if (register_class())
				{
					create(pos,s, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
				}
			}

			window::window(const math::vec2i& pos, const math::vec2i& s, const std::wstring& title, handle parent_handle, ulong style)
				: m_hwnd(0), m_window_title(title), m_class_name(title + L"_class"),
				m_parent_hwnd(parent_handle), m_instance(0)
				, m_using_external_handle(false)
			{
				m_instance = GetModuleHandle(NULL);
				if (register_class())
				{
					create(pos,s, style);
				}
			}

			window::~window()
			{
				if (0 != m_hwnd && !m_using_external_handle)
					DestroyWindow(m_hwnd);

				if (0 != m_instance && !m_using_external_handle)
					UnregisterClass(m_class_name.c_str(), m_instance);
			}

			bool window::set_state(int state)
			{
				return !m_using_external_handle ? (TRUE == ShowWindow(m_hwnd, state )) : false;
			}

			bool window::update()
			{
				return !m_using_external_handle ? (TRUE == UpdateWindow( m_hwnd )) : true;
			}

			
			math::vec2i window::size() const
			{
				RECT rc;
				GetWindowRect(m_hwnd, &rc);
				return math::vec2i(rc.right - rc.left, rc.bottom - rc.top);
			}

			math::vec2i window::position() const
			{
				RECT rc;
				GetWindowRect(m_hwnd, &rc);
				return math::vec2i(rc.left, rc.top);
			}

			bool window::move(const math::vec2i& p, const math::vec2i& s)
			{
				if (m_using_external_handle)
					return false;

				return TRUE == MoveWindow(m_hwnd, 
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

				m_wc.style = 0;
				m_wc.lpfnWndProc = (WNDPROC)dispatch;
				m_wc.cbClsExtra = 0;
				m_wc.cbWndExtra = 0;
				m_wc.hInstance = m_instance;
				m_wc.hIcon = NULL;
				m_wc.hCursor = LoadCursor(NULL,IDC_ARROW);
				m_wc.hbrBackground = (HBRUSH)( COLOR_WINDOW );
				m_wc.lpszMenuName = 0;//MAKEINTRESOURCE( IDR_MENU1 );
				m_wc.lpszClassName = m_class_name.c_str();

				return (0 != RegisterClass (&m_wc));
				//UnregisterClass(lpClassName,hInstance);
			}
			

			bool window::create(const math::vec2i& pos, const math::vec2i& s, ulong style)
			{
				assert(!m_using_external_handle && "Invalid call CreateWindow while using external window handler!");

				m_hwnd = CreateWindowEx (
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
					m_parent_hwnd,
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

			result window::wnd_proc(ushort message, uint wParam, long lParam )
			{ 
				return DefWindowProc(m_hwnd, message, wParam, lParam );
			}
			
			//std::vector<window*> window::ms_windows;
			std::map<handle, window*>	window::m_map;

			result __stdcall window::dispatch( handle hWnd, ushort message, uint wParam, long lParam )
			{
				std::map<HWND,window*>::iterator it = m_map.find(hWnd);

				if( it == m_map.end() )
				{
					if( message == WM_NCCREATE || message == WM_CREATE )
					{
						window* w = (window*)((CREATESTRUCT*)lParam)->lpCreateParams;
						w->m_hwnd = hWnd;
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
				result r = w->wnd_proc(message,wParam,lParam);

				if( message == WM_DESTROY )
				{
					//SetWindowLongW()
					w->m_hwnd = 0;
					m_map.erase(it);					
				}

				return r;
			}
		}
	}
}