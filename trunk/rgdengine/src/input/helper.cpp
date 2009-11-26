#include "precompiled.h"

#include <rgde/core/application.h>
#include <rgde/input/helper.h>
#include <rgde/input/input.h>
#include <rgde/input/control.h>
#include <rgde/input/command.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace input
{
//////////////////////////////////////////////////////////////////////////
	// CHelper
	Helper::Helper()
	{
	}

	Helper::Helper(const std::wstring &command_name)
	{
		attach(command_name);
	}

	Helper::~Helper()
	{
		detach();
	}

	void Helper::attach (const std::wstring &command_name)
	{
		detach();
		m_command = Input::get_command(command_name);
		//->
		if (!m_command)
		{
			Input::add_command(command_name);
			m_command = Input::get_command(command_name);
		}
		//-<
		if (m_command)
			m_command->attach(this);
	}

	void Helper::detach ()
	{
		if (m_command)
		{
			m_command->detach(this);
			m_command.reset();
		}        
	}

	void Helper::operator += (Helper::Handler handler)
	{
		m_handlers.push_back(handler);
	}


	void Helper::notify (const Control &c)
	{
		helper_event ev;

		switch (c.get_type())
		{
			case Control::Axis:
				ev.m_type = helper_event::Axis; 
				break;
			case Control::Button:
				ev.m_type = helper_event::Button; 
				break;
			default:
				ev.m_type = helper_event::Button;
		}

		ev.m_press = c.m_press;
		ev.m_delta = c.m_delta;
		ev.m_time  = c.m_time;

		std::list<Helper::Handler>::iterator i = m_handlers.begin();
		while (i != m_handlers.end())
		{
			Helper::Handler& handler = *i;
			handler(ev);
			++i;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// CButton
	Button::Button (): m_press(0) 
	{
	}

	Button::Button(const std::wstring &commandName): 
		Helper(commandName), m_press(0) 
	{
	}

	void Button::operator += (Button::ButtonHandler handler)
	{
		m_buttonHandlers.push_back(handler);
	}

	void Button::notify (const Control &c)
	{
        Helper::notify(c);

		if (c.get_type() != Control::Button)
			return;

		if (c.m_press)
			++m_press;
		else
			--m_press;

		if (m_press<0)
			m_press = 0;

		std::list<Button::ButtonHandler>::iterator i = m_buttonHandlers.begin();
		while (i != m_buttonHandlers.end())
		{
			(*i)(c.m_press);
			++i;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// CTrigger

	Trigger::Trigger()
		: m_is_active(false) 
	{
	}

	Trigger::Trigger(const std::wstring &commandName): 
		Helper(commandName), 
		m_is_active(false) 
	{
	}

	void Trigger::operator += (Trigger::TriggerHandler handler)
	{
		m_triggerHandlers.push_back(handler);
	}

	void Trigger::setState (bool bOn)
	{
		m_is_active = bOn;
	}

	void Trigger::notify (const Control &c)
	{
        Helper::notify(c);

        if (c.get_type() != Control::Button)
			return;

		if (!c.m_press)
			return;

		m_is_active = !m_is_active;

		std::list<Trigger::TriggerHandler>::iterator i = m_triggerHandlers.begin();
		while (i != m_triggerHandlers.end())
		{
			(*i)(m_is_active);
			++i;
		}
	}

//////////////////////////////////////////////////////////////////////////

	key_up::key_up() 
	{
	}

	key_up::key_up(const std::wstring &commandName): Helper(commandName) 
	{
	}

	void key_up::operator += (key_up::KeyUpHandler handler)
	{
		m_keyupHandlers.push_back(handler);
	}

	void key_up::notify (const Control &c)
	{
        Helper::notify(c);

		if (c.get_type() != Control::Button)
			return;

		if (c.m_press)
			return;

		std::list<key_up::KeyUpHandler>::iterator i = m_keyupHandlers.begin();
		while (i != m_keyupHandlers.end())
		{
			(*i)();
			++i;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	key_down::key_down () 
	{
	}

	key_down::key_down (const std::wstring &commandName): 
		Helper(commandName) 
	{
	}

	void key_down::operator += (key_down::KeyDownHandler handler)
	{
		m_keydownHandlers.push_back(handler);
	}

	void key_down::notify (const Control &c)
	{
        Helper::notify(c);

		if (c.get_type() != Control::Button)
			return;

		if (!c.m_press)
			return;

		std::list<key_down::KeyDownHandler>::iterator i = m_keydownHandlers.begin();
		while (i != m_keydownHandlers.end())
		{
			(*i)();
			++i;
		}
	}

//////////////////////////////////////////////////////////////////////////

	RelativeAxis::RelativeAxis()
	{
	}

	RelativeAxis::RelativeAxis(const std::wstring &commandName) : 
		Helper(commandName) 
	{
	}

	void RelativeAxis::operator += (RelativeAxis::RelativeAxisHandler handler)
	{
		m_raxisHandlers.push_back(handler);
	}

	void RelativeAxis::notify (const Control &c)
	{
        Helper::notify(c);

		if (c.get_type() != Control::Axis)
			return;

		std::list<RelativeAxis::RelativeAxisHandler>::iterator i = m_raxisHandlers.begin();
		while (i != m_raxisHandlers.end())
		{
			(*i)(c.m_delta);
			++i;
		}
	}

//////////////////////////////////////////////////////////////////////////

	AbsoluteAxis::AbsoluteAxis ():
		m_min (0),
		m_max (100),
		m_pos (0)
	{
	}

	AbsoluteAxis::AbsoluteAxis (const std::wstring &commandName):
		Helper(commandName),
		m_min (0),
		m_max (100),
		m_pos (0)
	{
	}

	void AbsoluteAxis::operator += (AbsoluteAxis::AbsoluteAxisHandler handler)
	{
		m_aaxisHandlers.push_back(handler);
	}

	void AbsoluteAxis::setMin (int nMin)
	{
		m_min = nMin;

		if (m_min > m_max)
		{
			std::swap(m_min, m_max);
		}

		set_pos(m_pos);
	}

	void AbsoluteAxis::setMax (int nMax)
	{
		m_max = nMax;
		setMin(m_min);
	}

	void AbsoluteAxis::set_pos (int pos)
	{
		m_pos = min(max(m_min, pos), m_max);
	}

	void AbsoluteAxis::notify (const Control &c)
	{
        Helper::notify(c);

		if (c.get_type() != Control::Axis)
			return;

		set_pos(m_pos + c.m_delta);

		std::list<AbsoluteAxis::AbsoluteAxisHandler>::iterator i = m_aaxisHandlers.begin();
		while (i != m_aaxisHandlers.end())
		{
			(*i)(m_pos);
			++i;
		}
	}

//////////////////////////////////////////////////////////////////////////

    Cursor::Cursor (): m_x(0), m_y(0)
    {
        subscribe<mouse_move>(&Cursor::onCursorMove);
    }

	void Cursor::operator += (CursorHandler handler)
    {
		m_cursorHandlers.push_back(handler);
    }

    void Cursor::set_pos (float x, float y)
    {
        m_x = x;
        m_y = y;

        adjustPosToClient(x,y);

        HWND hwnd = (HWND)core::application::get()->get_handle();
        POINT pos;
        pos.x = (int)x;
        pos.y = (int)y;
        ClientToScreen(hwnd, &pos);

        SetCursorPos(pos.x,pos.y);
    }

    void Cursor::onCursorMove (mouse_move e)
    {
        m_x = e.x;
        m_y = e.y;

        adjustPosToWindow(m_x, m_y);

		std::list<Cursor::CursorHandler>::iterator i = m_cursorHandlers.begin();
		while (i != m_cursorHandlers.end())
		{
			(*i)(m_x, m_y);
			++i;
		}
    }

	void Cursor::notify (const Control &c)
    {
        //должно быть пусто
    }

    void Cursor::adjustPosToWindow(float &x, float &y)
    {
        RECT client;
        RECT window;
        HWND hwnd = (HWND)core::application::get()->get_handle();

        GetClientRect(hwnd, &client);
        GetWindowRect(hwnd, &window);

        if (EqualRect(&client, &window))
            return;

        x = x * (window.right-window.left) / (client.right-client.left);
        y = y * (window.bottom-window.top) / (client.bottom-client.top);
    }

    void Cursor::adjustPosToClient(float &x, float &y)
    {
        RECT client;
        RECT window;
        HWND hwnd = (HWND)core::application::get()->get_handle();

        GetClientRect(hwnd, &client);
        GetWindowRect(hwnd, &window);

        if (EqualRect(&client, &window))
            return;

        x = x * (client.right-client.left) / (window.right-window.left);
        y = y * (client.bottom-client.top) / (window.bottom-window.top);
    }

//////////////////////////////////////////////////////////////////////////

    mouse::mouse (): m_left(false), m_middle(false), m_right(false)
    {
        subscribe<mouse_whell>  (&mouse::onWhell);
        subscribe<mouse_button> (&mouse::onButton);
    }

    void mouse::setMoveHandler (CursorHandler handler)
    {
		*this += handler;
    }

	void mouse::setWhellHandler (WhellHandler handler)
    {
		m_whellHandlers.push_back(handler);
    }

    void mouse::setLeftButtonHandler (ButtonHandler handler)
    {
		m_leftButtonHandlers.push_back(handler);
    }

    void mouse::setMiddleButtonHandler (ButtonHandler handler)
    {
		m_middleButtonHandlers.push_back(handler);
    }

    void mouse::setRightButtonHandler (ButtonHandler handler)
    {
		m_rightButtonHandlers.push_back(handler);
    }

    void mouse::onWhell (mouse_whell e)
    {
		std::list<mouse::WhellHandler>::iterator i = m_whellHandlers.begin();
		while (i != m_whellHandlers.end())
		{
			(*i)(e.delta);
			++i;
		}
    }

    void mouse::onButton (mouse_button e)
    {
        //определим тип нажатия
        action_type click;
        if (e.click == mouse_button::Down)        click = mouse::Down;
        if (e.click == mouse_button::Up)          click = mouse::Up;
        if (e.click == mouse_button::DoubleClick) click = mouse::DoubleClick;

        switch (e.button)
        {
            case mouse_button::Left:
            {
                //поменяем состояние
                if (e.click == mouse_button::Down) m_left = true;
                if (e.click == mouse_button::Up)   m_left = false;

                //вызовем обработчики
                std::list<mouse::ButtonHandler>::iterator i = m_leftButtonHandlers.begin();
		        while (i != m_leftButtonHandlers.end())
		        {
                    (*i)(click);
			        ++i;
		        }
                break;
            }

            case mouse_button::Middle:
            {
                //поменяем состояние
                if (e.click == mouse_button::Down) m_middle = true;
                if (e.click == mouse_button::Up)   m_middle = false;

                //вызовем обработчики
                std::list<mouse::ButtonHandler>::iterator i = m_middleButtonHandlers.begin();
		        while (i != m_middleButtonHandlers.end())
		        {
                    (*i)(click);
			        ++i;
		        }
                break;
            }
            case mouse_button::Right:
            {
                //поменяем состояние
                if (e.click == mouse_button::Down) m_right = true;
                if (e.click == mouse_button::Up)   m_right = false;

                //вызовем обработчики
                std::list<mouse::ButtonHandler>::iterator i = m_rightButtonHandlers.begin();
		        while (i != m_rightButtonHandlers.end())
		        {
                    (*i)(click);
			        ++i;
		        }
                break;
            }
        }

    }

//////////////////////////////////////////////////////////////////////////
}