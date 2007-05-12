//MenuLevelObject.cpp
#include "MenuLevelObject.h"
#include <iostream>
//#include <render/renderFont.h>
#include <engine.h>

MenuLevelObject::MenuLevelObject(): m_active_menu_item(0)
{
	std::cout << "2: MenuLevelObject created. (SPACE - play, ESC - exit)" << std::endl;

    {
        using namespace input;

		m_up    .attach(L"Up");
		m_down  .attach(L"Down");
		m_cSpace.attach(L"Space");
		m_cEsc  .attach(L"Escape");

		Input::getDevice(types::Keyboard)->getControl(types::KeyUp    )->bind(L"Up");
        Input::getDevice(types::Keyboard)->getControl(types::KeyDown  )->bind(L"Down");
        Input::getDevice(types::Keyboard)->getControl(types::KeySpace )->bind(L"Space");
        Input::getDevice(types::Keyboard)->getControl(types::KeyEscape)->bind(L"Escape");

        m_up     += boost::bind(&MenuLevelObject::onUpKey,   this);
        m_down   += boost::bind(&MenuLevelObject::onDownKey, this);
        m_cSpace += boost::bind(&MenuLevelObject::onSpace,   this);
        m_cEsc   += boost::bind(&MenuLevelObject::onEsc,     this);
    }

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

	for (unsigned int i = 0; i < m_items.size(); ++i)
	{
		unsigned int color = i == m_active_menu_item ? 0xFFF00000 : 0xFFFFFFFF;
		float y = size[1]/2-50*m_items.size()/2 + i*50;
		math::Rect r(0, y, size[0], 50);
		m_menu_font->render(m_items[i]->text, r, color, true, flags);
	}
	//m_menu_font->render(L"PLAY GAME", math::Rect(0, size[1]/2-50, size[0], 50), 0xFFFFFFFF, true, flags);
	//m_menu_font->render(L"EXIT", math::Rect(0, size[1]/2, size[0], 50), 0xFF0000FF, true, flags);
}

void MenuLevelObject::onUpKey()
{
	m_active_menu_item--;
	m_active_menu_item = m_active_menu_item < 0 ? 0 : m_active_menu_item;
}

void MenuLevelObject::onDownKey()
{
    m_active_menu_item++;
    m_active_menu_item = m_active_menu_item >= NumMenuItems ? NumMenuItems-1 : m_active_menu_item;
}

void MenuLevelObject::onSpace() 
{
	m_items[m_active_menu_item]->send(this);
	//sendEvent(m_items[m_active_menu_item].e);
	//sendEvent(game::CCompliteLevelEvent()); // активируется уровень который прописан как следующий
	//sendEvent(game::CSetLevelEvent("play")); // окончание текущего уровня и начало уровня "play"
}

void MenuLevelObject::onEsc() 
{
	sendEvent<game::CCloseGameEvent>(game::CCloseGameEvent());
}


namespace game
{
	REGISTER_LEVEL_TYPE(MenuLevelObject)
}