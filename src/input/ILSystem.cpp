#include "precompiled.h"

#include "input/ILSystem.h"
#include "input/ILSystemImpl.h"


namespace input
{

  //указатель на единственный экземпл€р
  //по стандарту и так ноль, но все равно спокойней на душе :)
  CSystem* CSystem::ms_pInstance = 0;

  //только дл€ конструировани€ синглтона
  CSystem::CSystem ()
  {
    assert(!ms_pInstance);
    ms_pInstance = this;
  }

  CSystem::~CSystem ()
  {
    assert(ms_pInstance);
    ms_pInstance = 0;
  }

  //получить указатель на синглтон системы ввода
  CSystem& CSystem::Get ()
  {
    if (!ms_pInstance)
      ms_pInstance = new CSystem;
    return *ms_pInstance;
  }

  //удалить синглтон системы ввода
  void CSystem::Destroy ()
  {
    if (ms_pInstance)
      delete ms_pInstance;
    ms_pInstance = 0;
  }

  //инициализировать систему ввода
  void CSystem::init (ISystemImpl *pImpl)
  {
    m_pImpl.reset(pImpl);
  }

  //получить устройство
  CDevice* CSystem::GetDevice (EDevice eDevice)
  {
    return Get().m_pImpl->getDevice(eDevice);
  }

  //проверить, существует ли указанное устройство
  bool CSystem::IsDevicePresent (EDevice eDevice)
  {
    return Get().m_pImpl->isDevicePresent(eDevice);
  }

  //получить контрол
  CControl* CSystem::GetControl (EDevice eDevice, EControl eControl)
  {
    return Get().m_pImpl->getControl(eDevice,eControl);
  }

  //проверить, существует ли указанныйы контрол
  bool CSystem::IsControlPresent (EDevice eDevice, EControl eControl)
  {
    return Get().m_pImpl->isControlPresent(eDevice,eControl);
  }

  //извлечь из буфера все событи€ от устройств ввода
  void CSystem::Update ()
  {
    Get().m_pImpl->update();
  }

} //namespace input
