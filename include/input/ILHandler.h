//ILHandler.h

//IL - Input Library (by Dub)
//функтор для привязки методов различных классов к команде

#ifndef _ILHandler_
#define _ILHandler_

#include "input/ILBase.h"
#include <cassert>

namespace input
{

  ////////////////////////////////////////
  // базовая структура для обработчиков //
  ////////////////////////////////////////
  class IHandler
  {
  public:
    IHandler () {}
    virtual ~IHandler() {}

    virtual void call (const CEvent             &rArg) = 0;
    virtual void call (const CButtonEvent       &rArg) = 0;
    virtual void call (const CTriggerEvent      &rArg) = 0;
    virtual void call (const CRelativeAxisEvent &rArg) = 0;
    virtual void call (const CAbsoluteAxisEvent &rArg) = 0;

  private:
    IHandler (const IHandler&);
    IHandler& operator= (const IHandler&);
  };

  ///////////////////////////////////////////
  // обработчик для вызова методов классов //
  ///////////////////////////////////////////
  template <typename RECEIVER, typename ARG>
  struct TMethodHandler: public IHandler
  {
    typedef void (RECEIVER::*ACTION)(const ARG&);

    //конструктор
    TMethodHandler
    (
      RECEIVER *pPeceiver,
      ACTION    pAction
    ):
      m_pReceiver (pReceiver),
      m_pAction   (pAction)
    {
    }

    //деструктор
    virtual ~TMethodHandler () {}

  private:
    RECEIVER *pReceiver;
    ACTION    pAtion;

    TMethodHandler (const TMethodHandler&);
    TMethodHandler& operator= (const TMethodHandler&);
  };

  //////////////////////////////////////////////////////////////
  // конкретизации TMethodHandler для разных типов аргументов //
  //////////////////////////////////////////////////////////////

  //конкретизация для CEvent
  template <typename RECEIVER>
  struct TMethodHandler<RECEIVER,CEvent>: public IHandler
  {
    typedef void (RECEIVER::*ACTION)(const CEvent&);

    //конструктор
    TMethodHandler
    (
      RECEIVER *pReceiver,
      ACTION    pAction
    ):
      m_pReceiver (pReceiver),
      m_pAction   (pAction)
    {
    }

    //деструктор
    virtual ~TMethodHandler () {}

    //вызов
    void call (const CEvent             &rArg) {(m_pReceiver->*m_pAction)(        rArg);}
    void call (const CButtonEvent       &rArg) {(m_pReceiver->*m_pAction)((CEvent)rArg);}
    void call (const CTriggerEvent      &rArg) {(m_pReceiver->*m_pAction)((CEvent)rArg);}
    void call (const CRelativeAxisEvent &rArg) {(m_pReceiver->*m_pAction)((CEvent)rArg);}
    void call (const CAbsoluteAxisEvent &rArg) {(m_pReceiver->*m_pAction)((CEvent)rArg);}

  private:
    RECEIVER *m_pReceiver;
    ACTION    m_pAction;

    TMethodHandler (const TMethodHandler&);
    TMethodHandler& operator= (const TMethodHandler&);
  };

  //конкретизация для CButtonEvent
  template <typename RECEIVER>
  struct TMethodHandler<RECEIVER,CButtonEvent>: public IHandler
  {
    typedef void (RECEIVER::*ACTION)(const CButtonEvent&);

    //конструктор
    TMethodHandler
    (
      RECEIVER *pReceiver,
      ACTION    pAction
    ):
      m_pReceiver (pReceiver),
      m_pAction   (pAction)
    {
    }

    //деструктор
    virtual ~TMethodHandler () {}

    //вызов
    void call (const CEvent             &rArg) {}
    void call (const CButtonEvent       &rArg) {(m_pReceiver->*m_pAction)(rArg);}
    void call (const CTriggerEvent      &rArg) {}
    void call (const CRelativeAxisEvent &rArg) {}
    void call (const CAbsoluteAxisEvent &rArg) {}

  private:
    RECEIVER *m_pReceiver;
    ACTION    m_pAction;

    TMethodHandler (const TMethodHandler&);
    TMethodHandler& operator= (const TMethodHandler&);
  };

  //конкретизация для CTriggerEvent
  template <typename RECEIVER>
  struct TMethodHandler<RECEIVER,CTriggerEvent>: public IHandler
  {
    typedef void (RECEIVER::*ACTION)(const CTriggerEvent&);

    //конструктор
    TMethodHandler
    (
      RECEIVER *pReceiver,
      ACTION    pAction
    ):
      m_pReceiver (pReceiver),
      m_pAction  (pAction)
    {
    }

    //деструктор
    virtual ~TMethodHandler () {}

    //вызов
    void call (const CEvent             &rArg) {}
    void call (const CButtonEvent       &rArg) {}
    void call (const CTriggerEvent      &rArg) {(m_pReceiver->*m_pAction)(rArg);}
    void call (const CRelativeAxisEvent &rArg) {}
    void call (const CAbsoluteAxisEvent &rArg) {}

  private:
    RECEIVER *m_pReceiver;
    ACTION    m_pAction;

    TMethodHandler (const TMethodHandler&);
    TMethodHandler& operator= (const TMethodHandler&);
  };

  //конкретизация для CRelativeAxisEvent
  template <typename RECEIVER>
  struct TMethodHandler<RECEIVER,CRelativeAxisEvent>: public IHandler
  {
    typedef void (RECEIVER::*ACTION)(const CRelativeAxisEvent&);

    //конструктор
    TMethodHandler
    (
      RECEIVER *pReceiver,
      ACTION    pAction
    ):
      m_pReceiver (pReceiver),
      m_pAction   (pAction)
    {
    }

    //деструктор
    virtual ~TMethodHandler () {}

    //вызов
    void call (const CEvent             &rArg) {}
    void call (const CButtonEvent       &rArg) {}
    void call (const CTriggerEvent      &rArg) {}
    void call (const CRelativeAxisEvent &rArg) {(m_pReceiver->*m_pAction)(rArg);}
    void call (const CAbsoluteAxisEvent &rArg) {}

  private:
    RECEIVER *m_pReceiver;
    ACTION    m_pAction;

    TMethodHandler (const TMethodHandler&);
    TMethodHandler& operator= (const TMethodHandler&);
  };

  //конкретизация для CAbsoluteAxisEvent
  template <typename RECEIVER>
  struct TMethodHandler<RECEIVER,CAbsoluteAxisEvent>: public IHandler
  {
    typedef void (RECEIVER::*ACTION)(const CAbsoluteAxisEvent&);

    //конструктор
    TMethodHandler
    (
      RECEIVER *pReceiver,
      ACTION    pAction
    ):
      m_pReceiver (pReceiver),
      m_pAction   (pAction)
    {
    }

    //деструктор
    virtual ~TMethodHandler () {}

    //вызов
    void call (const CEvent             &rArg) {}
    void call (const CButtonEvent       &rArg) {}
    void call (const CTriggerEvent      &rArg) {}
    void call (const CRelativeAxisEvent &rArg) {}
    void call (const CAbsoluteAxisEvent &rArg) {(m_pReceiver->*m_pAction)(rArg);}

  private:
    RECEIVER *m_pReceiver;
    ACTION    m_pAction;

    TMethodHandler (const TMethodHandler&);
    TMethodHandler& operator= (const TMethodHandler&);
  };

} //namespace input

#endif //_ILHandler_