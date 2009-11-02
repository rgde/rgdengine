#include "precompiled.h"

#include <rgde/input/device.h>
#include <rgde/input/control.h>

namespace input
{
	device_dx9::device_dx9(types::device eName, int nIndx, input_impl &rInput):
		m_nIndx  (nIndx),
		m_eName  (eName),
		m_rInput (rInput)
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
    Control* device_dx9::get_control(types::control eControlName)
    {
        std::map<types::control,Control*>::iterator pos = m_controls.find(eControlName);

        if (pos == m_controls.end())
          return 0;

        return pos->second;
    }

    Control* device_dx9::get_control(const std::wstring &sControlName)
    {
        return get_control(String2Control(sControlName));
    }

    //есть ли такой контрол
    bool device_dx9::isControlPresent(types::control eControlName) const
    {
        return m_controls.find(eControlName) != m_controls.end();
    }

    bool device_dx9::isControlPresent (const std::wstring &sControlName) const
    {
        return isControlPresent(String2Control(sControlName));
    }

    //добавить кнопку
    void device_dx9::addButton (types::control eControlName)
    {
        Control *pControl = new Control(eControlName, Control::Button, *this);
        m_controls[pControl->get_name()] = pControl;
    }

    //добавить ось
    void device_dx9::addAxis (types::control eControlName)
    {
        Control *pControl = new Control(eControlName, Control::Axis, *this);
        m_controls[pControl->get_name()] = pControl;
    }

    //отвязать команду ото всех контролов
    void device_dx9::detachCommand (CommandPtr pCommand)
    {
        std::map<types::control, Control*>::iterator i = m_controls.begin();

        while (i != m_controls.end())
            i->second->unbind(pCommand);
    }
}