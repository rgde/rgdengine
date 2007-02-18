//ILSystemImpl.h

//IL - Input Library (by Dub)
//класс-предок для конкретной реализации системы ввода

#ifndef _ILSystemImpl_
#define _ILSystemImpl_

#include "input/ILBase.h"
#include <map>

namespace input
{

  //////////////////////////////////////////////////////////
  // класс-предок для конкретной реализации системы ввода //
  //////////////////////////////////////////////////////////
  class ISystemImpl
  {
  public:
    ISystemImpl () {}
    virtual ~ISystemImpl ();

    //доступ к устройствам ввода
    CDevice* getDevice       (EDevice); //получить устройство
    bool     isDevicePresent (EDevice); //проверить, существует ли указанное устройство

    //доступ к контролам
    CControl* getControl       (EDevice, EControl); //получить контрол
    bool      isControlPresent (EDevice, EControl); //проверить, существует ли указанныйы контрол

    //апдейт системы ввода (получение информации от
    //устройств ввода и рассылка их по контролам)
    virtual void update () = 0;

  protected:
    //только этот метод может создать новый обьект устройства ввода
    //этот метод могут использовать только потомки ISystemImpl
    CDevice* newDevice (EDevice);

    //только этот метод может добавить котрол к устройчтву ввода
    //этот метод могут использовать только потомки ISystemImpl
    void addControl (EDevice, CControl*);

  private:
    //карта устройств
    std::map<EDevice,CDevice*> m_mapDevices;

    //запрет конструктора копирования и оператора присваивания
    ISystemImpl (const ISystemImpl&);
    ISystemImpl& operator= (const ISystemImpl&);
  };

} //namespace input

#endif //_ILSystemImpl_


