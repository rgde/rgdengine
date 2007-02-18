//ILCommand.h

//IL - Input Library (by Dub)
//команда для системы ввода (является наблюдателем для CControl)

#ifndef _ILCommand_
#define _ILCommand_

#include "input/ILBase.h"
#include "input/ILHandler.h"
#include "input/ILCommandEnum.h"
#include <list>

namespace input
{

  ///////////////////////////
  // базовый класс команды //
  ///////////////////////////
  class CCommand
  {
  public:
    //виды команд
    enum EType
    {
      Button,       //кнопка
      Trigger,      //триггер
      RelativeAxis, //относительное изменение оси (дельта)
      AbsoluteAxis, //абсолютное изменение оси (координата)
    };

    //деструктор
    virtual ~CCommand ()
    {
      while (!m_listHandlers.empty())
      {
        delete *m_listHandlers.begin();
        m_listHandlers.erase(m_listHandlers.begin());
      }
      detachAll();
    }

    //получение информации о команде
    ECommand        getName () const {return m_eName;}; //получить enum команды
    CCommand::EType getType () const {return m_eType;}; //получить тип команды

    //работа со связкой команда-контролы
    void attachToControl (ECommand);
    void attachToControl (EDevice, EControl);
    void attachToControl (CControl*);

    void detachFromControl (EDevice, EControl);
    void detachFromControl (CControl*);

    bool isAttachedToControl (EDevice, EControl);
    bool isAttachedToControl (CControl*);

    void detachAll ();

    //вызвать обработку всех хандлеров
    template <typename ARG>
    void call (const ARG &rArg)
    {
      std::list<IHandler*>::const_iterator i = m_listHandlers.begin(),
        end = m_listHandlers.end();
      while (i!=end)
      {
        (*i)->call(rArg);
        ++i;
      }
    }

    //класс Command является наблюдателем класса Control
    //метод Notify вызывается при изменении состояния Control
    virtual void notify (const CControl*) = 0;

  protected:
    //добавить обработчик
    void pushHandler(IHandler *pHandler) {m_listHandlers.push_back(pHandler);}

    //защищенный конструктор (создавать можно только потомков класса Command)
    explicit CCommand (EType eType):
      m_eType (eType),
      m_eName (InvalidCommand)
    {
    }

  private:
    //данные для команды
    ECommand             m_eName;        //enum команды
    std::list<CControl*> m_listControls; //контролы, к которым привязана команда
    EType                m_eType;        //тип команды

    //список обрабочиков
    std::list<IHandler*> m_listHandlers;

    //запрет создания копий
    CCommand (const CCommand&);
    CCommand& operator= (const CCommand&);
  };

  ////////////////////
  // команда кнопки //
  ////////////////////
  class CButtonCommand: public CCommand
  {
  public:
    //коструктор
    CButtonCommand(): CCommand(CCommand::Button), m_bPress(false) {}

    //добавить обработчики для команды
    template <typename RECIVER>
    void addHandler (RECIVER *pReciver, void (RECIVER::*pAction)(const CEvent&))
    {
      pushHandler(new TMethodHandler<RECIVER,CEvent>(pReciver,pAction));
    }

    template <typename RECIVER>
    void addHandler (RECIVER *pReciver, void (RECIVER::*pAction)(const CButtonEvent&))
    {
      pushHandler(new TMethodHandler<RECIVER,CButtonEvent>(pReciver,pAction));
    }

    //класс Command является наблюдателем класса CControl
    //метод Notify вызывается при изменении состояния CControl
    void notify(const CControl*);

    //опрос состояния команды
    operator bool () const {return m_bPress;}

  private:
    bool m_bPress; //нажата ли кнопка
  };

  ///////////////////////////
  // команда переключателя //
  ///////////////////////////
  class CTriggerCommand: public CCommand
  {
  public:
    //коструктор
    CTriggerCommand(): CCommand(CCommand::Trigger), m_bOn(false) {}

    //добавить обработчики для команды
    template <typename RECIVER>
    void addHandler (RECIVER *pReciver, void (RECIVER::*pAction)(const CEvent&))
    {
      pushHandler(new TMethodHandler<RECIVER,CEvent>(pReciver,pAction));
    }

    template <typename RECIVER>
    void addHandler (RECIVER *pReciver, void (RECIVER::*pAction)(const CTriggerEvent&))
    {
      pushHandler(new TMethodHandler<RECIVER,CTriggerEvent>(pReciver,pAction));
    }

    //класс Command является наблюдателем класса Control
    //метод Notify вызывается при изменении состояния Control
    void notify(const CControl*);

    //опрос состояния команды
    operator bool () const {return m_bOn;}

    //задание состояния триггера
    void setState (bool on) {m_bOn = on;}

  private:
    bool m_bOn; //влючен ли триггер
  };

  ///////////////////////////////
  // команда относительной оси //
  ///////////////////////////////
  class CRelativeAxisCommand: public CCommand
  {
  public:
    //коструктор
    CRelativeAxisCommand(): CCommand(CCommand::RelativeAxis), m_nDelta(0) {}

    //добавить обработчики для команды
    template <typename RECIVER>
    void addHandler (RECIVER *pReciver, void (RECIVER::*pAction)(const CEvent&))
    {
      pushHandler(new TMethodHandler<RECIVER,CEvent>(pReciver,pAction));
    }

    template <typename RECIVER>
    void addHandler (RECIVER *pReciver, void (RECIVER::*pAction)(const CRelativeAxisEvent&))
    {
      pushHandler(new TMethodHandler<RECIVER,CRelativeAxisEvent>(pReciver,pAction));
    }

    //класс Command является наблюдателем класса Control
    //метод Notify вызывается при изменении состояния Control
    void notify(const CControl*);

    //опрос состояния команды
    operator int () const {return m_nDelta;}
    int getDelta () const {return m_nDelta;}

  private:
    int m_nDelta; //последний сдвиг относительной оси
  };

  ////////////////////////////
  // команда абсолютной оси //
  ////////////////////////////
  class CAbsoluteAxisCommand: public CCommand
  {
  public:
    //коструктор
    CAbsoluteAxisCommand(): CCommand(CCommand::AbsoluteAxis), m_nCoord(50), m_nMinimum(0), m_nMaximum(100) {}

    //добавить обработчики для команды
    template <typename RECIVER>
    void addHandler (RECIVER *pReciver, void (RECIVER::*pAction)(const CEvent&))
    {
      pushHandler(new TMethodHandler<RECIVER,CEvent>(pReciver,pAction));
    }

    template <typename RECIVER>
    void addHandler (RECIVER *pReciver, void (RECIVER::*pAction)(const CAbsoluteAxisEvent&))
    {
      pushHandler(new TMethodHandler<RECIVER,CAbsoluteAxisEvent>(pReciver,pAction));
    }

    //класс Command является наблюдателем класса Control
    //метод Notify вызывается при изменении состояния Control
    void notify(const CControl*);

    //опрос состояния команды
    operator int   () const {return m_nCoord;}
    int getCoord   () const {return m_nCoord;}
    int getMinimum () const {return m_nMinimum;}
    int getMaximum () const {return m_nMaximum;}

    //настроить абсолютную ось
    void setState (int minimum, int maximum, int coord)
    {
      m_nMinimum = minimum < maximum ? minimum : maximum;
      m_nMaximum = minimum < maximum ? maximum : minimum;
      m_nCoord   = coord < m_nMinimum ? m_nMinimum : coord > m_nMaximum ? m_nMaximum : coord;
    }

  private:
    int m_nCoord;   //координата абсолютной оси
    int m_nMinimum; //минимальное значение координаты для абсолютной оси
    int m_nMaximum; //максимальное значение координаты для абсолютной оси
  };

} //namespace input

#endif //_ILCommand_