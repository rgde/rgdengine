#include "stdafx.h"

#include <rgde/input/device.h>
#include <rgde/input/control.h>

namespace rgde
{
namespace input
{
	device::device(types::EDevice eName, int nIndx, input_impl &input):
		m_index  (nIndx),
		m_device_type  (eName),
		m_input (input)
	{
	}

    device::~device()
    {
        while (!m_controls.empty())
        {
          delete m_controls.begin()->second;
          m_controls.erase(m_controls.begin());
        }
    }

    //получить контрол
    control* device::get_control(types::EControl contol_type)
    {
        std::map<types::EControl,control*>::iterator pos = m_controls.find(contol_type);

        if (pos == m_controls.end())
          return 0;

        return pos->second;
    }

    control* device::get_control(const std::wstring &control_name)
    {
		return get_control(string_to_type::control(control_name));
    }

    //есть ли такой контрол
    bool device::is_control_present(types::EControl contol_type) const
    {
        return m_controls.find(contol_type) != m_controls.end();
    }

    bool device::is_control_present (const std::wstring &control_name) const
    {
        return is_control_present(string_to_type::control(control_name));
    }

    //добавить кнопку
    void device::add_button (types::EControl contol_type)
    {
        control *pControl = new control(contol_type, control::button, *this);
        m_controls[pControl->get_name()] = pControl;
    }

    //добавить ось
    void device::add_axis (types::EControl contol_type)
    {
        control *pControl = new control(contol_type, control::axis, *this);
        m_controls[pControl->get_name()] = pControl;
    }

    //отвязать команду ото всех контролов
    void device::detach_command (command_ptr command)
    {
        std::map<types::EControl, control*>::iterator i = m_controls.begin();

        while (i != m_controls.end())
            i->second->unbind(command);
    }
}
}