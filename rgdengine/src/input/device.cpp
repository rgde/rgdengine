#include "precompiled.h"

#include <rgde/input/device.h>
#include <rgde/input/control.h>

namespace input
{
	device::device(devices type, int index, input_impl &input):
		m_index  (index),
		m_type  (type),
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
    Control* device::get_control(controls contol_name)
    {
        std::map<controls,Control*>::iterator pos = m_controls.find(contol_name);

        if (pos == m_controls.end())
          return 0;

        return pos->second;
    }

    Control* device::get_control(const std::wstring &contol_name)
    {
        return get_control(String2Control(contol_name));
    }

    //есть ли такой контрол
    bool device::is_control_present(controls contol_name) const
    {
        return m_controls.find(contol_name) != m_controls.end();
    }

    bool device::is_control_present (const std::wstring &contol_name) const
    {
        return is_control_present(String2Control(contol_name));
    }

    //добавить кнопку
    void device::add_button (controls contol_name)
    {
        Control *pControl = new Control(contol_name, Control::Button, *this);
        m_controls[pControl->get_name()] = pControl;
    }

    //добавить ось
    void device::add_axis (controls contol_name)
    {
        Control *pControl = new Control(contol_name, Control::Axis, *this);
        m_controls[pControl->get_name()] = pControl;
    }

    //отвязать команду ото всех контролов
    void device::detach_command (command_ptr pCommand)
    {
        std::map<controls, Control*>::iterator i = m_controls.begin();

        while (i != m_controls.end())
            i->second->unbind(pCommand);
    }
}