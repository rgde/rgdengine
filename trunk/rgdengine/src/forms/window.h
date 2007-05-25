#pragma once

// Windows
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// STL
#include <map>

// Base
#define BOOST_FUNCTION_MAX_ARGS 3
#include <boost/function.hpp>

// Forms
#include "message.h"
#include "drawing.h"

namespace Forms
{
	class Window
	{
	public:
		typedef boost::function<void(Message&)> MessageEvent;

	private:
		HWND m_hWnd;
		WNDPROC m_SuperWindowProc;
		typedef std::map<UINT, MessageEvent> MessageEventMap;
		MessageEventMap m_MessageEventMap;

		// Window function for windows
		static LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		// Window function for sub windows
		static LRESULT CALLBACK SubWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		// Message dispatcher function
		void MessageDispatcher(Message &msg);

	protected:
		virtual bool OnCommand(MessageT<WM_COMMAND> &msg);
		virtual bool OnNotify(MessageT<WM_NOTIFY> &msg);

	public:
		Window();
		virtual ~Window();

		HWND Handle() const {return m_hWnd;}

		void RegisterCls(const std::wstring ClassName, UINT Style, HICON hIcon, HCURSOR hCursor, HBRUSH hbrBackground, const std::wstring MenuName = std::wstring(), HICON hIconSmall = NULL);
		void CreateWnd(HWND Parent, const std::wstring ClassName, const std::wstring Name, DWORD Style, DWORD ExStyle, UINT ID, const Drawing::Rectangle &Rect);
		void Subclass(Window *wnd);
		void SetMessageEvent(UINT uMsg, const MessageEvent &Event);
	};
}