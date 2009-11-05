//TODO: расширить парсинг виндовз сообщений, определить нужную реакцию на основные.
#include "precompiled.h"

#include <rgde/core/application.h>
#include <rgde/core/task.h>

// forms
#include "../forms/window.h"

#include <rgde/base/xml_helpers.h>

#include <windows.h>
#include "../base/exception.h"

#include <rgde/render/manager.h>

#include <boost/filesystem/operations.hpp>


typedef unsigned long dword;

namespace core
{
	static application* gs_pApplication = 0;

	application* application::get()
	{
		return gs_pApplication;
	}

	class application_impl : public forms::Window, 
							public application,
							public event::sender
	{
	protected:
		//cs_object m_cs;
		bool m_active;

		// Message handles
		void on_activate(forms::Message &msg);
		void on_close(forms::Message &msg);
		void on_key_updown(forms::Message &msg);
		void on_mouse(forms::Message &msg);
		void on_size(forms::Message &msg);

		void OnEnterSizeMove(forms::Message &msg);
		void OnExitSizeMove(forms::Message &msg);

		virtual void close()
		{
			m_bIsClosing = true;
		}

		bool CheckForWindowSizeChange();

		void setupHandlers();

		RECT get_rect() const {return m_rcClientOld;}

	public:
		application_impl();

		virtual ~application_impl();

		void init(window_handle hParent);
		void init(std::wstring Name, int Width, int Height, bool Fullscreen, bool resize_enable);

		virtual void run();
		virtual bool update();
		virtual window_handle get_handle() const;
		virtual void add(task_ptr pTask);
	private:
		
		typedef std::list<task_ptr> TaskList;
		TaskList m_tasks;
		bool	m_bIsPaused;
		bool	m_bIsClosing;
		RECT	m_rcClientOld;
	};

    //////////////////////////////////////////////////////////////////////////

	void application_impl::add(task_ptr pTask)
	{
		m_tasks.push_back(pTask);
		m_tasks.sort();
	}

	void application_impl::on_key_updown(forms::Message &msg)
	{
		if (msg.uMsg == WM_KEYDOWN)
			switch(msg.wParam)
			{
				case VK_F8:
					{
						static bool bIsRenderDebugInfo = true;
						::render::TheRenderManager::get().enableVolumes(bIsRenderDebugInfo);
						bIsRenderDebugInfo = !bIsRenderDebugInfo;
						break;
					}

				case VK_PAUSE: 
					{
						// TODO:
						// пока здесь паузятся все таски, но потом 
						// обработчики надо разнести и паузить только
						// апдейты, т.е. здесь надо отсылать эвент
						m_bIsPaused = !m_bIsPaused;
						break; 
					}
			};

		const UINT uMsg = msg.uMsg;
		LPARAM lParam = msg.lParam;
		// Consolidate the keyboard messages and pass them to the app's keyboard callback
		if( uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN || 
			uMsg == WM_KEYUP   || uMsg == WM_SYSKEYUP )
		{
			bool bKeyDown = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
			DWORD dwMask = (1 << 29);
			bool bAltDown = ( (lParam & dwMask) != 0 );
		}
	}

	void application_impl::on_mouse(forms::Message &msg)
	{
        switch (msg.uMsg)
        {
            case WM_MOUSEMOVE:
                this->send_event<mouse_move>(mouse_move(LOWORD(msg.lParam), HIWORD(msg.lParam)));
                break;
            case WM_LBUTTONDOWN:
                this->send_event<mouse_button>(mouse_button(mouse_button::Left, mouse_button::Down));
                break;
            case WM_LBUTTONUP:
                this->send_event<mouse_button>(mouse_button(mouse_button::Left, mouse_button::Up));
                break;
            case WM_LBUTTONDBLCLK:
                this->send_event<mouse_button>(mouse_button(mouse_button::Left, mouse_button::DoubleClick));
                break;
            case WM_MBUTTONDOWN:
                this->send_event<mouse_button>(mouse_button(mouse_button::Middle, mouse_button::Down));
                break;
            case WM_MBUTTONUP:
                this->send_event<mouse_button>(mouse_button(mouse_button::Middle, mouse_button::Up));
                break;
            case WM_MBUTTONDBLCLK:
                this->send_event<mouse_button>(mouse_button(mouse_button::Middle, mouse_button::DoubleClick));
                break;
            case WM_RBUTTONDOWN:
                this->send_event<mouse_button>(mouse_button(mouse_button::Right, mouse_button::Down));
                break;
            case WM_RBUTTONUP:
                this->send_event<mouse_button>(mouse_button(mouse_button::Right, mouse_button::Up));
                break;
            case WM_RBUTTONDBLCLK:
                this->send_event<mouse_button>(mouse_button(mouse_button::Right, mouse_button::DoubleClick));
                break;
            case WM_MOUSEWHEEL:
                this->send_event<mouse_whell>(mouse_whell(HIWORD(msg.wParam)));
                break;
        };
	}

	void application_impl::on_size(forms::Message &msg)
	{
		const UINT uMsg = msg.uMsg;
		LPARAM lParam = msg.lParam;
		WPARAM wParam = msg.wParam;

		if( SIZE_MINIMIZED == wParam )
		{
			m_bIsPaused = true;
		}
		else
		{
			RECT rcCurrentClient;
			GetClientRect( Handle(), &rcCurrentClient );

			if( rcCurrentClient.top == 0 && rcCurrentClient.bottom == 0 )
			{
				// Rapidly clicking the task bar to minimize and restore a window
				// can cause a WM_SIZE message with SIZE_RESTORED when 
				// the window has actually become minimized due to rapid change
				// so just ignore this message
			}
			else if( SIZE_MAXIMIZED == wParam )
			{
				m_bIsPaused = false;
				CheckForWindowSizeChange();
			}
			else if( SIZE_RESTORED == wParam )
			{      
				CheckForWindowSizeChange();
			}
		}
	}
	//------------------------------------------------------------------
	void application_impl::OnEnterSizeMove(forms::Message &msg)
	{
		m_bIsPaused = true;
	}
	//------------------------------------------------------------------
	void application_impl::OnExitSizeMove(forms::Message &msg)
	{
		m_bIsPaused = false;
		CheckForWindowSizeChange();
	}
	//------------------------------------------------------------------
	bool application_impl::CheckForWindowSizeChange()
	{
		//if (inFullScreenMode) return false;
		RECT rcClientOld = m_rcClientOld;

		// update window properties
		RECT rcWindowClient;
		GetClientRect( Handle(), &rcWindowClient );
		m_rcClientOld = rcWindowClient;

		// Check if the window client rect has changed
		if( rcClientOld.right - rcClientOld.left != rcWindowClient.right - rcWindowClient.left ||
			rcClientOld.bottom - rcClientOld.top != rcWindowClient.bottom - rcWindowClient.top )
		{
			m_bIsPaused = true;

			// TODO: Send event with new sizes
			int width = rcWindowClient.right - rcWindowClient.left;
			int height = rcWindowClient.bottom - rcWindowClient.top;
			this->send_event<window_resize>(window_resize(width, height));

			m_bIsPaused = false;
		}

		// TODO: !!!!!!!
		return false;
	}

	application* application::create(const std::wstring& window_name)
	{
		if ( 0 != gs_pApplication) return gs_pApplication;

		std::wstring buffer;
		buffer.resize(256);
		GetCurrentDirectoryW(256, &buffer[0]);

		std::wstring name = window_name.empty() ? L"RGDE Window" : window_name;
		int  nWidth = 640;
		int  nHeight = 480;
		bool bFullscreen = false;

		TiXmlDocument XmlConfig;

		if(!base::load_xml("EngineConfig.xml", XmlConfig))
		{
			base::lwrn << "application::create(): Can't load config file: \"EngineConfig.xml\"";
			base::lwrn << "Using default settings.";			
		}
		else
		{
			TiXmlHandle hConfigHandle(&XmlConfig);
			TiXmlHandle pWindowNode = hConfigHandle.FirstChildElement("Engine").FirstChildElement("Window");//.FirstChildElement().Element();

			std::string ansi_name = base::safe_read<std::string>(pWindowNode, "name", "");
			name = ansi_name.empty() ? name : std::wstring(ansi_name.begin(), ansi_name.end());
			nWidth = base::safe_read<int>(pWindowNode, "width", 640);
			nHeight = base::safe_read<int>(pWindowNode, "height", 480);
			bFullscreen = base::safe_read<bool>(pWindowNode, "Fullscreen", 0);
		}

		return create(std::wstring(name.begin(), name.end()), nWidth, nHeight, bFullscreen);
	}

	application* application::create(const std::wstring& Name, int Width, int Height, bool Fullscreen, bool resize_enable)
	{
		if ( 0 != gs_pApplication)
			return gs_pApplication;

		std::wstring buffer;
		buffer.resize(256);
		//GetCurrentDirectoryW(256, &buffer[0]);

		GetModuleFileNameW(NULL, &buffer[0], 256);
		//GetMonitorInfoW()

		//GetLongPathNameW(`)

		std::wstring module_path;
		module_path.resize(256);
		GetFullPathNameW(&buffer[0], 256, &module_path[0], NULL);

		//using boost::filesystem::path;
		boost::filesystem::wpath p(buffer);

		std::wstring path = p.branch_path().string();

		SetCurrentDirectoryW(path.c_str());
		

		application_impl* pApp = new application_impl();
		pApp->init(Name, Width, Height, Fullscreen, resize_enable);
	
		return pApp;
	}

	application* application::create(window_handle hParentWindow)
	{
		if ( 0 != gs_pApplication)
			return gs_pApplication;

		std::wstring buffer;
		buffer.resize(256);
		GetCurrentDirectoryW(256, &buffer[0]);

		application_impl* pApp = new application_impl();
		pApp->init(hParentWindow);
		
		return pApp;
	}

	application_impl::application_impl() : m_bIsPaused(false), m_bIsClosing(false)
	{
		gs_pApplication = this;
		base::log::init();
	}

	application_impl::~application_impl()
	{
		base::log::destroy();
		gs_pApplication = 0;
	}

	window_handle application_impl::get_handle() const 
	{
		return Handle();
	}

	void application_impl::on_activate(forms::Message &msg)
	{
		if (msg.wParam == WA_ACTIVE)
		{
			GetClientRect(Handle(), &m_rcClientOld);
			m_active = true;

			//RECT rcWindowClient;
			//GetClientRect( Handle(), &rcWindowClient );
			//int width = rcWindowClient.right - rcWindowClient.left;
			//int height = rcWindowClient.bottom - rcWindowClient.top;
			//this->send_event(window_resize(width, height));
		}
		else
		{
			m_active = false;
		} 
	}

	void application_impl::on_close(forms::Message &msg)
	{
		m_bIsClosing = true;
	}

	void application_impl::setupHandlers()
	{
		// Set message handlers
		SetMessageEvent(WM_ACTIVATE,		 boost::bind(&application_impl::on_activate, this, _1));
		SetMessageEvent(WM_CLOSE,			boost::bind(&application_impl::on_close, this, _1));

		// Keyboard
		SetMessageEvent(WM_KEYDOWN,			boost::bind(&application_impl::on_key_updown, this, _1));
		SetMessageEvent(WM_SYSKEYDOWN,		boost::bind(&application_impl::on_key_updown, this, _1));
		SetMessageEvent(WM_KEYUP,			boost::bind(&application_impl::on_key_updown, this, _1));
		SetMessageEvent(WM_SYSKEYUP,		boost::bind(&application_impl::on_key_updown, this, _1));

		// Mouse
		SetMessageEvent(WM_LBUTTONDOWN,		boost::bind(&application_impl::on_mouse, this, _1));
		SetMessageEvent(WM_LBUTTONUP,		boost::bind(&application_impl::on_mouse, this, _1));
		SetMessageEvent(WM_LBUTTONDBLCLK,	boost::bind(&application_impl::on_mouse, this, _1));

		SetMessageEvent(WM_MBUTTONDOWN,		boost::bind(&application_impl::on_mouse, this, _1));
		SetMessageEvent(WM_MBUTTONUP,		boost::bind(&application_impl::on_mouse, this, _1));
		SetMessageEvent(WM_MBUTTONDBLCLK,	boost::bind(&application_impl::on_mouse, this, _1));

		SetMessageEvent(WM_RBUTTONDOWN,		boost::bind(&application_impl::on_mouse, this, _1));
		SetMessageEvent(WM_RBUTTONUP,		boost::bind(&application_impl::on_mouse, this, _1));
		SetMessageEvent(WM_RBUTTONDBLCLK,	boost::bind(&application_impl::on_mouse, this, _1));

		//SetMessageEvent(WM_XBUTTONDOWN,	boost::bind(&application_impl::on_mouse, this, _1));
		//SetMessageEvent(WM_XBUTTONUP,		boost::bind(&application_impl::on_mouse, this, _1));
		//SetMessageEvent(WM_XBUTTONDBLCLK,	boost::bind(&application_impl::on_mouse, this, _1));
		SetMessageEvent(WM_MOUSEWHEEL,	boost::bind(&application_impl::on_mouse, this, _1));

		SetMessageEvent(WM_MOUSEMOVE,		boost::bind(&application_impl::on_mouse, this, _1));

		SetMessageEvent(WM_SIZE,			boost::bind(&application_impl::on_size, this, _1));

		SetMessageEvent(WM_ENTERSIZEMOVE,	boost::bind(&application_impl::OnEnterSizeMove, this, _1));
		SetMessageEvent(WM_EXITSIZEMOVE,	boost::bind(&application_impl::OnExitSizeMove, this, _1));
	}

	void application_impl::init(window_handle hParent)
	{
		// Styles and position
		dword Style;
		dword ExStyle;

		std::wstring Name(L"engine_wnd");

		// Window rectangle
		forms::Drawing::Rectangle Rect;
		GetWindowRect((HWND)hParent, &Rect);

		{
			//WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS
			Style = WS_CHILD | WS_VISIBLE;// | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;// WS_CHILD | WS_OVERLAPPED | WS_BORDER;// | WS_CAPTION | WS_SYSMENU;
			ExStyle = 0;
		}


		//AdjustWindowRect(&Rect, Style, false);

		// Register window class
		//RegisterCls(Name.c_str(), CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, LoadCursor(NULL,IDC_ARROW), (HBRUSH)(COLOR_WINDOW));
		RegisterCls(Name.c_str(), 0/* CS_HREDRAW | CS_VREDRAW | CS_OWNDC*/, NULL, LoadCursor(NULL,IDC_ARROW), (HBRUSH)(COLOR_WINDOW));


		// create window
		{
			forms::Drawing::Rectangle _rect(0, 0, Rect.GetWidth(), Rect.GetHeight());
			CreateWnd((HWND)hParent, Name.c_str(), Name.c_str(), Style, ExStyle, 0, _rect);
		}

		setupHandlers();


		// show window
		::ShowWindow(Handle(), SW_SHOW);
		::UpdateWindow(Handle());
	}

	void application_impl::init(std::wstring Name, int Width, int Height, bool Fullscreen, bool resize_enable)
	{
		// Styles and position
		dword Style;
		dword ExStyle;
		int x, y;

		if (Fullscreen)
		{
			Style = WS_POPUP|WS_SYSMENU;
			Style &= ~WS_MAXIMIZE & ~WS_MINIMIZE;
			ExStyle = WS_EX_TOPMOST;
			x = y = 0;

			//RECT DesktopSize;
			//GetClientRect(GetDesktopWindow(),&DesktopSize);
		}
		else
		{
			Style = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

			//if (resize_enable)
			//	// | WS_SIZEBOX ;//WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU;
			//	Style = WS_OVERLAPPEDWINDOW;
			//else
			//	Style = WS_CAPTION | WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX;

			ExStyle = 0;
			x = (GetSystemMetrics(SM_CXSCREEN) / 2) - (Width / 2);
			y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (Height / 2);
		}

		// Window rectangle
		forms::Drawing::Rectangle Rect(y, x, x + Width, y + Height);
		
		/*if (!Fullscreen)
			AdjustWindowRect(&Rect, Style, false);*/
		
		// Register window class
		//RegisterCls(Name.c_str(), CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, LoadCursor(NULL,IDC_ARROW), (HBRUSH)(COLOR_WINDOW));
		
		
		//RegisterCls(Name.c_str(), CS_HREDRAW | CS_VREDRAW, NULL, LoadCursor(NULL,IDC_ARROW), (HBRUSH)(COLOR_WINDOW));
		RegisterCls(Name.c_str(), NULL, NULL, LoadCursor(NULL,IDC_ARROW), (HBRUSH)(COLOR_WINDOW));

		// create window
		CreateWnd(NULL, Name.c_str(), Name.c_str(), WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN/*Style*/, 0/*ExStyle*/, 0, Rect);

		// show window
		::ShowWindow(Handle(), SW_SHOW);
		::UpdateWindow(Handle());

		WINDOWPLACEMENT wpFullscreen;
		ZeroMemory( &wpFullscreen, sizeof(WINDOWPLACEMENT) );
		wpFullscreen.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement( Handle(), &wpFullscreen );
		if( (wpFullscreen.flags & WPF_RESTORETOMAXIMIZED) != 0 )
		{
			// Restore the window to normal if the window was maximized then minimized.  This causes the 
			// WPF_RESTORETOMAXIMIZED flag to be set which will cause SW_RESTORE to restore the 
			// window from minimized to maxmized which isn't what we want
			wpFullscreen.flags &= ~WPF_RESTORETOMAXIMIZED;
			wpFullscreen.showCmd = SW_RESTORE;
			SetWindowPlacement( Handle(), &wpFullscreen );
		}

		// Set message handlers
		setupHandlers();

		//// show window
		//::ShowWindow(Handle(), SW_SHOW);
		//::UpdateWindow(Handle());
	}

	bool application_impl::update()
	{
		{
			bool MessagePumpActive = true;
			MSG msg;

			if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{
					MessagePumpActive = false;
				}
				else
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
			}
			else
			{
				//if (!m_active)
				//{
				//	WaitMessage();
				//}
				//else
				{
					if (!m_bIsPaused)
						for (TaskList::iterator it = m_tasks.begin(); it != m_tasks.end(); ++it)
						{							
							(*it)->execute();							
						}
				}
			}
			return MessagePumpActive;
		}
	}

	void application_impl::run()
	{
		while (update() && !m_bIsClosing)
		{
		}

		// destroy all tasks
		//m_tasks.clear();

		// send device destroy reset
		//PostQuitMessage(0);		
	}
}