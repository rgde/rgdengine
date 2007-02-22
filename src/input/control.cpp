//control.cpp
#include "precompiled.h"
#include "input/inputimpl.h"
#include "input/control.h"
#include "input/device.h"
#include "input/command.h"
#include <algorithm>

namespace input
{

    CControl::CControl(EControl eName, EType eType, CDevice &rDevice):
        m_rDevice (rDevice),
        m_eType   (eType),
        m_nTime   (0),
        m_bPress  (false),
        m_nDelta  (0),
        m_eName   (eName)
    {
    }

    //добавить наблюдателя
    void CControl::bind (PCommand pCommand)
    {
        std::list<PCommand>::iterator pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          pCommand
        );

        if (pos != m_commands.end())
            return;

        m_commands.push_back(pCommand);
    }

    void CControl::bind (const std::wstring &sCommandName)
    {
        bind (m_rDevice.getInput().getCommand(sCommandName));
    }

    //удалить наблюдателя
    void CControl::unbind (PCommand pCommand)
    {
        std::list<PCommand>::iterator pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          pCommand
        );

        if (pos == m_commands.end())
            return;

        m_commands.erase(pos);
    }

    void CControl::unbind (const std::wstring &sCommandName)
    {
        unbind(m_rDevice.getInput().getCommand(sCommandName));
    }

    //добавлен ли такой наблюдатель
    bool CControl::isbind (PCommand pCommand)
    {
        std::list<PCommand>::iterator pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          pCommand
        );

        if (pos == m_commands.end())
            return false;

        return true;
    }

    bool CControl::isbind (const std::wstring &sCommandName)
    {
        return isbind(m_rDevice.getInput().getCommand(sCommandName));
    }

    //уведомить наблюдателей о своем изменении
    void CControl::notifyAllObservers ()
    {
        std::list<PCommand>::iterator i = m_commands.begin();

        while (i != m_commands.end())
        {
            (*i)->notifyAllObservers(*this);
            ++i;
        }
    }

}