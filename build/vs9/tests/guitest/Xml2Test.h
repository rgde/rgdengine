#pragma once
#include "uitest.h"

class Xml2Test :
	public UITest
{
public:
	Xml2Test(void);
	~Xml2Test(void);
		
	virtual void createGUISystem(IDirect3DDevice9* dev);
	virtual void resetGUISystem();

protected:
	gui::WindowPtr m_wnd;
};
