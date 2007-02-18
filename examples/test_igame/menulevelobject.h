#pragma once

#include "engine.h"

//меню
class MenuLevelObject : public game::ILevelObject, 
						public event::CActor,
						public game::IDynamicObject
{
public:
	MenuLevelObject();
	~MenuLevelObject();

private:
	void update(float dt);

	void onSpace(const input::CButtonEvent&);
	void onEsc(const input::CButtonEvent&);

	void onUpKey(const input::CButtonEvent&);
	void onDownKey(const input::CButtonEvent&);

private:
	struct IMenuItem
	{
		virtual ~IMenuItem() {}
		virtual void send(MenuLevelObject*) = 0;
		std::wstring text;
	};

	template<class EventType>
	struct TMenuItem : public IMenuItem
	{
		TMenuItem(const std::wstring& str, const EventType& e)
			: event(e) 
		{
			text = str;
		}

		virtual void send(MenuLevelObject* obj) {obj->sendEvent(event);}

		EventType event;
	};

	typedef boost::shared_ptr<IMenuItem> PMenuItem;

	typedef std::vector<PMenuItem> MenuItems;
	MenuItems	m_items;

	int m_active_menu_item;
	enum {NumMenuItems = 2};

	render::PFont m_menu_font;
	input::CButtonCommand m_up;
	input::CButtonCommand m_down;
	input::CButtonCommand m_cSpace;
	input::CButtonCommand m_cEsc;
};