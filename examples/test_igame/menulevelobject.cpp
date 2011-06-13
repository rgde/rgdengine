//MenuLevelObject.cpp
#include "MenuLevelObject.h"
#include <iostream>
#include <rgde/engine.h>

MenuLevelObject::MenuLevelObject()
: m_active_menu_item(0)
, m_up("Up")
, m_down("Down")
, m_cSpace("Space")
, m_esc("Escape")
{
	std::cout << "2: MenuLevelObject created. (SPACE - play, ESC - exit)" << std::endl;

    {
        using namespace input;

		Input::get_control(device::keyboard, KeyUp    )->bind("Up");
        Input::get_control(device::keyboard, KeyDown  )->bind("Down");
        Input::get_control(device::keyboard, KeySpace )->bind("Space");
        Input::get_control(device::keyboard, KeyEscape)->bind("Escape");

        m_up     += boost::bind(&MenuLevelObject::onUpKey,   this);
        m_down   += boost::bind(&MenuLevelObject::onDownKey, this);
        m_cSpace += boost::bind(&MenuLevelObject::onSpace,   this);
        m_esc    += boost::bind(&MenuLevelObject::onEsc,     this);
    }

	using render::font;
	m_menu_font = font::create(40, L"Arial", font::ExtraBold);

	using namespace game;
	PMenuItem item(new TMenuItem<events::on_complite_level>(L"PLAY GAME", events::on_complite_level()));
	m_items.push_back(item);
	item = PMenuItem(new TMenuItem<events::on_close_game>(L"EXIT", events::on_close_game()));
	m_items.push_back(item);	
}

MenuLevelObject::~MenuLevelObject()
{
	std::cout << "MenuLevelObject destroed." << std::endl;
}

void MenuLevelObject::update(float dt)
{
	using render::font;
	unsigned int flags = font::Center | font::SingleLine | font::VCenter | font::Center;
	math::vec2f size(800, 600);

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
	//send(m_items[m_active_menu_item].e);
	//send(game::events::on_complite_level()); // активируется уровень который прописан как следующий
	//send(game::CSetLevelEvent("play")); // окончание текущего уровня и начало уровня "play"
}

void MenuLevelObject::onEsc() 
{
	send<game::events::on_close_game>(game::events::on_close_game());
}


namespace game
{
	REGISTER_LEVEL_TYPE(MenuLevelObject)
}