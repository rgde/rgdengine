#pragma once

namespace event
{
    class CListener;
    class CActor;
    class CEvent;
    class CNamedEvent;

    //базовый класс для менеджера событий
    class IEventManager
    {
    public:
        IEventManager(){}
        virtual ~IEventManager(){}

        //все менеджеры должны уметь избавляться от подписчиков
        virtual void unsubscribe(CListener*)=0;

        //все! убираю все менеджеры и уезжаю в отпуск!!! (конец работе системы евентов)
        static void DeleteAllManagers();

        //отписать листенера от всех менеджеров
        static void Unsubscribe(CListener*);

    protected:
        //нужно для того чтобы все работало
		typedef std::list<IEventManager*> EventManagers;
		static EventManagers ms_listInstances;

    private:
        //закрыть
        IEventManager(const IEventManager&);
        IEventManager& operator=(const IEventManager&);
    };

    //шаблонный класс для менеджера событий
    template<typename EVENT>
    class TEventManager: public IEventManager
    {
    public:
        //кто и от кого получает евент
        //CActor* может быть нулевым
        //тогда CListener* получает сообщение от любого CActor'а
        typedef std::pair<CListener*,CActor*> ToFrom;

        //получатель события
        typedef std::pair<ToFrom, boost::function<void(EVENT)> > Receiver;

        //список получателей события
        typedef std::list<Receiver> Receivers;

        //получить синглтон
        static TEventManager& Get();

        //подписка получателя
        void subscribe(CListener*, CActor*, boost::function<void(EVENT)>);

        //отписка получателя
        void unsubscribe(CListener*);

        //посылка сообщения
        void sendEvent(EVENT);

    private:
        TEventManager();
        ~TEventManager();

        //закрыть
        TEventManager(const TEventManager&);
        TEventManager& operator=(const TEventManager&);

        //нужно для того чтобы все работало
		static TEventManager *ms_pInstance;
		Receivers m_listReceivers;
    };

	template <typename EVENT>
	TEventManager<EVENT>* TEventManager<EVENT>::ms_pInstance = 0;

    //получатель
    class CListener
    {
    public:
        CListener(){}
        virtual ~CListener();

        //подписаться
        template<typename EVENT>
        void subscribe(boost::function<void(EVENT)>, CActor *pActor=0);

        //подписаться
        template<typename EVENT, typename LISTENER>
        void subscribe(LISTENER*, void (LISTENER::*pFunc)(EVENT), CActor *pActor=0);

        //подписка для именованных евентов
        void subscribe(const std::string&, boost::function<void(const std::string&, const std::string&)>, CActor *pActor=0);

        //подписка для именованных евентов
        template<typename LISTENER>
        void subscribe(const std::string&, LISTENER*, void (LISTENER::*pFunc)(const std::string&, const std::string&), CActor *pActor=0);

        //отписаться
        template<typename EVENT>
        void unsubscribe();

        //отписаться от именованного евента
        void unsubscribe(const std::string&);

        //отписаться от всех
        void unsubscribe();

    private:
        //закрыть
        CListener(const CListener&);
        CListener& operator=(const CListener&);
    };

    //получатель и отправитель
    class CActor: public CListener
    {
    public:
        CActor(){}

        //послать событие
        template<typename EVENT>
        void sendEvent(EVENT);

        //послать именованное событие
        void sendEvent(const std::string&, const std::string&);

    private:
        //закрыть
        CActor(const CActor&);
        CActor& operator=(const CActor&);
    };

    //базовый класс для события
    class CEvent
    {
    public:
        CEvent()
			: m_pSender(0) 
		{
		}

        virtual ~CEvent() {}
        CActor* getSender() {return m_pSender;}

    protected:
        CActor* m_pSender;

    private:
        friend class CActor;
        void setSender(CActor* pSender) {m_pSender = pSender;}
    };

    //вспомогательный класс для простых событий
    template<class DATA>
    class TEvent: public CEvent
    {
    public:
        TEvent(const DATA &rData): m_data(rData) {}
        DATA m_data;
    };

    class CNamedEvent: public CEvent
    {
    public:
        CNamedEvent(const std::string& name, const std::string& param)
			: m_name(name), m_param(param) 
		{
		}

        std::string m_name;
        std::string m_param;
    };

    template<>
    class TEventManager<CNamedEvent>: public IEventManager
    {
    public:
        //кто и с каким именем получает событие
        typedef std::pair<CListener*,std::string> To;

        //кто и от кого получает евент
        //CActor* может быть нулевым
        //тогда To получает сообщение от любого CActor'а
        typedef std::pair<To,CActor*> ToFrom;

        //получатель события
        typedef std::pair<ToFrom, boost::function<void(const std::string&, const std::string&)> > Receiver;

        //список получателей события
        typedef std::list<Receiver> Receivers;

        static TEventManager& Get();

        void subscribe(const std::string&, CListener*, CActor*, boost::function<void(const std::string&, const std::string&)>);

        void unsubscribe(const std::string&, CListener*);

        void unsubscribe(CListener*);

        void sendEvent(CNamedEvent);

        //проверка, что к этому менеджеру подписан данный получатель, но для евентов с другими именами
        bool isOtherNameSubscribe(const std::string&, CListener*);

    private:
        TEventManager();
        ~TEventManager();

        //закрыть
        TEventManager(const TEventManager&);
        TEventManager& operator=(const TEventManager&);

        //нужно для того чтобы все работало
		static TEventManager *ms_pInstance;
		Receivers m_listReceivers;
    };

#include "event.inl"
}