#include "stdafx.h"

#include <rgde/input/helper.h>
#include <rgde/input/input.h>
#include <rgde/input/control.h>
#include <rgde/input/command.h>


namespace rgde
{
namespace input
{
//////////////////////////////////////////////////////////////////////////
	helper::helper(system& system)
		: m_system(system)
	{
	}

	helper::helper(const std::wstring &command_name, system& system)
		: m_system(system)
	{
		attach(command_name);
	}

	helper::~helper()
	{
		detach();
	}

	void helper::attach (const std::wstring &command_name)
	{
		detach();
		m_command = m_system.get_command(command_name);
		//->
		if (!m_command)
		{
			m_system.add_command(command_name);
			m_command = m_system.get_command(command_name);
		}
		//-<
		if (m_command)
			m_command->attach(this);
	}

	void helper::detach ()
	{
		if (m_command)
		{
			m_command->detach(this);
			m_command.reset();
		}        
	}

	void helper::operator += (helper::handler_type handler)
	{
		m_handlers.push_back(handler);
	}


	void helper::notify (const control &control)
	{
		helper_event ev;

		switch (control.get_type())
		{
			case control::axis:
				ev.m_type = helper_event::axis; 
				break;
			case control::button:
				ev.m_type = helper_event::button; 
				break;
			default:
				ev.m_type = helper_event::button;
		}

		ev.m_press = control.m_press;
		ev.m_delta = control.m_delta;
		ev.m_time  = control.m_time;

		std::list<helper::handler_type>::iterator i = m_handlers.begin();
		while (i != m_handlers.end())
		{
			helper::handler_type& handler = *i;
			handler(ev);
			++i;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// CButton
	button::button (system& system)
		: m_press(0)
		, helper(system)
	{
	}

	button::button(const std::wstring &command_name, system& system)
		: helper(command_name, system)
		, m_press(0) 
	{
	}

	void button::operator += (button::button_handler_type handler)
	{
		m_button_handlers.push_back(handler);
	}

	void button::notify (const control &control)
	{
        helper::notify(control);

		if (control.get_type() != control::button)
			return;

		if (control.m_press)
			++m_press;
		else
			--m_press;

		if (m_press<0)
			m_press = 0;

		std::list<button::button_handler_type>::iterator i = m_button_handlers.begin();
		while (i != m_button_handlers.end())
		{
			(*i)(control.m_press);
			++i;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// CTrigger

	trigger::trigger(system& system)
		: m_is_active(false)
		, helper(system)
	{
	}

	trigger::trigger(const std::wstring &command_name, system& system)
		: helper(command_name, system)
		, m_is_active(false) 
	{
	}

	void trigger::operator += (trigger::trigger_handler_type handler)
	{
		m_trigger_handlers.push_back(handler);
	}

	void trigger::set_state (bool bOn)
	{
		m_is_active = bOn;
	}

	void trigger::notify (const control &control)
	{
        helper::notify(control);

        if (control.get_type() != control::button)
			return;

		if (!control.m_press)
			return;

		m_is_active = !m_is_active;

		std::list<trigger::trigger_handler_type>::iterator i = m_trigger_handlers.begin();
		while (i != m_trigger_handlers.end())
		{
			(*i)(m_is_active);
			++i;
		}
	}

//////////////////////////////////////////////////////////////////////////

	key_up::key_up(system& system) 
		: helper(system)
	{
	}

	key_up::key_up(const std::wstring &command_name, system& system)
		: helper(command_name, system)
	{
	}

	void key_up::operator += (key_up::keyup_handler_type handler)
	{
		m_keyup_handlers.push_back(handler);
	}

	void key_up::notify (const control &control)
	{
        helper::notify(control);

		if (control.get_type() != control::button)
			return;

		if (control.m_press)
			return;

		std::list<key_up::keyup_handler_type>::iterator i = m_keyup_handlers.begin();
		while (i != m_keyup_handlers.end())
		{
			(*i)();
			++i;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	key_down::key_down(system& system) 
		: helper(system)
	{
	}

	key_down::key_down(const std::wstring &command_name, system& system)
		: helper(command_name, system) 
	{
	}

	void key_down::operator += (key_down::keydown_handler_type handler)
	{
		m_keydownHandlers.push_back(handler);
	}

	void key_down::notify (const control &control)
	{
        helper::notify(control);

		if (control.get_type() != control::button)
			return;

		if (!control.m_press)
			return;

		std::list<key_down::keydown_handler_type>::iterator i = m_keydownHandlers.begin();
		while (i != m_keydownHandlers.end())
		{
			(*i)();
			++i;
		}
	}

//////////////////////////////////////////////////////////////////////////

	relative_axis::relative_axis(system& system)
		: helper(system)
	{
	}

	relative_axis::relative_axis(const std::wstring &command_name, system& system) 
		: helper(command_name, system) 
	{
	}

	void relative_axis::operator += (relative_axis::relativeaxis_handle_type handler)
	{
		m_raxis_handlers.push_back(handler);
	}

	void relative_axis::notify (const control &control)
	{
        helper::notify(control);

		if (control.get_type() != control::axis)
			return;

		std::list<relative_axis::relativeaxis_handle_type>::iterator i = m_raxis_handlers.begin();
		while (i != m_raxis_handlers.end())
		{
			(*i)(control.m_delta);
			++i;
		}
	}

//////////////////////////////////////////////////////////////////////////

	absolute_axis::absolute_axis (system& system)
		: helper(system)
		, m_min (0)
		, m_max (100)
		, m_pos (0)
	{
	}

	absolute_axis::absolute_axis (const std::wstring &command_name, system& system)
		: helper(command_name, system)
		, m_min (0)
		, m_max (100)
		, m_pos (0)
	{
	}

	void absolute_axis::operator += (absolute_axis::absolute_axis_handler_type handler)
	{
		m_aaxis_handlers.push_back(handler);
	}

	void absolute_axis::set_min (int nMin)
	{
		m_min = nMin;

		if (m_min > m_max)
		{
			std::swap(m_min, m_max);
		}

		set_pos(m_pos);
	}

	void absolute_axis::set_max (int nMax)
	{
		m_max = nMax;
		set_min(m_min);
	}

	void absolute_axis::set_pos (int nPos)
	{
		m_pos = std::min(std::max(m_min, nPos), m_max);
	}

	void absolute_axis::notify (const control &control)
	{
        helper::notify(control);

		if (control.get_type() != control::axis)
			return;

		set_pos(m_pos + control.m_delta);

		std::list<absolute_axis::absolute_axis_handler_type>::iterator i = m_aaxis_handlers.begin();
		while (i != m_aaxis_handlers.end())
		{
			(*i)(m_pos);
			++i;
		}
	}
//
////////////////////////////////////////////////////////////////////////////
//
//    Cursor::Cursor (): x(0), y(0)
//    {
//        subscribe<CCursorMove>(&Cursor::onCursorMove);
//    }
//
//	  void Cursor::operator += (CursorHandler handler)
//    {
//		m_cursorHandlers.push_back(handler);
//    }
//
//    void Cursor::set_pos (float x, float y)
//    {
//        x = x;
//        y = y;
//
//        adjustPosToClient(x,y);
//
//        HWND hwnd = (HWND)core::application::get()->getWindowHandle();
//        POINT pos;
//        pos.x = (int)x;
//        pos.y = (int)y;
//        ClientToScreen(hwnd, &pos);
//
//        SetCursorPos(pos.x,pos.y);
//    }
//
//    void Cursor::onCursorMove (CCursorMove e)
//    {
//        x = e.x;
//        y = e.y;
//
//        adjustPosToWindow(x, y);
//
//		std::list<Cursor::CursorHandler>::iterator i = m_cursorHandlers.begin();
//		while (i != m_cursorHandlers.end())
//		{
//			(*i)(x, y);
//			++i;
//		}
//    }
//
//	void Cursor::notify (const control &control)
//    {
//        //должно быть пусто
//    }
//
//    void Cursor::adjustPosToWindow(float &x, float &y)
//    {
//        RECT client;
//        RECT window;
//        HWND hwnd = (HWND)core::application::get()->getWindowHandle();
//
//        GetClientRect(hwnd, &client);
//        GetWindowRect(hwnd, &window);
//
//        if (EqualRect(&client, &window))
//            return;
//
//        x = x * (window.right-window.left) / (client.right-client.left);
//        y = y * (window.bottom-window.top) / (client.bottom-client.top);
//    }
//
//    void Cursor::adjustPosToClient(float &x, float &y)
//    {
//        RECT client;
//        RECT window;
//        HWND hwnd = (HWND)core::application::get()->getWindowHandle();
//
//        GetClientRect(hwnd, &client);
//        GetWindowRect(hwnd, &window);
//
//        if (EqualRect(&client, &window))
//            return;
//
//        x = x * (client.right-client.left) / (window.right-window.left);
//        y = y * (client.bottom-client.top) / (window.bottom-window.top);
//    }
//
////////////////////////////////////////////////////////////////////////////
//
//    Mouse::Mouse (): m_left(false), m_middle(false), m_right(false)
//    {
//        subscribe<CMouseWhell>  (&Mouse::onWhell);
//        subscribe<CMouseButton> (&Mouse::onButton);
//    }
//
//    void Mouse::setMoveHandler (CursorHandler handler)
//    {
//		*this += handler;
//    }
//
//	void Mouse::setWhellHandler (WhellHandler handler)
//    {
//		m_whellHandlers.push_back(handler);
//    }
//
//    void Mouse::setLeftButtonHandler (button_handler_type handler)
//    {
//		m_leftButtonHandlers.push_back(handler);
//    }
//
//    void Mouse::setMiddleButtonHandler (button_handler_type handler)
//    {
//		m_middleButtonHandlers.push_back(handler);
//    }
//
//    void Mouse::setRightButtonHandler (button_handler_type handler)
//    {
//		m_rightButtonHandlers.push_back(handler);
//    }
//
//    void Mouse::onWhell (CMouseWhell e)
//    {
//		std::list<Mouse::WhellHandler>::iterator i = m_whellHandlers.begin();
//		while (i != m_whellHandlers.end())
//		{
//			(*i)(e.delta);
//			++i;
//		}
//    }
//
//    void Mouse::onButton (CMouseButton e)
//    {
//        //определим тип нажатия
//        ClickType click;
//        if (e.click == CMouseButton::Down)        click = Mouse::Down;
//        if (e.click == CMouseButton::Up)          click = Mouse::Up;
//        if (e.click == CMouseButton::DoubleClick) click = Mouse::DoubleClick;
//
//        switch (e.button)
//        {
//            case CMouseButton::Left:
//            {
//                //поменяем состояние
//                if (e.click == CMouseButton::Down) m_left = true;
//                if (e.click == CMouseButton::Up)   m_left = false;
//
//                //вызовем обработчики
//                std::list<Mouse::button_handler_type>::iterator i = m_leftButtonHandlers.begin();
//		        while (i != m_leftButtonHandlers.end())
//		        {
//                    (*i)(click);
//			        ++i;
//		        }
//                break;
//            }
//
//            case CMouseButton::Middle:
//            {
//                //поменяем состояние
//                if (e.click == CMouseButton::Down) m_middle = true;
//                if (e.click == CMouseButton::Up)   m_middle = false;
//
//                //вызовем обработчики
//                std::list<Mouse::button_handler_type>::iterator i = m_middleButtonHandlers.begin();
//		        while (i != m_middleButtonHandlers.end())
//		        {
//                    (*i)(click);
//			        ++i;
//		        }
//                break;
//            }
//            case CMouseButton::Right:
//            {
//                //поменяем состояние
//                if (e.click == CMouseButton::Down) m_right = true;
//                if (e.click == CMouseButton::Up)   m_right = false;
//
//                //вызовем обработчики
//                std::list<Mouse::button_handler_type>::iterator i = m_rightButtonHandlers.begin();
//		        while (i != m_rightButtonHandlers.end())
//		        {
//                    (*i)(click);
//			        ++i;
//		        }
//                break;
//            }
//        }
//
//    }

//////////////////////////////////////////////////////////////////////////
}
}