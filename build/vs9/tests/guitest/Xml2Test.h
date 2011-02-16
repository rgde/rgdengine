#pragma once

#include "uitest.h"


class xml_test : public rgde::ui_test_application
{
public:
	xml_test(int x, int y, int w, int h, const std::wstring& title);
		
	virtual void createGUISystem();
	virtual void resetGUISystem();

protected:
	gui::WindowPtr m_wnd;
};
