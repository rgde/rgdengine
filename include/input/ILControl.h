//IL - Input Library
///элемент устройства ввода (является наблюдаемым для Command)
#pragma once

#include "input/ILBase.h"

namespace input
{
  /// элемент устройства ввода
  class CControl
  {
  public:

	//виды контролов 
    enum EType
    {
      Button, //кнопка
      Axis,   //ось
    };

    explicit CControl (EControl eName, CControl::EType eType, const CDevice &rDevice):
      m_eName   (eName),
      m_eType   (eType),
      m_rDevice (rDevice),
      m_bPress  (false),
      m_nDelta  (0),
      m_nTime   (0)
    {
    }

    virtual ~CControl ();

    //получение информации о контроле
    EControl        getName   () const {return m_eName;};   //получить enum контрола
    CControl::EType getType   () const {return m_eType;};   //получить тип контрола
    const CDevice&  getDevice () const {return m_rDevice;}; //получить устройство-хозяин контрола

    //доступ к состоянию контрола
    int  getTime  () const      {return m_nTime;}
    void setTime  (int nTime)   {m_nTime = nTime;}
    bool getPress () const      {return m_bPress;}
    void setPress (bool bPress) {m_bPress = bPress;}
    int  getDelta () const      {return m_nDelta;}
    void setDelta (int nDelta)  {m_nDelta = nDelta;}

    //Control является subject'ом в связке subject-observer
    //где observer'ом является класс CCommand. Методы для этой
    //связки:
    bool attachObserver     (CCommand*); //добавить наблюдателя
    void detachObserver     (CCommand*); //удалить наблюдателя
    void notifyAllObservers ();          //уведомить наблюдателей о своем изменении

  private:
    //данные контрола
    EControl       m_eName;   //enum контрола
    EType          m_eType;   //тип контрола
    int            m_nTime;   //время последнего обновления контрола
    const CDevice &m_rDevice; //устройство, которому принадлежит контрол

    //список наблюдателей для данного контрола
    std::list<CCommand*> m_listObservers;

    //данные, зависящие от типа котрола
    //(неохота создавать потомков ради добавления одной переменной)
    bool m_bPress;
    int  m_nDelta;
  };
}