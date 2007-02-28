//Event.h
#pragma once
#include "EventBase.h"

namespace event
{

    ////////////////
    // получатель //
    ////////////////
    class CListener: private CListenerBase
    {
    public:
        //подписаться на получение событий
        //  func    - объект с сигнатурой void(Event)
        //  pSender - от кого хотим получать события (0 или ничего, если хотим получать от всех)
        template <typename Event>
        void subscribe( boost::function<void(Event)> func, CSenderBase *pSender=0 )
        {
            _subscribe<Event>(func,pSender);
        }

        //подписаться на получение событий
        //  ptr     - указатель на член-функцию с сигнатурой void(Event)
        //  pSender - от кого хотим получать события (0 или ничего, если хотим получать от всех)
        template<typename Event, typename Class, typename EventArg>
        void subscribe (void (Class::*ptr)(EventArg), CSenderBase *pSender=0)
        {
            _subscribe<Event>( boost::bind(ptr, static_cast<Class*>(this), _1), pSender );
        }

        //отписаться от получения событий
        template <typename Event>
        void unsubscribe()
        {
            _unsubscribe<Event>();
        }
    };

    /////////////////
    // отправитель //
    /////////////////
    class CSender: public CListener, public CSenderBase
    {
    public:
        //отправить событие
        template<typename Event>
        void sendEvent(Event event)
        {
            _sendEvent<Event>(event);
        }
    };

} //namespace event