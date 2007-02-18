//получить синглтон
template<typename EVENT>
TEventManager<EVENT>& TEventManager<EVENT>::Get()
{
    if (0 == ms_pInstance)
        ms_pInstance = new TEventManager<EVENT>();
    return *ms_pInstance;
}

//подписка получателя
template<typename EVENT>
void TEventManager<EVENT>::subscribe(CListener* pListener, CActor* pActor, boost::function<void(EVENT)> f)
{
    //проверка на вшивость
    /*
    Receivers::iterator i = m_listReceivers.begin(),
        end = m_listReceivers.end();
    while (i != end)
    {
        if (boost::function_equal(i->second,f))
            return;
        ++i;
    }
    */

    //добавим
    ToFrom tofrom = std::make_pair<CListener*,CActor*>(pListener,pActor);
    Receiver rec = std::make_pair<ToFrom, boost::function<void(EVENT)> >(tofrom,f);
    m_listReceivers.push_back(rec);
}

//отписка получателя
template<typename EVENT>
void TEventManager<EVENT>::unsubscribe(CListener* p)
{
    Receivers::iterator i = m_listReceivers.begin(),
        end = m_listReceivers.end();
    while(i != end)
        if(p == i->first.first)
            i = m_listReceivers.erase(i);
        else
            ++i;
}

//посылка сообщения
template<typename EVENT>
void TEventManager<EVENT>::sendEvent(EVENT event)
{
    Receivers::iterator i = m_listReceivers.begin(),
        end = m_listReceivers.end();
    while(i != end)
    {
        if(i->first.second==0)
            i->second(event);
        else if (i->first.second==event.getSender())
            i->second(event);
        ++i;
    }
}

//ctor
template<typename EVENT>
TEventManager<EVENT>::TEventManager()
{
    assert(!ms_pInstance);
    ms_pInstance = this;
    ms_listInstances.push_back(this);
}

//dtor
template<typename EVENT>
TEventManager<EVENT>::~TEventManager()
{
    assert(ms_pInstance);
    ms_pInstance = 0;

    for(Receivers::iterator it = m_listReceivers.begin();
        it != m_listReceivers.end(); ++it)
	{
		Receiver& r = *it;
		CListener* l = r.first.first;
		l->unsubscribe<EVENT>();
	}

	m_listReceivers.clear();
}

//подписаться
template<typename EVENT>
void CListener::subscribe(boost::function<void(EVENT)> f, CActor *pActor)
{
    //подпишемся у менеджера
    TEventManager<EVENT>::Get().subscribe(this,pActor,f);
}

//подписаться
template<typename EVENT, typename LISTENER>
void CListener::subscribe(LISTENER *pListener, void (LISTENER::*pFunc)(EVENT), CActor *pActor)
{
    boost::function<void(EVENT)> f = boost::bind(pFunc, pListener, _1);

    subscribe(f, pActor);
}

//подписка для именованных евентов
template<typename LISTENER>
void CListener::subscribe(const std::string& name, LISTENER *pListener, void (LISTENER::*pFunc)(const std::string&, const std::string&), CActor *pActor)
{
    boost::function<void(const std::string&, const std::string&)> f = boost::bind(pFunc, pListener, _1, _2);
    subscribe(name, f, pActor);
}

//отписаться
template<typename EVENT>
void CListener::unsubscribe()
{
    //отпишемся от менеджера
    TEventManager<EVENT>::Get().unsubscribe(this); 
}

//послать событие
template<typename EVENT>
void CActor::sendEvent(EVENT event)
{
    event.setSender(this);
    TEventManager<EVENT>::Get().sendEvent(event);
}