//EventBase.h
#pragma once
#include "base/Singelton.h"

namespace event
{

    class CSenderBase;
    class CListenerBase;

    /////////////////////////////////////
    // базовый класс менеджера событий //
    /////////////////////////////////////
    class CEventManager
    {
    public:
        virtual ~CEventManager(){}

        //отписать данного получателя ото всех менеджеров
        static void unsubscribeAll(CListenerBase *pListener)
        {
            std::list<CEventManager*>::iterator i = ms_managers.begin();
            while (i != ms_managers.end())
            {
                (*i)->unsubscribe(pListener);
                ++i;
            }
        }

    protected:
        //отписать получателя от менеджера (потомка класса CEventManager)
        virtual void unsubscribe (CListenerBase*) = 0;

        //менеджер добавляет себя в общий список менеджеров
        static void addManager (CEventManager *pManager)
        {
            ms_managers.push_back(pManager);
        }

        //менеджер удаляет себя из общего списка менеджеров
        static void delManager (CEventManager *pManager)
        {
            std::list<CEventManager*>::iterator i = ms_managers.begin();
            while (i != ms_managers.end())
            {
                if ((*i) == pManager)
                    i = ms_managers.erase(i);
                else
                    ++i;
            }
        }

    private:
        static std::list<CEventManager*> ms_managers;
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

        //регистрация менеджера в глобальном списке менеджеров
        TEventManager ()
        {
            CEventManager::addManager(this);
        }

        //убираем регистрацию менеджера
       ~TEventManager()
        {
            CEventManager::delManager(this);
        }

    private:
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
            CEventManager::unsubscribeAll(this);
        }

        //подписаться на получение событий
        template <typename Event>
        void _subscribe( boost::function<void(Event)> f, CSenderBase *pSender=0 )
        {
            base::TSingelton<TEventManager<Event> >::Get().subscribe(this,f,pSender);
        }

        //отписаться от получения событий
        template <typename Event>
        void _unsubscribe()
        {
            base::TSingelton<TEventManager<Event> >::Get().unsubscribe(this);
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
            base::TSingelton<TEventManager<Event> >::Get().sendEvent(event,this);
        }

    private:
        CSenderBase(const CSenderBase&);
        CSenderBase& operator= (const CSenderBase&);
    };

} //namespace event