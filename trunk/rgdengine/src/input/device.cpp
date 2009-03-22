#include "precompiled.h"

#include <rgde/input/device.h>
#include <rgde/input/control.h>

namespace input
{
	Device::Device(types::device eName, int nIndx, input_impl &rInput):
		m_nIndx  (nIndx),
		m_eName  (eName),
		m_rInput (rInput)
	{
	}

    Device::~Device()
    {
        while (!m_controls.empty())
        {
          delete m_controls.begin()->second;
          m_controls.erase(m_controls.begin());
        }
    }

    //получить контрол
    Control* Device::get_control(types::control eControlName)
    {
        std::map<types::control,Control*>::iterator pos = m_controls.find(eControlName);

        if (pos == m_controls.end())
          return 0;

        return pos->second;
    }

    Control* Device::get_control(const std::wstring &sControlName)
    {
        return get_control(String2Control(sControlName));
    }

    //есть ли такой контрол
    bool Device::isControlPresent(types::control eControlName) const
    {
        return m_controls.find(eControlName) != m_controls.end();
    }

    bool Device::isControlPresent (const std::wstring &sControlName) const
    {
        return isControlPresent(String2Control(sControlName));
    }

    //добавить кнопку
    void Device::addButton (types::control eControlName)
    {
        Control *pControl = new Control(eControlName, Control::Button, *this);
        m_controls[pControl->get_name()] = pControl;
    }

    //добавить ось
    void Device::addAxis (types::control eControlName)
    {
        Control *pControl = new Control(eControlName, Control::Axis, *this);
        m_controls[pControl->get_name()] = pControl;
    }

    //отвязать команду ото всех контролов
    void Device::detachCommand (CommandPtr pCommand)
    {
        std::map<types::control, Control*>::iterator i = m_controls.begin();

        while (i != m_controls.end())
            i->second->unbind(pCommand);
    }
}