//event.cpp
#include "precompiled.h"
#include "event/event.h"

namespace event
{
	IEventManager::EventManagers IEventManager::ms_listInstances;
	TEventManager<CNamedEvent> *TEventManager<CNamedEvent>::ms_pInstance= 0;

	//все! убираю все менеджеры и уезжаю в отпуск!!! (конец работе системы евентов)
	void IEventManager::DeleteAllManagers()
	{
		for (EventManagers::iterator i = ms_listInstances.begin(); i != ms_listInstances.end(); ++i)
		{
			IEventManager *p= *i;
			delete p;
		}
		ms_listInstances.clear();
	}

	void IEventManager::Unsubscribe(CListener *pListeber)
	{
		EventManagers::iterator i	= ms_listInstances.begin(),
								end	= ms_listInstances.end();
		while (i != end)
		{
			(*i)->unsubscribe(pListeber);
			++i;
		}
	}

	//dtor
	CListener::~CListener()
	{
		unsubscribe();
	}

	//подписка для именованных евентов
	void CListener::subscribe(const std::string& name, boost::function<void(const std::string&, const std::string&)> f, CActor *pActor)
	{
		//подпишемся у менеджера
		TEventManager<CNamedEvent>::Get().subscribe(name, this, pActor, f);
	}

	//отписаться от именованного евента
	void CListener::unsubscribe(const std::string& name)
	{
		//отпишемся от менеджера
		TEventManager<CNamedEvent>::Get().unsubscribe(name, this);
	}

	//отписаться от всех
	void CListener::unsubscribe()
	{
		IEventManager::Unsubscribe(this);
	}

	//послать именованное событие
	void CActor::sendEvent(const std::string &name, const std::string &param)
	{
		sendEvent(CNamedEvent(name, param));
	}

	//получить синглтон
	TEventManager<CNamedEvent> & TEventManager<CNamedEvent>::Get()
	{
		if (0 == ms_pInstance)
			ms_pInstance = new TEventManager<CNamedEvent>();
		return *ms_pInstance;
	}

	//подписка получателя
	void TEventManager<CNamedEvent>::subscribe(const std::string& name, CListener *pListener, CActor *pActor, boost::function<void(const std::string&, const std::string&)> f)
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
		To to			= std::make_pair<CListener*, std::string>(pListener, name);
		ToFrom tofrom	= std::make_pair<To, CActor*>(to, pActor);
		Receiver rec	= std::make_pair<ToFrom, boost::function<void(const std::string&, const std::string&)> >(tofrom, f);
		m_listReceivers.push_back(rec);
	}

	//отписка получателя
	void TEventManager<CNamedEvent>::unsubscribe(const std::string& name, CListener *p)
	{
		Receivers::iterator i	= m_listReceivers.begin(),
							end	= m_listReceivers.end();
		while (i != end)
			if (p == i->first.first.first) //жуткое уродство )
			{
				if (name == i->first.first.second)
					i = m_listReceivers.erase(i);
				else
					++i;
			}
			else
				++i;
	}

	//отписка получателя
	void TEventManager<CNamedEvent>::unsubscribe(CListener *p)
	{
		Receivers::iterator i	= m_listReceivers.begin(),
							end	= m_listReceivers.end();
		while (i != end)
			if (p == i->first.first.first) //жуткое уродство )
				i = m_listReceivers.erase(i);
			else
				++i;
	}

	//посылка сообщения
	void TEventManager<CNamedEvent>::sendEvent(CNamedEvent event)
	{
		Receivers::iterator i	= m_listReceivers.begin(),
							end	= m_listReceivers.end();
		while (i != end)
		{
			if (i->first.second == 0 || i->first.second == event.getSender())
				if (i->first.first.second == event.m_name)
					i->second(event.m_name, event.m_param);
			++i;
		}
	}

	//проверка, что к этому менеджеру подписан данный получатель, но для евентов с другими именами
	bool TEventManager<CNamedEvent>::isOtherNameSubscribe(const std::string& name, CListener *pListener)
	{
		Receivers::iterator i	= m_listReceivers.begin(),
							end	= m_listReceivers.end();
		while (i != end)
		{
			if (i->first.first.first == pListener)
				if (i->first.first.second != name)
					return true;
			++i;
		}

		return false;
	}

	//ctor
	TEventManager<CNamedEvent>::TEventManager()
	{
		assert(!ms_pInstance);
		ms_pInstance = this;
		ms_listInstances.push_back(this);
	}

	//dtor
	TEventManager<CNamedEvent>::~TEventManager()
	{
		assert(ms_pInstance);
		ms_pInstance = 0;
		Receivers::iterator i	= m_listReceivers.begin(),
							end	= m_listReceivers.end();
		while (i != end)
			i = m_listReceivers.erase(i);
	}
} //namespace event