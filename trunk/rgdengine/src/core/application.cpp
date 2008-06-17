//TODO: расширить парсинг виндовз сообщений, определить нужную реакцию на основные.
#include "precompiled.h"

#include <rgde/core/application.h>
#include <rgde/core/Task.h>

// Forms
#include "../forms/window.h"

#include <rgde/base/xml_helpers.h>

#include <windows.h>
#include "../base/exception.h"

#include <rgde/render/renderManager.h>

#include <boost/filesystem/operations.hpp>


typedef unsigned long dword;

namespace core
{
	static IApplication* gs_pApplication = 0;

	IApplication* IApplication::Get()
	{
		return gs_pApplication;
	}

	class ApplicationImpl : public Forms::Window, 
							public IApplication,
							public event::Sender
	{
	protected:
		//cs_object m_cs;
		bool m_Active;

		// Message handles
		void OnActivate(Forms::Message &msg);
		void OnClose(Forms::Message &msg);
		void OnKeyUpDown(Forms::Message &msg);
		void OnMouse(Forms::Message &msg);
		void OnSize(Forms::Message &msg);

		void OnEnterSizeMove(Forms::Message &msg);
		void OnExitSizeMove(Forms::Message &msg);

		virtual void close()
		{
			m_bIsClosing = true;
		}

		bool CheckForWindowSizeChange();

		void setupHandlers();

		RECT getRect() const {return m_rcClientOld;}

	public:
		ApplicationImpl();

		virtual ~ApplicationImpl();

		void init(WindowHandle hParent);
		void init(std::wstring Name, int Width, int Height, bool Fullscreen, bool resize_enable);

		virtual void Run();
		virtual bool update();
		virtual WindowHandle getWindowHandle() const;
		virtual void addTask(PTask pTask);
	private:
		
		typedef std::list<PTask> TaskList;
		TaskList m_tasks;
		bool	m_bIsPaused;
		bool	m_bIsClosing;
		RECT	m_rcClientOld;
	};

    //////////////////////////////////////////////////////////////////////////

	void ApplicationImpl::addTask(PTask pTask)
	{
		m_tasks.push_back(pTask);
		m_tasks.sort();
	}

	void ApplicationImpl::OnKeyUpDown(Forms::Message &msg)
	{
		if (msg.uMsg == WM_KEYDOWN)
			switch(msg.wParam)
			{
				case VK_F8:
					{
						static bool bIsRenderDebugInfo = true;
						::render::TheRenderManager::Get().enableVolumes(bIsRenderDebugInfo);
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

	void ApplicationImpl::OnMouse(Forms::Message &msg)
	{
        switch (msg.uMsg)
        {
            case WM_MOUSEMOVE:
                this->send_event<CCursorMove>(CCursorMove(LOWORD(msg.lParam), HIWORD(msg.lParam)));
                break;
            case WM_LBUTTONDOWN:
                this->send_event<CMouseButton>(CMouseButton(CMouseButton::Left, CMouseButton::Down));
                break;
            case WM_LBUTTONUP:
                this->send_event<CMouseButton>(CMouseButton(CMouseButton::Left, CMouseButton::Up));
                break;
            case WM_LBUTTONDBLCLK:
                this->send_event<CMouseButton>(CMouseButton(CMouseButton::Left, CMouseButton::DoubleClick));
                break;
            case WM_MBUTTONDOWN:
                this->send_event<CMouseButton>(CMouseButton(CMouseButton::Middle, CMouseButton::Down));
                break;
            case WM_MBUTTONUP:
                this->send_event<CMouseButton>(CMouseButton(CMouseButton::Middle, CMouseButton::Up));
                break;
            case WM_MBUTTONDBLCLK:
                this->send_event<CMouseButton>(CMouseButton(CMouseButton::Middle, CMouseButton::DoubleClick));
                break;
            case WM_RBUTTONDOWN:
                this->send_event<CMouseButton>(CMouseButton(CMouseButton::Right, CMouseButton::Down));
                break;
            case WM_RBUTTONUP:
                this->send_event<CMouseButton>(CMouseButton(CMouseButton::Right, CMouseButton::Up));
                break;
            case WM_RBUTTONDBLCLK:
                this->send_event<CMouseButton>(CMouseButton(CMouseButton::Right, CMouseButton::DoubleClick));
                break;
            case WM_MOUSEWHEEL:
                this->send_event<CMouseWhell>(CMouseWhell(HIWORD(msg.wParam)));
                break;
        };
	}

	void ApplicationImpl::OnSize(Forms::Message &msg)
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
	void ApplicationImpl::OnEnterSizeMove(Forms::Message &msg)
	{
		m_bIsPaused = true;
	}
	//------------------------------------------------------------------
	void ApplicationImpl::OnExitSizeMove(Forms::Message &msg)
	{
		m_bIsPaused = false;
		CheckForWindowSizeChange();
	}
	//------------------------------------------------------------------
	bool ApplicationImpl::CheckForWindowSizeChange()
	{
		//if (inFullScreenMode) return false;
		RECT rcClientOld = m_rcClientOld;

		// Update window properties
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
			this->send_event<CWindowResize>(CWindowResize(width, height));

			m_bIsPaused = false;
		}

		// TODO: !!!!!!!
		return false;
	}

	IApplication* IApplication::Create(const std::wstring& window_name)
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

		if(!base::loadXml("EngineConfig.xml", XmlConfig))
		{
			base::lwrn << "IApplication::Create(): Can't load config file: \"EngineConfig.xml\"";
			base::lwrn << "Using default settings.";			
		}
		else
		{
			TiXmlHandle hConfigHandle(&XmlConfig);
			TiXmlHandle pWindowNode = hConfigHandle.FirstChildElement("Engine").FirstChildElement("Window");//.FirstChildElement().Element();

			std::string ansi_name = base::safeReadValue<std::string>(pWindowNode, "name", "");
			name = ansi_name.empty() ? name : std::wstring(ansi_name.begin(), ansi_name.end());
			nWidth = base::safeReadValue<int>(pWindowNode, "width", 640);
			nHeight = base::safeReadValue<int>(pWindowNode, "height", 480);
			bFullscreen = base::safeReadValue<bool>(pWindowNode, "Fullscreen", 0);
		}

		return Create(std::wstring(name.begin(), name.end()), nWidth, nHeight, bFullscreen);
	}

	IApplication* IApplication::Create(const std::wstring& Name, int Width, int Height, bool Fullscreen, bool resize_enable)
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
		

		ApplicationImpl* pApp = new ApplicationImpl();
		pApp->init(Name, Width, Height, Fullscreen, resize_enable);
	
		return pApp;
	}

	IApplication* IApplication::Create(WindowHandle hParentWindow)
	{
		if ( 0 != gs_pApplication)
			return gs_pApplication;

		std::wstring buffer;
		buffer.resize(256);
		GetCurrentDirectoryW(256, &buffer[0]);

		ApplicationImpl* pApp = new ApplicationImpl();
		pApp->init(hParentWindow);
		
		return pApp;
	}

	ApplicationImpl::ApplicationImpl() : m_bIsPaused(false), m_bIsClosing(false)
	{
		gs_pApplication = this;
		base::log::init();
	}

	ApplicationImpl::~ApplicationImpl()
	{
		base::log::destroy();
		gs_pApplication = 0;
	}

	WindowHandle ApplicationImpl::getWindowHandle() const 
	{
		return Handle();
	}

	void ApplicationImpl::OnActivate(Forms::Message &msg)
	{
		if (msg.wParam == WA_ACTIVE)
		{
			GetClientRect(Handle(), &m_rcClientOld);
			m_Active = true;

			//RECT rcWindowClient;
			//GetClientRect( Handle(), &rcWindowClient );
			//int width = rcWindowClient.right - rcWindowClient.left;
			//int height = rcWindowClient.bottom - rcWindowClient.top;
			//this->sendEvent(CWindowResize(width, height));
		}
		else
		{
			m_Active = false;
		} 
	}

	void ApplicationImpl::OnClose(Forms::Message &msg)
	{
		m_bIsClosing = true;
	}

	void ApplicationImpl::setupHandlers()
	{
		// Set message handlers
		SetMessageEvent(WM_ACTIVATE,		 boost::bind(&ApplicationImpl::OnActivate, this, _1));
		SetMessageEvent(WM_CLOSE,			boost::bind(&ApplicationImpl::OnClose, this, _1));

		// Keyboard
		SetMessageEvent(WM_KEYDOWN,			boost::bind(&ApplicationImpl::OnKeyUpDown, this, _1));
		SetMessageEvent(WM_SYSKEYDOWN,		boost::bind(&ApplicationImpl::OnKeyUpDown, this, _1));
		SetMessageEvent(WM_KEYUP,			boost::bind(&ApplicationImpl::OnKeyUpDown, this, _1));
		SetMessageEvent(WM_SYSKEYUP,		boost::bind(&ApplicationImpl::OnKeyUpDown, this, _1));

		// Mouse
		SetMessageEvent(WM_LBUTTONDOWN,		boost::bind(&ApplicationImpl::OnMouse, this, _1));
		SetMessageEvent(WM_LBUTTONUP,		boost::bind(&ApplicationImpl::OnMouse, this, _1));
		SetMessageEvent(WM_LBUTTONDBLCLK,	boost::bind(&ApplicationImpl::OnMouse, this, _1));

		SetMessageEvent(WM_MBUTTONDOWN,		boost::bind(&ApplicationImpl::OnMouse, this, _1));
		SetMessageEvent(WM_MBUTTONUP,		boost::bind(&ApplicationImpl::OnMouse, this, _1));
		SetMessageEvent(WM_MBUTTONDBLCLK,	boost::bind(&ApplicationImpl::OnMouse, this, _1));

		SetMessageEvent(WM_RBUTTONDOWN,		boost::bind(&ApplicationImpl::OnMouse, this, _1));
		SetMessageEvent(WM_RBUTTONUP,		boost::bind(&ApplicationImpl::OnMouse, this, _1));
		SetMessageEvent(WM_RBUTTONDBLCLK,	boost::bind(&ApplicationImpl::OnMouse, this, _1));

		//SetMessageEvent(WM_XBUTTONDOWN,	boost::bind(&ApplicationImpl::OnMouse, this, _1));
		//SetMessageEvent(WM_XBUTTONUP,		boost::bind(&ApplicationImpl::OnMouse, this, _1));
		//SetMessageEvent(WM_XBUTTONDBLCLK,	boost::bind(&ApplicationImpl::OnMouse, this, _1));
		SetMessageEvent(WM_MOUSEWHEEL,	boost::bind(&ApplicationImpl::OnMouse, this, _1));

		SetMessageEvent(WM_MOUSEMOVE,		boost::bind(&ApplicationImpl::OnMouse, this, _1));

		SetMessageEvent(WM_SIZE,			boost::bind(&ApplicationImpl::OnSize, this, _1));

		SetMessageEvent(WM_ENTERSIZEMOVE,	boost::bind(&ApplicationImpl::OnEnterSizeMove, this, _1));
		SetMessageEvent(WM_EXITSIZEMOVE,	boost::bind(&ApplicationImpl::OnExitSizeMove, this, _1));
	}

	void ApplicationImpl::init(WindowHandle hParent)
	{
		// Styles and position
		dword Style;
		dword ExStyle;

		std::wstring Name(L"engine_wnd");

		// Window rectangle
		Forms::Drawing::Rectangle Rect;
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


		// Create window
		{
			Forms::Drawing::Rectangle _rect(0, 0, Rect.GetWidth(), Rect.GetHeight());
			CreateWnd((HWND)hParent, Name.c_str(), Name.c_str(), Style, ExStyle, 0, _rect);
		}

		setupHandlers();


		// show window
		::ShowWindow(Handle(), SW_SHOW);
		::UpdateWindow(Handle());
	}

	void ApplicationImpl::init(std::wstring Name, int Width, int Height, bool Fullscreen, bool resize_enable)
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
		Forms::Drawing::Rectangle Rect(y, x, x + Width, y + Height);
		
		/*if (!Fullscreen)
			AdjustWindowRect(&Rect, Style, false);*/
		
		// Register window class
		//RegisterCls(Name.c_str(), CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL, LoadCursor(NULL,IDC_ARROW), (HBRUSH)(COLOR_WINDOW));
		
		
		//RegisterCls(Name.c_str(), CS_HREDRAW | CS_VREDRAW, NULL, LoadCursor(NULL,IDC_ARROW), (HBRUSH)(COLOR_WINDOW));
		RegisterCls(Name.c_str(), NULL, NULL, LoadCursor(NULL,IDC_ARROW), (HBRUSH)(COLOR_WINDOW));

		// Create window
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

	bool ApplicationImpl::update()
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
				//if (!m_Active)
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

	void ApplicationImpl::Run()
	{
		while (update() && !m_bIsClosing)
		{
		}

		// destroy all tasks
		m_tasks.clear();

		// send device destroy reset
		//PostQuitMessage(0);		
	}
}