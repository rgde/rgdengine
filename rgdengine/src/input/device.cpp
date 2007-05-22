#include "precompiled.h"

#include <rgde/input/device.h>
#include <rgde/input/control.h>

namespace input
{
	Device::Device(types::EDevice eName, int nIndx, InputImpl &rInput):
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
    Control* Device::getControl(types::EControl eControlName)
    {
        std::map<types::EControl,Control*>::iterator pos = m_controls.find(eControlName);

        if (pos == m_controls.end())
          return 0;

        return pos->second;
    }

    Control* Device::getControl(const std::wstring &sControlName)
    {
        return getControl(String2Control(sControlName));
    }

    //есть ли такой контрол
    bool Device::isControlPresent(types::EControl eControlName) const
    {
        return m_controls.find(eControlName) != m_controls.end();
    }

    bool Device::isControlPresent (const std::wstring &sControlName) const
    {
        return isControlPresent(String2Control(sControlName));
    }

    //добавить кнопку
    void Device::addButton (types::EControl eControlName)
    {
        Control *pControl = new Control(eControlName, Control::Button, *this);
        m_controls[pControl->getName()] = pControl;
    }

    //добавить ось
    void Device::addAxis (types::EControl eControlName)
    {
        Control *pControl = new Control(eControlName, Control::Axis, *this);
        m_controls[pControl->getName()] = pControl;
    }

    //отвязать команду ото всех контролов
    void Device::detachCommand (CommandPtr pCommand)
    {
        std::map<types::EControl, Control*>::iterator i = m_controls.begin();

        while (i != m_controls.end())
            i->second->unbind(pCommand);
    }
}