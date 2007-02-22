//command.cpp
#include "precompiled.h"
#include "input/command.h"
#include "input/control.h"
#include "input/helper.h"

namespace input
{

    void CCommand::notifyAllObservers (const CControl &rControl)
    {
        if (m_bLocked)
            return;

        std::list<CHelper*>::iterator i = m_helpers.begin();
        while (i != m_helpers.end())
        {
            (*i)->notify(rControl);
            ++i;
        }
    }

    void CCommand::attachObserver (CHelper *pHelper)
    {
        m_helpers.push_back(pHelper);
    }

    void CCommand::detachObserver (CHelper *pHelper)
    {
        std::list<CHelper*>::iterator i = m_helpers.begin();
        while (i != m_helpers.end())
        {
            if (*i == pHelper)
                i = m_helpers.erase(i);
            else
                ++i;
        }
    }


}