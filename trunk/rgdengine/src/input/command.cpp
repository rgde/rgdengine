#include "precompiled.h"

#include "input/command.h"
#include "input/control.h"
#include "input/helper.h"

namespace input
{
	Command::Command(const std::wstring &sName, InputImpl &rInput):
		m_rInput  (rInput),
		m_sName   (sName),
		m_bLocked (false)
	{
	}

    void Command::notifyAllObservers (const Control &rControl)
    {
        if (m_bLocked)
            return;

        HelpersIt i = m_helpers.begin();
        while (i != m_helpers.end())
        {
            (*i)->notify(rControl);
            ++i;
        }
    }

    void Command::attachObserver (Helper *pHelper)
    {
        m_helpers.push_back(pHelper);
    }

    void Command::detachObserver (Helper *pHelper)
    {
        HelpersIt i = m_helpers.begin();
        while (i != m_helpers.end())
        {
            if (*i == pHelper)
                i = m_helpers.erase(i);
            else
                ++i;
        }
    }


}