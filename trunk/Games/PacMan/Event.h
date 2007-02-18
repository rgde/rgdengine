///**--------------------------------------------------------------------------------------
//
//									RGDE Engine
//
//----------------------------------------------------------------------------------------
//\author Dub (Originally created)
//
//\author Andrew "Zlobnik" Chouprina, 2005
//mail:		dxprg@mail.ru
//
//Created:	15 june 2005
//---------------------------------------------------------------------------------------*/
//#pragma once
//
//
//
//namespace test_PacMan
//{
//
//	class CListener;
//	class CActor;
//
//	//базовый класс для менеджеров событий
//	class IEventManager
//	{
//	public:
//		virtual ~IEventManager() {}
//		virtual void unsubscribe(CListener*) = 0;
//		static void DeleteAllMangers();
//
//	protected:
//		typedef std::list<IEventManager*> EventManagers;
//		typedef EventManagers::iterator iterator;
//		static EventManagers ms_listInstances;
//	};
//
//	//базовый класс для события
//	class CEvent
//	{
//	public:
//		CEvent(): m_pSender(0) {}
//		virtual ~CEvent() {}
//		CActor* getSender() {return m_pSender;}
//
//	protected:
//		CActor* m_pSender;
//
//	private:
//		friend class CActor;
//		void setSender(CActor* pSender) {m_pSender = pSender;}
//	};
//
//	//вспомогательный класс для простых событий
//	template<class DATA>
//	class TEvent: public CEvent
//	{
//	public:
//		TEvent(const DATA &rData): m_data(rData) {}
//		DATA m_data;
//	};
//
//	//базовый класс для получателей сообщений
//	class CListener
//	{
//	public:
//		virtual ~CListener();
//		template<class RECEIVER, class EVENT>
//			void subscribe(void(RECEIVER::*pHandlerFunc)(EVENT));
//		template<class EVENT>
//			void unsubscribe();
//		void unsubscribe();
//
//	protected:
//		typedef std::list<IEventManager*> EventManagers;
//		EventManagers m_listManagers;
//	};
//
//	//базовый класс для получателя и отправителя сообщений
//	class CActor: public CListener
//	{
//	public:
//		template<class EVENT>
//			void sendEvent(EVENT event);
//		template<class EVENT>
//			void sendEvent(CActor *pReceiver, EVENT event);
//	};
//
//	//шаблонный класс для менеджера событий
//	template<class EVENT>
//	class TEventManager: public IEventManager
//	{
//		//базовый класс получателя сообщений
//		template<typename EVENT>
//		class IReceiver
//		{
//		public:
//			virtual ~IReceiver() {}
//			virtual void operator()(EVENT event) = 0;
//			virtual CListener* getReceiver() = 0;
//		};
//
//		//шаблонный класс получателя сообщений
//		template <typename RECEIVER, typename EVENT>
//		class TReceiver: public IReceiver<EVENT>
//		{
//		public:
//			typedef void (RECEIVER::*HandlerFunc)(EVENT);
//			TReceiver(RECEIVER *pReceiver, HandlerFunc pHandlerFunc):
//			m_pReceiver(pReceiver), m_pHandlerFunc(pHandlerFunc) {}
//			void operator()(EVENT event) {(m_pReceiver->*m_pHandlerFunc)(event);}
//			CListener* getReceiver() {return m_pReceiver;}
//
//		protected:
//			RECEIVER* m_pReceiver;
//			HandlerFunc m_pHandlerFunc;
//		};
//
//		//тайпдефы
//		typedef IReceiver<EVENT> ConcreteReceiver;
//		typedef std::list<ConcreteReceiver*> ReceiversList;
//
//		TEventManager()
//		{
//			assert(!ms_pInstance);
//			ms_pInstance = this;
//			ms_listInstances.push_back(this);
//		}
//
//		~TEventManager()
//		{
//			assert(ms_pInstance);
//			ms_pInstance = 0;
//			ReceiversList::iterator i = m_listReceivers.begin(),
//				end = m_listReceivers.end();
//			while (i != end)
//			{
//				delete (*i);
//				i = m_listReceivers.erase(i);
//			}
//		}
//
//	public:
//		static TEventManager& Get ()
//		{
//			if (0 == ms_pInstance)
//				ms_pInstance = new TEventManager<EVENT>();
//			return *ms_pInstance;
//		}
//
//		template<typename RECEIVER>
//			void subscribe(CListener *pListener, void(RECEIVER::*pHandlerFunc)(EVENT))
//		{
//			//если такой актор уже подписан, то не позволять ему подписываться вновь
//			//(пока не вызван unsubscribe)
//			ReceiversList::iterator i = m_listReceivers.begin(),
//				end = m_listReceivers.end();
//			while (i != end)
//			{
//				if ((*i)->getReceiver() == pListener)
//					return;
//				++i;
//			}
//			//подписать актора на получение событий
//			m_listReceivers.push_back(new TReceiver<RECEIVER,EVENT>((RECEIVER*)pListener,pHandlerFunc));
//		}
//
//		void unsubscribe(CListener *pListener)
//		{
//			ReceiversList::iterator i = m_listReceivers.begin(),
//				end = m_listReceivers.end();
//			while (i != end)
//			{
//				if ((*i)->getReceiver() == pListener)
//				{
//					delete (*i);
//					i = m_listReceivers.erase(i);
//				}
//				else
//					++i;
//			}
//		}
//
//		void sendEvent(EVENT event)
//		{
//			ReceiversList::iterator i = m_listReceivers.begin(),
//				end = m_listReceivers.end();
//			while (i != end)
//			{
//				ConcreteReceiver *pTarget = *i;
//				(*pTarget)(event);
//				++i;
//			}
//		}
//
//		void sendEvent(CListener *pReceiver, EVENT event)
//		{
//			ReceiversList::iterator i = m_listReceivers.begin(),
//				end = m_listReceivers.end();
//			while (i != end)
//			{
//				if ((*i)->getReceiver() == pReceiver)
//				{
//					ConcreteReceiver *pTarget = *i;
//					(*pTarget)(event);
//				}
//				++i;
//			}
//		}
//
//	private:
//		static TEventManager *ms_pInstance;
//		ReceiversList m_listReceivers;
//	};
//
//	template <typename EVENT>
//		TEventManager<EVENT>* TEventManager<EVENT>::ms_pInstance = 0;
//
//	//подписаться на получение события типа EVENT в ф-ю *handler
//	template <typename RECEIVER, typename EVENT>
//		void CListener::subscribe (void(RECEIVER::*pHandlerFunc)(EVENT))
//	{
//		//подписаться
//		TEventManager<EVENT>::Get().subscribe((RECEIVER*)this,pHandlerFunc);
//		//запомнить, что надо отписаться
//		EventManagers::iterator i = std::find
//			(
//			m_listManagers.begin(),
//			m_listManagers.end(),
//			&TEventManager<EVENT>::Get()
//			);
//		if (i == m_listManagers.end())
//			m_listManagers.push_back(&TEventManager<EVENT>::Get());
//	}
//
//	//отписаться от получения событий типа Event
//	template<typename EVENT>
//		void CListener::unsubscribe ()
//	{
//		//отписаться
//		TEventManager<EVENT>::Get().unsubscribe(this);
//		//теперь от этого менеджера отписываться не надо
//		EventManagers::iterator i = std::find
//			(
//			m_listManagers.begin(),
//			m_listManagers.end(),
//			&TEventManager<EVENT>::Get()
//			);
//		if (i != m_listManagers.end())
//			m_listManagers.erase(i);
//	}
//
//	//послать событие всем подписавшимся
//	template<typename EVENT>
//		void CActor::sendEvent (EVENT event)
//	{
//		event.setSender(this);
//		TEventManager<EVENT>::Get().sendEvent(event);
//	}
//
//	//послать сообщение конкретному получателю
//	template <typename EVENT>
//		void CActor::sendEvent (CActor *pReceiver, EVENT event)
//	{
//		event.setSender(this);
//		TEventManager<EVENT>::Get().sendEvent(pReceiver,event);
//	}
//
//}