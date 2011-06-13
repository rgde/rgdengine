#pragma once
#include <rgde/engine.h>

//меню
class MenuLevelObject : public game::level_object, 
						public event::sender,
						public game::dynamic_object
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

		virtual void send(MenuLevelObject* obj) {obj->send<EventType>(event);}

		EventType event;
	};

	typedef boost::shared_ptr<IMenuItem> PMenuItem;

	typedef std::vector<PMenuItem> MenuItems;
	MenuItems	m_items;

	int m_active_menu_item;
	enum {NumMenuItems = 2};

	render::font_ptr m_menu_font;
	input::key_down m_up;
	input::key_down m_down;
	input::key_down m_cSpace;
	input::key_down m_esc;
};