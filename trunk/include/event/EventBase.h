//EventBase.h
#pragma once
#include "base/Singelton.h"

namespace event
{

    class CSenderBase;
    class CListenerBase;
    class CEventManager;

    /////////////////////////////////////////
    // синглтон со списком всех менеджеров //
    /////////////////////////////////////////
    class CListManagers
    {
    public:
        static CListManagers& Get();
        void unsubscribeAll (CListenerBase *pListener); //отписать данного получателя ото всех менеджеров
        void addManager     (CEventManager *pManager);  //менеджер добавляет себя в общий список менеджеров
        void delManager     (CEventManager *pManager);  //менеджер удаляет себя из общего списка менеджеров

    private:
        CListManagers();
       ~CListManagers();

        std::list<CEventManager*> m_managers;
    };

    /////////////////////////////////////
    // базовый класс менеджера событий //
    /////////////////////////////////////
    class CEventManager
    {
    public:
        virtual ~CEventManager(){}

        //отписать получателя от менеджера (потомка класса CEventManager)
        virtual void unsubscribe (CListenerBase*) = 0;
    };

    ///////////////////////////////////////
    // шаблонный класс менеджера событий //
    ///////////////////////////////////////
    template <typename Event>
    class TEventManager: public CEventManager
    {
        //подписка на получение событий
        struct Subscription
        {
            CListenerBase                *m_pListener; //кто хочет получать события
            CSenderBase                  *m_pSender;   //от кого хочет получать события (0 - от всех)
            boost::function<void(Event)>  m_func;      //какой метод вызывать

            bool operator==(const Subscription &s)
            {
                //->
                //return
                //(
                //    m_pListener == s.m_pListener &&    
                //    m_pSender   == s.m_pSender   &&
                //    m_func      == s.m_func
                //);
                return m_pListener == s.m_pListener;
                //-<
            }
        };

    public:
        //синглтон епт
        static TEventManager& Get()
        {
            static TEventManager instance;
            return instance;
        }

        //подписать pListener на получение событий в функтор func от отправителя pSender (если равен 0 - то от всех)
        void subscribe (CListenerBase *pListener, boost::function<void(Event)> func, CSenderBase *pSender)
        {
            Subscription subs;
            subs.m_pListener = pListener;
            subs.m_func      = func;
            subs.m_pSender   = pSender;
            m_subscriptions.push_back(subs);
        }

        //отписать pListener от получения всех событий типа Event
        void unsubscribe (CListenerBase *pListener)
        {
            std::list<Subscription>::iterator i = m_subscriptions.begin();
            while (i != m_subscriptions.end())
            {
                if (i->m_pListener == pListener)
                    i = m_subscriptions.erase(i);
                else
                    ++i;
            }
        }

        //отправить событие event от отправителя pSender
        void sendEvent (Event event, const CSenderBase *pSender)
        {
            std::list<Subscription> tmp(m_subscriptions.begin(), m_subscriptions.end());

            std::list<Subscription>::iterator i = tmp.begin();
            while (i != tmp.end())
            {
                if (std::find(m_subscriptions.begin(), m_subscriptions.end(), *i) != m_subscriptions.end())
                {
                    if (i->m_pSender == 0 || i->m_pSender == pSender)
                        i->m_func(event);
                    ++i;
                }
            }
        }

    private:
        //регистрация менеджера в глобальном списке менеджеров
        TEventManager ()
        {
            CListManagers::Get().addManager(this);
        }

        //убираем регистрацию менеджера
       ~TEventManager()
        {
            CListManagers::Get().delManager(this);
        }

        TEventManager(const TEventManager&);
        TEventManager& operator= (const TEventManager&);

        std::list<Subscription> m_subscriptions;
    };    

    //////////////////////////////
    // базовый класс получателя //
    //////////////////////////////
    class CListenerBase
    {
    protected:
        CListenerBase() {}

        //отписать получателя от всех менеджеров
        virtual ~CListenerBase()
        {
            CListManagers::Get().unsubscribeAll(this);
        }

        //подписаться на получение событий
        template <typename Event>
        void _subscribe( boost::function<void(Event)> f, CSenderBase *pSender=0 )
        {
            TEventManager<Event>::Get().subscribe(this,f,pSender);
        }

        //отписаться от получения событий
        template <typename Event>
        void _unsubscribe()
        {
            TEventManager<Event>::Get().unsubscribe(this);
        }

    private:
        CListenerBase(const CListenerBase&);
        CListenerBase& operator= (const CListenerBase&);
    };

    ///////////////////////////////
    // базовый класс отправителя //
    ///////////////////////////////
    class CSenderBase
    {
    protected:
        CSenderBase() {}

        //отправить событие
        template<typename Event>
        void _sendEvent(Event event)
        {
            TEventManager<Event>::Get().sendEvent(event,this);
        }

    private:
        CSenderBase(const CSenderBase&);
        CSenderBase& operator= (const CSenderBase&);
    };

} //namespace event