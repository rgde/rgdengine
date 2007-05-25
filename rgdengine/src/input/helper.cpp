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

	Helper::Helper(const std::wstring &sCommandName)
	{
		attach(sCommandName);
	}

	Helper::~Helper()
	{
		detach();
	}

	void Helper::attach (const std::wstring &sCommandName)
	{
		detach();
		m_command = Input::getCommand(sCommandName);
		//->
		if (!m_command)
		{
			Input::addCommand(sCommandName);
			m_command = Input::getCommand(sCommandName);
		}
		//-<
		if (m_command)
			m_command->attachObserver(this);
	}

	void Helper::detach ()
	{
		if (m_command)
		{
			m_command->detachObserver(this);
			m_command.reset();
		}        
	}

	void Helper::operator += (Helper::Handler handler)
	{
		m_handlers.push_back(handler);
	}


	void Helper::notify (const Control &rControl)
	{
		HelperEvent ev;

		switch (rControl.getType())
		{
			case Control::Axis:
				ev.m_type = HelperEvent::Axis; 
				break;
			case Control::Button:
				ev.m_type = HelperEvent::Button; 
				break;
			default:
				ev.m_type = HelperEvent::Button;
		}

		ev.m_press = rControl.m_press;
		ev.m_delta = rControl.m_delta;
		ev.m_time  = rControl.m_time;

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

	void Button::notify (const Control &rControl)
	{
        Helper::notify(rControl);

		if (rControl.getType() != Control::Button)
			return;

		if (rControl.m_press)
			++m_press;
		else
			--m_press;

		if (m_press<0)
			m_press = 0;

		std::list<Button::ButtonHandler>::iterator i = m_buttonHandlers.begin();
		while (i != m_buttonHandlers.end())
		{
			(*i)(rControl.m_press);
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

	void Trigger::notify (const Control &rControl)
	{
        Helper::notify(rControl);

        if (rControl.getType() != Control::Button)
			return;

		if (!rControl.m_press)
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

	KeyUp::KeyUp() 
	{
	}

	KeyUp::KeyUp(const std::wstring &commandName): Helper(commandName) 
	{
	}

	void KeyUp::operator += (KeyUp::KeyUpHandler handler)
	{
		m_keyupHandlers.push_back(handler);
	}

	void KeyUp::notify (const Control &rControl)
	{
        Helper::notify(rControl);

		if (rControl.getType() != Control::Button)
			return;

		if (rControl.m_press)
			return;

		std::list<KeyUp::KeyUpHandler>::iterator i = m_keyupHandlers.begin();
		while (i != m_keyupHandlers.end())
		{
			(*i)();
			++i;
		}
	}

	//////////////////////////////////////////////////////////////////////////

	KeyDown::KeyDown () 
	{
	}

	KeyDown::KeyDown (const std::wstring &commandName): 
		Helper(commandName) 
	{
	}

	void KeyDown::operator += (KeyDown::KeyDownHandler handler)
	{
		m_keydownHandlers.push_back(handler);
	}

	void KeyDown::notify (const Control &rControl)
	{
        Helper::notify(rControl);

		if (rControl.getType() != Control::Button)
			return;

		if (!rControl.m_press)
			return;

		std::list<KeyDown::KeyDownHandler>::iterator i = m_keydownHandlers.begin();
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

	void RelativeAxis::notify (const Control &rControl)
	{
        Helper::notify(rControl);

		if (rControl.getType() != Control::Axis)
			return;

		std::list<RelativeAxis::RelativeAxisHandler>::iterator i = m_raxisHandlers.begin();
		while (i != m_raxisHandlers.end())
		{
			(*i)(rControl.m_delta);
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

		setPos(m_pos);
	}

	void AbsoluteAxis::setMax (int nMax)
	{
		m_max = nMax;
		setMin(m_min);
	}

	void AbsoluteAxis::setPos (int nPos)
	{
		m_pos = min(max(m_min, nPos), m_max);
	}

	void AbsoluteAxis::notify (const Control &rControl)
	{
        Helper::notify(rControl);

		if (rControl.getType() != Control::Axis)
			return;

		setPos(m_pos + rControl.m_delta);

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
        subscribe<CCursorMove>(&Cursor::onCursorMove);
    }

	void Cursor::operator += (CursorHandler handler)
    {
		m_cursorHandlers.push_back(handler);
    }

    void Cursor::onCursorMove (CCursorMove e)
    {
        m_x = e.x;
        m_y = e.y;

		std::list<Cursor::CursorHandler>::iterator i = m_cursorHandlers.begin();
		while (i != m_cursorHandlers.end())
		{
			(*i)(m_x, m_y);
			++i;
		}
    }

	void Cursor::notify (const Control &rControl)
    {
        //должно быть пусто
    }

//////////////////////////////////////////////////////////////////////////
}