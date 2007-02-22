//helper.cpp
#include "precompiled.h"
#include "input/helper.h"
#include "input/input.h"
#include "input/control.h"
#include "input/command.h"

namespace input
{

    /////////////
    // CHelper //
    /////////////

    CHelper::CHelper ()
    {
    }

    CHelper::CHelper(const std::wstring &sCommandName): m_pCommand(CInput::getCommand(sCommandName))
    {
        if (m_pCommand.get())
            m_pCommand->attachObserver(this);
    }

    CHelper::~CHelper()
    {
        if (m_pCommand.get())
            m_pCommand->detachObserver(this);
    }

    void CHelper::attach (const std::wstring &sCommandName)
    {
        detach();
        m_pCommand = CInput::getCommand(sCommandName);
        if (m_pCommand.get())
            m_pCommand->attachObserver(this);
    }

    void CHelper::detach ()
    {
        if (m_pCommand.get())
            m_pCommand->detachObserver(this);
        m_pCommand.reset();
    }

    void CHelper::operator += (CHelper::Handler handler)
    {
        m_handlers.push_back(handler);
    }


    void CHelper::notify (const CControl &rControl)
    {
        CHelperEvent ev;

        switch (rControl.getType())
        {
            case CControl::Axis:   ev.m_eType = CHelperEvent::Axis; break;
            case CControl::Button: ev.m_eType = CHelperEvent::Button; break;
            default:			   ev.m_eType = CHelperEvent::Button;
        }
        ev.m_bPress = rControl.m_bPress;
        ev.m_nDelta = rControl.m_nDelta;
        ev.m_nTime  = rControl.m_nTime;

        std::list<CHelper::Handler>::iterator i = m_handlers.begin();
        while (i != m_handlers.end())
        {
            (*i)(ev);
            ++i;
        }
    }

    /////////////
    // CButton //
    /////////////

    void CButton::operator += (CButton::ButtonHandler handler)
    {
        m_buttonHandlers.push_back(handler);
    }

    void CButton::notify (const CControl &rControl)
    {
        if (rControl.getType() != CControl::Button)
            return;

        if (rControl.m_bPress)
            ++m_nPress;
        else
            --m_nPress;
        if (m_nPress<0)
            m_nPress = 0;

        std::list<CButton::ButtonHandler>::iterator i = m_buttonHandlers.begin();
        while (i != m_buttonHandlers.end())
        {
            (*i)(rControl.m_bPress);
            ++i;
        }
    }

    //////////////
    // CTrigger //
    //////////////

    void CTrigger::operator += (CTrigger::TriggerHandler handler)
    {
        m_triggerHandlers.push_back(handler);
    }

    void CTrigger::setState (bool bOn)
    {
        m_bOn = bOn;
    }

    void CTrigger::notify (const CControl &rControl)
    {
        if (rControl.getType() != CControl::Button)
            return;

        if (!rControl.m_bPress)
            return;

        m_bOn = !m_bOn;

        std::list<CTrigger::TriggerHandler>::iterator i = m_triggerHandlers.begin();
        while (i != m_triggerHandlers.end())
        {
            (*i)(m_bOn);
            ++i;
        }
    }

    ////////////
    // CKeyUp //
    ////////////

    void CKeyUp::operator += (CKeyUp::KeyUpHandler handler)
    {
        m_keyupHandlers.push_back(handler);
    }

    void CKeyUp::notify (const CControl &rControl)
    {
        if (rControl.getType() != CControl::Button)
            return;

        if (rControl.m_bPress)
            return;

        std::list<CKeyUp::KeyUpHandler>::iterator i = m_keyupHandlers.begin();
        while (i != m_keyupHandlers.end())
        {
            (*i)();
            ++i;
        }
    }

    //////////////
    // CKeyDown //
    //////////////

    void CKeyDown::operator += (CKeyDown::KeyDownHandler handler)
    {
        m_keydownHandlers.push_back(handler);
    }

    void CKeyDown::notify (const CControl &rControl)
    {
        if (rControl.getType() != CControl::Button)
            return;

        if (!rControl.m_bPress)
            return;

        std::list<CKeyDown::KeyDownHandler>::iterator i = m_keydownHandlers.begin();
        while (i != m_keydownHandlers.end())
        {
            (*i)();
            ++i;
        }
    }

    ///////////////////
    // CRelativeAxis //
    ///////////////////

    void CRelativeAxis::operator += (CRelativeAxis::RelativeAxisHandler handler)
    {
        m_raxisHandlers.push_back(handler);
    }

    void CRelativeAxis::notify (const CControl &rControl)
    {
		if (rControl.getType() != CControl::Axis)
            return;

        std::list<CRelativeAxis::RelativeAxisHandler>::iterator i = m_raxisHandlers.begin();
        while (i != m_raxisHandlers.end())
        {
            (*i)(rControl.m_nDelta);
            ++i;
        }
    }

    ///////////////////
    // CAbsoluteAxis //
    ///////////////////

    void CAbsoluteAxis::operator += (CAbsoluteAxis::AbsoluteAxisHandler handler)
    {
        m_aaxisHandlers.push_back(handler);
    }

	int CAbsoluteAxis::getMin ()
	{
		return m_nMin;
	}

	int CAbsoluteAxis::getMax ()
	{
		return m_nMax;
	}

	int CAbsoluteAxis::getPos ()
	{
		return m_nPos;
	}

	void CAbsoluteAxis::setMin (int nMin)
	{
		m_nMin = nMin;

		if (m_nMin > m_nMax)
		{
			int t  = m_nMin;
			m_nMin = m_nMax;
			m_nMax = t;
		}

		if (m_nPos < m_nMin) m_nPos = m_nMin;
		if (m_nPos > m_nMax) m_nPos = m_nMax;
	}

	void CAbsoluteAxis::setMax (int nMax)
	{
		m_nMax = nMax;

		if (m_nMin > m_nMax)
		{
			int t  = m_nMin;
			m_nMin = m_nMax;
			m_nMax = t;
		}

		if (m_nPos < m_nMin) m_nPos = m_nMin;
		if (m_nPos > m_nMax) m_nPos = m_nMax;
	}

	void CAbsoluteAxis::setPos (int nPos)
	{
		m_nPos = nPos;

		if (m_nPos < m_nMin) m_nPos = m_nMin;
		if (m_nPos > m_nMax) m_nPos = m_nMax;
	}

    void CAbsoluteAxis::notify (const CControl &rControl)
    {
		if (rControl.getType() != CControl::Axis)
            return;

		m_nPos += rControl.m_nDelta;
		if (m_nPos < m_nMin) m_nPos = m_nMin;
		if (m_nPos > m_nMax) m_nPos = m_nMax;

        std::list<CAbsoluteAxis::AbsoluteAxisHandler>::iterator i = m_aaxisHandlers.begin();
        while (i != m_aaxisHandlers.end())
        {
            (*i)(m_nPos);
            ++i;
        }
    }

}