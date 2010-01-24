#include	<shlobj.h>

#include "uitest.h"
#include "guiplatform.h"

#include <rgde/core/math.h>

#include <boost/filesystem.hpp>
#include <boost/bind.hpp>

using namespace gui;
using namespace rgde;


ui_test_application::ui_test_application(int x, int y, int w, int h, const std::wstring& title)
	: m_render(NULL)
	, m_system(NULL)
	, m_hFile(INVALID_HANDLE_VALUE)
	, window(math::vec2i(x, y), math::vec2i(w, h), title, 0, WS_BORDER | WS_CAPTION | WS_SYSMENU)
	, m_render_device(get_handle(), m_filesystem)
	, m_elapsed(0)
	, m_active(true)
{
	wchar_t	wpath[MAX_PATH];
	// init app data path
	SHGetFolderPathW(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, wpath);

	std::wstring log(wpath);
	log += L"\\RGDEngine\\guitest.log";
	m_hFile = CreateFileW(log.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	wchar_t buf[512];
	GetModuleFileNameW(NULL, &buf[0], 512);

	boost::filesystem::wpath p(buf);
	std::wstring path = p.branch_path().string() + L"/../data/";
	SetCurrentDirectoryW(path.c_str());

	show();
	update(0.0f);	
}

ui_test_application::~ui_test_application()
{
	if(m_system)
		delete m_system;

	if(m_render)
		delete m_render;

	if(m_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);
}

void ui_test_application::run()
{	
	createGUISystem();

	m_timer.restart();

	while( is_created() )
	{
		if( !do_events() && m_active)
		{
			m_elapsed = m_timer.elapsed();
			update(m_elapsed);
			render();
		}
	}
}

void ui_test_application::createGUISystem()
{
	m_render = gui::rgde_platform::CreateRenderer(m_render_device, 
		m_filesystem, 
		1024);

	if(m_system)
		delete m_system;

	m_system = new System(*m_render, "gui\\", "default", 
			boost::bind(&ui_test_application::log, this, _1, _2));

	if(m_system)
	{
		//::ShowCursor(FALSE);
		Cursor& cursor = m_system->getCursor();
		cursor.setType("CursorNormal");
	}
}

void ui_test_application::resetGUISystem()
{
	if(m_render)
		m_render->clearRenderList();

	if(m_system)
		m_system->reset();	
}

void ui_test_application::update(float delta)
{
	m_framecount++;
	if(m_system)
	{
		m_system->tick(delta);
		m_system->draw();
	}
	
}

bool ui_test_application::do_events()
{
	MSG msg = {0};
	if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
		return true;
	}
	return false;
}

void ui_test_application::render()
{
	m_render_device.frame_begin();
	m_render_device.clear(rgde::math::color::Black);

	m_render_device.set_ztest(true);
	m_render_device.set_cull_mode(rgde::render::cull_none);
	m_render_device.set_lighting(false);
	m_render_device.set_alpha_blend(false);
	m_render_device.set_alpha_test(false);

	if (NULL != m_system)
		m_system->render();

	m_render_device.frame_end();
	m_render_device.present();
}

bool ui_test_application::isFinished() 
{
	/*if(m_framecount >= 50)
	return true;*/

	return false;
}

core::windows::result ui_test_application::wnd_proc(ushort message, uint wparam, long lparam )
{
	switch (message)
	{
	case WM_CHAR:
		handleChar((UINT_PTR)wparam);
		return 0;

	case WM_LBUTTONDOWN:
		handleMouseButton(EventArgs::Left, EventArgs::Down);
		return 0;

	case WM_LBUTTONUP:
		handleMouseButton(EventArgs::Left, EventArgs::Up);
		return 0;

	case WM_RBUTTONDOWN:
		handleMouseButton(EventArgs::Right, EventArgs::Down);
		return 0;

	case WM_RBUTTONUP:
		handleMouseButton(EventArgs::Right, EventArgs::Up);
		return 0;

	case WM_MBUTTONDOWN:
		handleMouseButton(EventArgs::Middle, EventArgs::Down);				
		return 0;

	case WM_MBUTTONUP:
		handleMouseButton(EventArgs::Middle, EventArgs::Up);
		return 0;

	case WM_ACTIVATE:	// Watch For Window Activate Message
		m_active = !HIWORD(wparam);// Check Minimization State
		return 0;

	case WM_KEYDOWN:
		{
			if ('Q' == wparam || 'q' == wparam || VK_ESCAPE == wparam)
				exit(0);

			handleKeyboard((UINT_PTR)wparam, EventArgs::Down);

			return 0;
		}

	case WM_KEYUP:
		handleKeyboard((UINT_PTR)wparam, EventArgs::Up);
		return 0;

	case WM_SIZE:
		//resize_scene(LOWORD(lparam), HIWORD(lparam));
		return 0;

	case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wparam);
			if(m_system)
				m_system->handleMouseWheel(delta);
		}
		return 0;

	case WM_MOUSEMOVE:
		if(m_system)
			return m_system->handleMouseMove(LOWORD(lparam), HIWORD(lparam));
		return 0;
	}
	return window::wnd_proc(message, wparam, lparam);
}

void ui_test_application::handleViewportChange()
{
	if(m_system)
		return m_system->handleViewportChange();
}


bool ui_test_application::handleMouseButton(EventArgs::MouseButtons btn, EventArgs::ButtonState state)
{
	if(m_system)
		return m_system->handleMouseButton(btn, state);
	else
		return false;
}

bool ui_test_application::handleKeyboard(UINT_PTR key, EventArgs::ButtonState state)
{
	if(m_system)
	{
		if (!m_filename.empty())
			if((EventArgs::Keys)key == EventArgs::K_F5 && state == EventArgs::Down)
			{
				resetGUISystem();
				return true;
			}

		return m_system->handleKeyboard((EventArgs::Keys)key, state);
	}
	else
		return false;
}

bool ui_test_application::handleChar(UINT_PTR ch)
{
	if(m_system)
		return m_system->handleChar((unsigned int)ch);
	else
		return false;
}


void ui_test_application::log(LogLevel level, const std::string& message)
{
	if(m_hFile != INVALID_HANDLE_VALUE)
	{
		std::string type;
		switch(level)
		{
		case LogSystem:
			type = "System";
			break;
		case LogWarning:
			type = "Warning";
			break;
		case LogError:
			type = "Error";
			break;
		case LogCritical:
			type = "Critical";
			break;
		default:
			type = "Message";
			break;
		};

		SYSTEMTIME st;
		GetLocalTime(&st);

		char timestamp[32] = {0};
		_snprintf(timestamp, 32, "[%04d.%02d.%02d %02d:%02d:%02d][%s] ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, type.c_str());

		char con_timestamp[32] = {0};
		_snprintf(con_timestamp, 32, "[%02d:%02d:%02d][%s] ", st.wHour, st.wMinute, st.wSecond, type.c_str());

		std::string m(timestamp);
		m += message;
		std::cout << (std::string(con_timestamp)+message).c_str() << std::endl;
		m += "\n";
		DWORD len = (DWORD)m.length();
		WriteFile(m_hFile, m.c_str(), len, &len, 0);
	}
}

void ui_test_application::load(const std::string& xml)
{
	if(m_system)
	{
		m_system->loadXml(xml);
	}
}

void ui_test_application::OnLostDevice(void)
{
	try
	{
		m_render->OnLostDevice();
	}
	catch(...)
	{	
	}		
}

HRESULT ui_test_application::OnResetDevice(void)
{
	try
	{
		m_render->OnResetDevice();
		handleViewportChange();
	}
	catch(...)
	{
		return S_FALSE;
	}
	return S_OK;
}