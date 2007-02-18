#include "precompiled.h"

#include "input/ILSystemImpl.h"
#include "input/ILControl.h"
#include "input/ILDevice.h"


namespace input
{

  ISystemImpl::~ISystemImpl ()
  {
    while (!m_mapDevices.empty())
    {
      delete m_mapDevices.begin()->second;
      m_mapDevices.erase(m_mapDevices.begin());
    }
  }

  //получить устройство
  CDevice* ISystemImpl::getDevice (EDevice eDevice)
  {
    std::map<EDevice,CDevice*>::const_iterator pos = m_mapDevices.find(eDevice);

    if (pos == m_mapDevices.end())
      return 0;

    return pos->second;
  }

  //проверить, существует ли указанное устройство
  bool ISystemImpl::isDevicePresent (EDevice eDevice)
  {
    return m_mapDevices.find(eDevice) != m_mapDevices.end();
  }

  //получить контрол
  CControl* ISystemImpl::getControl (EDevice eDevice, EControl eControl)
  {
    CDevice *d = getDevice(eDevice);

    if (!d)
      return 0;
    
    return d->getControl(eControl);
  }

  //проверить, существует ли указанныйы контрол
  bool ISystemImpl::isControlPresent (EDevice eDevice, EControl eControl)
  {
    CDevice *d = getDevice(eDevice);

    if (!d)
      return false;
    
    return d->isControlPresent(eControl);
  }

  //только этот метод может создать новый обьект устройства ввода
  //этот метод могут использовать только потомки InputSystemImpl
  CDevice* ISystemImpl::newDevice (EDevice eDevice)
  {
    assert(!isDevicePresent(eDevice));

    CDevice *pDevice = new CDevice(eDevice);
    m_mapDevices[eDevice] = pDevice;
    return pDevice;
  }

  //только этот метод может добавить контрол к устройству ввода
  //этот метод могут использовать только потомки InputSystemImpl
  void ISystemImpl::addControl (EDevice eDevice, CControl *pControl)
  {
    assert(isDevicePresent(eDevice) && pControl);
    assert(!getDevice(eDevice)->isControlPresent(pControl->getName()));

    CDevice *d = getDevice(eDevice);
    d->m_mapControls[pControl->getName()] = pControl;
  }

}