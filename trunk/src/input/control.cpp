#include "precompiled.h"

#include "input/inputimpl.h"
#include "input/control.h"
#include "input/device.h"
#include "input/command.h"

#include <algorithm>

namespace input
{
    Control::Control(types::EControl eName, EType eType, Device &rDevice):
        m_device (rDevice),
        m_type   (eType),
        m_time   (0),
        m_press  (false),
        m_delta  (0),
        m_name   (eName)
    {
    }

    void Control::bind (CommandPtr pCommand)
    {
        CommandsIter pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          pCommand
        );

        if (pos != m_commands.end())
            return;

        m_commands.push_back(pCommand);
    }

    void Control::bind (const std::wstring &sCommandName)
    {
        bind (m_device.getInput().getCommand(sCommandName));
    }

    //удалить наблюдателя
    void Control::unbind (CommandPtr pCommand)
    {
        CommandsIter pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          pCommand
        );

        if (pos == m_commands.end())
            return;

        m_commands.erase(pos);
    }

    void Control::unbind (const std::wstring &sCommandName)
    {
        unbind(m_device.getInput().getCommand(sCommandName));
    }

    //добавлен ли такой наблюдатель
    bool Control::isbind (CommandPtr pCommand)
    {
        CommandsIter pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          pCommand
        );

        if (pos == m_commands.end())
            return false;

        return true;
    }

    bool Control::isbind (const std::wstring &sCommandName)
    {
        return isbind(m_device.getInput().getCommand(sCommandName));
    }

    //уведомить наблюдателей о своем изменении
    void Control::notifyAllObservers ()
    {
        CommandsIter i = m_commands.begin();

        while (i != m_commands.end())
        {
            (*i)->notifyAllObservers(*this);
            ++i;
        }
    }

}