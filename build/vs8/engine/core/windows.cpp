#include "stdafx.h"

#include <engine/core/windows.h>
#include <engine/core/math.h>

namespace rgde
{
	namespace core
	{
		namespace windows
		{
			window::window(const std::wstring& title)
				: m_hwnd(0), m_window_title(title), m_class_name(title + L"_class"),
				  m_parent_hwnd(0), m_instance(0)
			{
				m_instance = GetModuleHandle(NULL);
				if (register_class())
				{
					point p(50,50);
					size s(640, 480);
					create(p,s, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
				}
			}

			window::window(const point& pos, const size& s, const std::wstring& title)
				: m_hwnd(0), m_window_title(title), m_class_name(title + L"_class"),
				m_parent_hwnd(0), m_instance(0) 
			{
				m_instance = GetModuleHandle(NULL);
				if (register_class())
				{
					create(pos,s, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
				}
			}

			window::window(const point& pos, const size& s, const std::wstring& title, handle parent_handle, ulong style)
				: m_hwnd(0), m_window_title(title), m_class_name(title + L"_class"),
				m_parent_hwnd(parent_handle), m_instance(0) 
			{
				m_instance = GetModuleHandle(NULL);
				if (register_class())
				{
					create(pos,s, style);
				}
			}

			window::~window()
			{
				if (0 != m_hwnd)
					DestroyWindow(m_hwnd);

				if (0 != m_instance)
					UnregisterClass(m_class_name.c_str(), m_instance);
			}

			bool window::set_state(int state)
			{
				return (TRUE == ShowWindow(m_hwnd, state ));
			}

			bool window::update()
			{
				return (TRUE == UpdateWindow( m_hwnd ));
			}

			
			size window::get_size() const
			{
				RECT rc;
				GetWindowRect(m_hwnd, &rc);
				return size(rc.right - rc.left, rc.bottom - rc.top);
			}

			point window::get_position() const
			{
				RECT rc;
				GetWindowRect(m_hwnd, &rc);
				return point(rc.left, rc.top);
			}

			bool window::move_window(const point& p, const size& s)
			{
				return TRUE == MoveWindow(m_hwnd, 
					p[0], p[1],
					s[0],
					s[1],
					FALSE);		// repaint window 
			}

			bool window::set_position(const point& p)
			{
				return move_window(p,get_size());
			}

			bool window::set_size(const size& s)
			{
				return move_window(get_position(),s);
			}

			bool window::register_class()
			{
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
			

			bool window::create(const point& pos, const size& s, ulong style)
			{
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


			//void window::move(WORD x,WORD y,WORD xx,WORD yy,bool d){
			//	MoveWindow(hWnd, 
			//		x, y,           // starting x- and y-coordinates 
			//		xx, // width of client area 
			//		yy, // height of client area 
			//		d);          // repaint window 		
			//}

			result window::wnd_proc(ushort message, uint wParam, long lParam )
			{ 
				return DefWindowProc(m_hwnd, message, wParam, lParam );
			}

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