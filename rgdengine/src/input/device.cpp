#include "precompiled.h"

#include <rgde/input/device.h>
#include <rgde/input/control.h>

namespace input
{
	device_dx9::device_dx9(types::device name, int index, input_impl &input):
		m_index  (index),
		m_name  (name),
		m_input (input)
	{
	}

    device_dx9::~device_dx9()
    {
        while (!m_controls.empty())
        {
          delete m_controls.begin()->second;
          m_controls.erase(m_controls.begin());
        }
    }

    //получить контрол
    Control* device_dx9::get_control(types::control contol_name)
    {
        std::map<types::control,Control*>::iterator pos = m_controls.find(contol_name);

        if (pos == m_controls.end())
          return 0;

        return pos->second;
    }

    Control* device_dx9::get_control(const std::wstring &contol_name)
    {
        return get_control(String2Control(contol_name));
    }

    //есть ли такой контрол
    bool device_dx9::is_control_present(types::control contol_name) const
    {
        return m_controls.find(contol_name) != m_controls.end();
    }

    bool device_dx9::is_control_present (const std::wstring &contol_name) const
    {
        return is_control_present(String2Control(contol_name));
    }

    //добавить кнопку
    void device_dx9::add_button (types::control contol_name)
    {
        Control *pControl = new Control(contol_name, Control::Button, *this);
        m_controls[pControl->get_name()] = pControl;
    }

    //добавить ось
    void device_dx9::add_axis (types::control contol_name)
    {
        Control *pControl = new Control(contol_name, Control::Axis, *this);
        m_controls[pControl->get_name()] = pControl;
    }

    //отвязать команду ото всех контролов
    void device_dx9::detach_command (command_ptr pCommand)
    {
        std::map<types::control, Control*>::iterator i = m_controls.begin();

        while (i != m_controls.end())
            i->second->unbind(pCommand);
    }
}