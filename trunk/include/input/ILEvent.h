#pragma once

#include "input/ILBase.h"

namespace input
{
  ///////////////////////////
  // базовое событие ввода //
  ///////////////////////////
  class CEvent
  {
  public:
    //виды событий ввода
    enum EType
    {
      Press            =  1, //НАжатие
      Release          =  2, //ОТжатие
      TriggerOn        =  4, //триггер Включен
      TriggerOff       =  8, //триггер ВЫключен
      RelativeRotation = 16, //относительно изменение координаты оси (дельта)
      AbsoluteRotation = 32, //абсолютное изменение координаты оси (координата)

      All              = 63, //маска для всех видов событий
      InvalidEvent     = 64, //для отладки - таких событий быть не должно
    };

    explicit CEvent (const CDevice &rDevice, const CControl &rControl):
      m_rDevice  (rDevice),
      m_rControl (rControl),
      m_eCommand (InvalidCommand),
      m_eType    (InvalidEvent)
    {
    }

    //общие данные для любого типа событий ввода
    const CDevice  &m_rDevice;  //устройство, от которого пришло событие
    const CControl &m_rControl; //контрол, от которого пришло событие
    ECommand        m_eCommand; //имя команды, для которой сгенерирован данный эвент
    EType           m_eType;    //тип события
    int             m_nTime;    //время генерации события
  };

  ///////////////////////////////////////////////
  // событие, гененрируемое для CButtonCommand //
  ///////////////////////////////////////////////
  class CButtonEvent: public CEvent
  {
  public:
    explicit CButtonEvent
    (
      const CDevice        &rDevice,
      const CControl       &rControl,
      const CButtonCommand &rCommand
    ):
      CEvent     (rDevice,rControl),
      m_rCommand (rCommand),
      m_bPress   (false)
    {
    }

    //данные для кнопки
    const CButtonCommand &m_rCommand; //ссылка на команду для кнопки
    bool m_bPress;                    //состояние кнопки
  };

  ////////////////////////////////////////////////
  // событие, гененрируемое для CTriggerCommand //
  ////////////////////////////////////////////////
  class CTriggerEvent: public CEvent
  {
  public:
    explicit CTriggerEvent
    (
      const CDevice         &rDevice,
      const CControl        &rControl,
      const CTriggerCommand &rCommand
    ):
      CEvent     (rDevice,rControl),
      m_rCommand (rCommand),
      m_bOn      (false)
    {
    }

    //данные для триггера
    const CTriggerCommand &m_rCommand; //ссылка на команду для триггера
    bool m_bOn;                        //состояние триггера
  };

  /////////////////////////////////////////////////////
  // событие, гененрируемое для CRelativeAxisCommand //
  /////////////////////////////////////////////////////
  class CRelativeAxisEvent: public CEvent
  {
  public:
    explicit CRelativeAxisEvent
    (
      const CDevice              &rDevice,
      const CControl             &rControl,
      const CRelativeAxisCommand &rCommand
    ):
      CEvent     (rDevice,rControl),
      m_rCommand (rCommand),
      m_nDelta   (0)
    {
    }

    //данные для относительного вращения оси
    const CRelativeAxisCommand &m_rCommand; //ссылка на команду для относительного вращения оси
    int m_nDelta;                           //сдвиг оси
  };

  //////////////////////////////////////////////////////
  // событие, гененрируемое для CAbsoluteAxisCommands //
  //////////////////////////////////////////////////////
  class CAbsoluteAxisEvent: public CEvent
  {
  public:
    explicit CAbsoluteAxisEvent
    (
      const CDevice              &rDevice,
      const CControl             &rControl,
      const CAbsoluteAxisCommand &rCommand
    ):
      CEvent     (rDevice,rControl),
      m_rCommand (rCommand),
      m_nCoord   (0),
      m_nMinimum (0),
      m_nMaximum (0)
    {
    }

    //данные для абсолютного вращения оси
    const CAbsoluteAxisCommand &m_rCommand; //ссылка на команду для абсолютного вращения оси
    int m_nCoord;                           //координата оси
    int m_nMinimum;                         //минимальное значение для оси
    int m_nMaximum;                         //максимальное значение для оси
  };
}