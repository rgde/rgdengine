//MenuLevelObject.cpp
#include "MenuLevelObject.h"
#include <iostream>
//#include <render/renderFont.h>
#include <engine.h>

MenuLevelObject::MenuLevelObject()
: m_active_menu_item(0)
{
	std::cout << "2: MenuLevelObject created. (SPACE - play, ESC - exit)" << std::endl;

	m_up.attachToControl(input::Keyboard,input::KeyUp);
	m_up.addHandler(this, &MenuLevelObject::onUpKey);

	m_down.attachToControl(input::Keyboard,input::KeyDown);
	m_down.addHandler(this, &MenuLevelObject::onDownKey);

	m_cSpace.attachToControl(input::Keyboard,input::KeySpace);
	m_cSpace.addHandler(this, &MenuLevelObject::onSpace);

	m_cEsc.attachToControl(input::Keyboard,input::KeyEscape);
	m_cEsc.addHandler(this, &MenuLevelObject::onEsc);

	using render::IFont;

	m_menu_font = IFont::Create(40, L"Arial", IFont::ExtraBold);

	using namespace game;

	PMenuItem item(new TMenuItem<CCompliteLevelEvent>(L"PLAY GAME", CCompliteLevelEvent()));
	m_items.push_back(item);

	item = PMenuItem(new TMenuItem<CCloseGameEvent>(L"EXIT", CCloseGameEvent()));
	m_items.push_back(item);	
}

MenuLevelObject::~MenuLevelObject()
{
	std::cout << "MenuLevelObject destroed." << std::endl;
}

void MenuLevelObject::update(float dt)
{
	using render::IFont;
	unsigned int flags = IFont::Center | IFont::SingleLine | IFont::VCenter | IFont::Center;
	math::Vec2f size(800, 600);

	for (int i = 0; i < m_items.size(); ++i)
	{
		unsigned int color = i == m_active_menu_item ? 0xFFF00000 : 0xFFFFFFFF;
		float y = size[1]/2-50*m_items.size()/2 + i*50;
		math::Rect r(0, y, size[0], 50);
		m_menu_font->render(m_items[i]->text, r, color, true, flags);
	}
	//m_menu_font->render(L"PLAY GAME", math::Rect(0, size[1]/2-50, size[0], 50), 0xFFFFFFFF, true, flags);
	//m_menu_font->render(L"EXIT", math::Rect(0, size[1]/2, size[0], 50), 0xFF0000FF, true, flags);
}

void MenuLevelObject::onUpKey(const input::CButtonEvent&)
{
	if (m_up) 
	{
		m_active_menu_item--;
		m_active_menu_item = m_active_menu_item < 0 ? 0 : m_active_menu_item;
	}
}

void MenuLevelObject::onDownKey(const input::CButtonEvent&)
{
	if (m_down) 
	{
		m_active_menu_item++;
		m_active_menu_item = m_active_menu_item >= NumMenuItems ? NumMenuItems-1 : m_active_menu_item;
	}
}

//здесь стоит if, чтобы реагировать только на Ќјжати€ (без него евент посылаетс€ и на ќ“жати€)
void MenuLevelObject::onSpace(const input::CButtonEvent&) 
{
	if (m_cSpace) 
		m_items[m_active_menu_item]->send(this);
		//sendEvent(m_items[m_active_menu_item].e);
		//sendEvent(game::CCompliteLevelEvent()); // активируетс€ уровень который прописан как следующий
		//sendEvent(game::CSetLevelEvent("play")); // окончание текущего уровн€ и начало уровн€ "play"
}

void MenuLevelObject::onEsc(const input::CButtonEvent&) 
{
	if (m_cEsc) 
		sendEvent(game::CCloseGameEvent());
}


namespace game
{
	REGISTER_LEVEL_TYPE(MenuLevelObject)
}