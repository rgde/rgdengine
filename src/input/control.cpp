#include "stdafx.h"

#include <rgde/input/control.h>
#include <rgde/input/device.h>
#include <rgde/input/command.h>
#include "inputimpl.h"

namespace rgde
{
namespace input
{
    control::control(types::EControl eName, EType eType, device &rDevice):
        m_device (rDevice),
        m_type   (eType),
        m_time   (0),
        m_press  (false),
        m_delta  (0),
        m_name   (eName)
    {
    }

    void control::bind (command_ptr command)
    {
        commands_iter pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          command
        );

        if (pos != m_commands.end())
            return;

        m_commands.push_back(command);
    }

    void control::bind (const std::wstring &command_name)
    {
        bind (m_device.get_input().get_command(command_name));
    }

    //удалить наблюдателя
    void control::unbind (command_ptr command)
    {
        commands_iter pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          command
        );

        if (pos == m_commands.end())
            return;

        m_commands.erase(pos);
    }

    void control::unbind (const std::wstring &command_name)
    {
        unbind(m_device.get_input().get_command(command_name));
    }

    //добавлен ли такой наблюдатель
    bool control::is_bind (command_ptr command)
    {
        commands_iter pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          command
        );

        if (pos == m_commands.end())
            return false;

        return true;
    }

    bool control::is_bind (const std::wstring &command_name)
    {
        return is_bind(m_device.get_input().get_command(command_name));
    }

    //уведомить наблюдателей о своем изменении
    void control::notify_all ()
    {
        commands_iter i = m_commands.begin();

        while (i != m_commands.end())
        {
            (*i)->notify_all(*this);
            ++i;
        }
    }

}
}