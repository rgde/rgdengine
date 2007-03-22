#pragma once
#include "engine.h"

//����
class MenuLevelObject : public game::ILevelObject, 
						public event::CSender,
						public game::IDynamicObject
{
public:
    MenuLevelObject();
   ~MenuLevelObject();

private:
	void update(float dt);

	void onSpace();
	void onEsc();
	void onUpKey();
	void onDownKey();

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

		virtual void send(MenuLevelObject* obj) {obj->sendEvent<EventType>(event);}

		EventType event;
	};

	typedef boost::shared_ptr<IMenuItem> PMenuItem;

	typedef std::vector<PMenuItem> MenuItems;
	MenuItems	m_items;

	int m_active_menu_item;
	enum {NumMenuItems = 2};

	render::PFont m_menu_font;
	input::CKeyDown m_up;
	input::CKeyDown m_down;
	input::CKeyDown m_cSpace;
	input::CKeyDown m_cEsc;
};