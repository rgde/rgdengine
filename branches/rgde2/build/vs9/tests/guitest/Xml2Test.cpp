#include "xml2test.h"

using namespace gui;

xml_test::xml_test(int x, int y, int w, int h, const std::wstring& title)
: rgde::ui_test_application(x,y,w,h,title)
{
	m_filename = "guitest\\test.xml";	
}

void xml_test::createGUISystem()
{
	ui_test_application::createGUISystem();
	load(m_filename);
}

void xml_test::resetGUISystem()
{
	ui_test_application::resetGUISystem();
	
	load(m_filename);
}
