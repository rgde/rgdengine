#include "DXUT.h"
#include "Xml2Test.h"

using namespace gui;

Xml2Test::Xml2Test(void)
{
	m_filename = "guitest\\hud.xml";
}

Xml2Test::~Xml2Test(void)
{
}

void Xml2Test::createGUISystem(IDirect3DDevice9* dev)
{
	UITest::createGUISystem(dev);
	
	load(m_filename);
}

void Xml2Test::resetGUISystem()
{
	UITest::resetGUISystem();
	
	load(m_filename);
}
