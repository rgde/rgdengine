#include "precompiled.h"

#include <rgde/input/control.h>
#include <rgde/input/device.h>
#include <rgde/input/command.h>
#include "inputimpl.h"

namespace input
{
    Control::Control(types::control name, type eType, device_dx9 &device):
        m_device (device),
        m_type   (eType),
        m_time   (0),
        m_press  (false),
        m_delta  (0),
        m_name   (name)
    {
    }

    void Control::bind (command_ptr pCommand)
    {
        commands_iter pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          pCommand
        );

        if (pos != m_commands.end())
            return;

        m_commands.push_back(pCommand);
    }

    void Control::bind (const std::wstring &command_name)
    {
        bind (m_device.get_impl().getCommand(command_name));
    }

    // remove observer
    void Control::unbind (command_ptr pCommand)
    {
        commands_iter pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          pCommand
        );

        if (pos == m_commands.end())
            return;

        m_commands.erase(pos);
    }

    void Control::unbind (const std::wstring &command_name)
    {
        unbind(m_device.get_impl().getCommand(command_name));
    }

    // check is that observer already added
    bool Control::is_bind (command_ptr pCommand)
    {
        commands_iter pos = std::find
        (
          m_commands.begin(),
          m_commands.end(),
          pCommand
        );

        if (pos == m_commands.end())
            return false;

        return true;
    }

    bool Control::is_bind (const std::wstring &command_name)
    {
        return is_bind(m_device.get_impl().getCommand(command_name));
    }

    // notify observers about control state change
    void Control::notify_all ()
    {
		if (m_commands.empty())
			return;

        commands_iter i = m_commands.begin();

        while (i != m_commands.end())
        {
			command_ptr command = *i;
			if (command)
			{
				command->notify_all(*this);
			}
            ++i;
        }
    }

}