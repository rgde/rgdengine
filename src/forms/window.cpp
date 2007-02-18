#include "precompiled.h"
// Forms
#include "window.h"
#include "drawing.h"

#pragma warning(disable: 4311) // warning C4311: 'type cast' : pointer truncation from '' to ''
#pragma warning(disable: 4312) // warning C4312: 'type cast' : conversion from '' to '' of greater size

namespace Forms
{
	template <typename T>
	void SetWndLong(HWND hWnd, int index, const T &value)
	{
		::SetWindowLong(hWnd, index, reinterpret_cast<LONG>(value));
	}

	template <typename T>
	T GetWndLong(HWND hWnd, int index)
	{
		return reinterpret_cast<T>(::GetWindowLong(hWnd, index));
	}

	LRESULT CALLBACK Window::MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Message msg(uMsg, wParam, lParam);

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT *cs= (CREATESTRUCT *)(lParam);
			SetWndLong<void*>(hWnd, GWL_USERDATA, cs->lpCreateParams);
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}

		Window *wnd	= GetWndLong<Window*>(hWnd, GWL_USERDATA);

		if (wnd)
		{
			switch (uMsg)
			{
			case WM_COMMAND:
				{
					MessageT<WM_COMMAND> command_msg(msg);
					if (!wnd->OnCommand(command_msg))
					{
						if (Window * subwnd = GetWndLong<Window*>(command_msg.Handle(), GWL_USERDATA))
						{
							subwnd->OnCommand(command_msg);
						}
					}
				}
				break;

			case WM_NOTIFY:
				{
					MessageT<WM_NOTIFY> notify_msg(msg);
					if (!wnd->OnNotify(notify_msg))
					{
						if (Window * subwnd = GetWndLong<Window*>(notify_msg.Handle(), GWL_USERDATA))
						{
							subwnd->OnNotify(notify_msg);
						}
					}
				}
				break;

			default:
				{
					wnd->MessageDispatcher(msg);
				}
			}
		}

		return msg.lResult;
	}

	LRESULT CALLBACK Window::SubWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Message msg(uMsg, wParam, lParam);

		Window *wnd	= GetWndLong<Window*>(hWnd, GWL_USERDATA);
		if (wnd)
		{
			wnd->MessageDispatcher(msg);
		}

		return msg.lResult;
	}

	void Window::MessageDispatcher(Message &msg)
	{
		MessageEventMap::iterator it;
		if ((it = m_MessageEventMap.find(msg.uMsg)) != m_MessageEventMap.end())
		{
			it->second(msg);
		}
		else
		{
			msg.lResult = ::CallWindowProc(m_SuperWindowProc, m_hWnd, msg.uMsg, msg.wParam, msg.lParam);
		}
	}

	bool Window::OnCommand(MessageT<WM_COMMAND> &msg)
	{
		return false;
	}

	bool Window::OnNotify(MessageT<WM_NOTIFY> &msg)
	{
		return false;
	}

	Window::Window()
		: m_hWnd(NULL),
		  m_SuperWindowProc(::DefWindowProc)
	{
	}

	Window::~Window()
	{
		if (m_hWnd)
		{
			::DestroyWindow(m_hWnd);
		}
	}

	void Window::RegisterCls(const std::wstring ClassName, UINT Style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, const std::wstring MenuName, HICON hIconSmall)
	{
		WNDCLASSEX wndclass	=
		{
			sizeof(WNDCLASSEX), Style, MainWindowProc, 0, 0, ::GetModuleHandle(NULL), hIcon, hCursor, hbrBackground, MenuName.c_str(), ClassName.c_str(), hIconSmall
		};
		::RegisterClassEx(&wndclass);
	}

	void Window::CreateWnd(HWND Parent, const std::wstring ClassName, const std::wstring Name, DWORD Style, DWORD ExStyle, UINT ID, const Drawing::Rectangle &Rect)
	{
		m_hWnd = ::CreateWindowEx(ExStyle, ClassName.c_str(), Name.c_str(), Style, Rect.left, Rect.top, Rect.GetWidth(), Rect.GetHeight(), Parent, (HMENU)ID, ::GetModuleHandle(NULL), this);
	}

	void Window::Subclass(Window *wnd)
	{
		SetWndLong<void*>(wnd->m_hWnd, GWL_USERDATA, wnd);
		wnd->m_SuperWindowProc = GetWndLong<WNDPROC>(wnd->m_hWnd, GWL_WNDPROC);
		SetWndLong<WNDPROC>(wnd->m_hWnd, GWL_WNDPROC, SubWindowProc);
	}

	void Window::SetMessageEvent(UINT uMsg, const MessageEvent &Event)
	{
		m_MessageEventMap[uMsg] = Event;
	}
}