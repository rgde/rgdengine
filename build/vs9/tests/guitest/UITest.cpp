#include "DXUT.h"

#include	<shlobj.h>

#include "UITest.h"
#include "guiplatform.h"

using namespace gui;


UITest::UITest() 
	: m_render(NULL), 
	m_system(NULL),
	m_hFile(INVALID_HANDLE_VALUE)
{
	wchar_t	wpath[MAX_PATH];
	// init app data path
	SHGetFolderPathW(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, wpath);

	std::wstring log(wpath);
	log += L"\\RGDEngine\\guitest.log";
	m_hFile = CreateFileW(log.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
}

UITest::~UITest()
{
	if(m_system)
		delete m_system;

	if(m_render)
		delete m_render;

	if(m_hFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFile);
}

void UITest::createGUISystem(IDirect3DDevice9* dev)
{
	m_render = gui::win32dx9platform::CreateRenderer(dev, 1024);
	if(m_system)
		delete m_system;
	m_system = new System(*m_render, "bootstrap\\GUI\\", "Well", 
			boost::bind(&UITest::log, this, _1, _2));
	if(m_system)
	{
		//::ShowCursor(FALSE);
		Cursor& cursor = m_system->getCursor();
		cursor.setType("CursorNormal");
	}
}

void UITest::resetGUISystem()
{
	if(m_render)
		m_render->clearRenderList();

	if(m_system)
		m_system->reset();	
}

void UITest::Update(float delta)
{
	m_framecount++;
	if(m_system)
	{
		m_system->tick(delta);
		m_system->draw();
	}
	
}

void UITest::Render()
{
	if (NULL != m_system)
		m_system->render();
	//else if (NULL != m_render)
	//	m_render->doRender();
}

bool UITest::isFinished() 
{
	/*if(m_framecount >= 50)
	return true;*/

	return false;
}

void UITest::handleViewportChange()
{
	if(m_system)
		return m_system->handleViewportChange();
}

bool UITest::handleMouseMove(int x, int y)
{
	if(m_system)
		return m_system->handleMouseMove(x, y);
	else
		return false;
}

bool UITest::handleMouseWheel(int diff)
{
	if(m_system)
		return m_system->handleMouseWheel(diff);
	else
		return false;
}

bool UITest::handleMouseButton(EventArgs::MouseButtons btn, EventArgs::ButtonState state)
{
	if(m_system)
		return m_system->handleMouseButton(btn, state);
	else
		return false;
}

bool UITest::handleKeyboard(UINT_PTR key, EventArgs::ButtonState state)
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

bool UITest::handleChar(UINT_PTR ch)
{
	if(m_system)
		return m_system->handleChar((unsigned int)ch);
	else
		return false;
}


void UITest::log(LogLevel level, const std::string& message)
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

void UITest::load(const std::string& xml)
{
	if(m_system)
	{
		m_system->loadXml(xml);
	}
}

void UITest::OnLostDevice(void)
{
	try
	{
		m_render->OnLostDevice();
	}
	catch(...)
	{	
	}		
}

HRESULT UITest::OnResetDevice(void)
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