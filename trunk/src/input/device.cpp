//device.cpp
#include "precompiled.h"
#include "input/device.h"
#include "input/control.h"

namespace input
{

    CDevice::~CDevice()
    {
        while (!m_controls.empty())
        {
          delete m_controls.begin()->second;
          m_controls.erase(m_controls.begin());
        }
    }

    //получить контрол
    CControl* CDevice::getControl (EControl eControlName)
    {
        std::map<EControl,CControl*>::iterator pos = m_controls.find(eControlName);

        if (pos == m_controls.end())
          return 0;

        return pos->second;
    }

    CControl* CDevice::getControl (const std::wstring &sControlName)
    {
        return getControl(String2Control(sControlName));
    }

    //есть ли такой контрол
    bool CDevice::isControlPresent (EControl eControlName) const
    {
        return m_controls.find(eControlName) != m_controls.end();
    }

    bool CDevice::isControlPresent (const std::wstring &sControlName) const
    {
        return isControlPresent(String2Control(sControlName));
    }

    //добавить кнопку
    void CDevice::addButton (EControl eControlName)
    {
        CControl *pControl = new CControl(eControlName, CControl::Button, *this);
        m_controls[pControl->getName()] = pControl;
    }

    //добавить ось
    void CDevice::addAxis (EControl eControlName)
    {
        CControl *pControl = new CControl(eControlName, CControl::Axis, *this);
        m_controls[pControl->getName()] = pControl;
    }

    //отвязать команду ото всех контролов
    void CDevice::detachCommand (PCommand pCommand)
    {
        std::map<EControl, CControl*>::iterator i = m_controls.begin();

        while (i != m_controls.end())
            i->second->unbind(pCommand);
    }

}