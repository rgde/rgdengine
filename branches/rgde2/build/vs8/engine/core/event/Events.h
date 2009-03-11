#pragma once

namespace rgde
{
	namespace core
	{
		namespace events
		{
			namespace details
			{
				class BaseSender;
				class BaseListener;
				class BaseEventsManager;

				// базовый класс менеджера событий
				class BaseEventsManager
				{
				protected:
					BaseEventsManager();
					virtual ~BaseEventsManager();

				public:
					//отписать получателя от менеджера (потомка класса BaseEventsManager)
					virtual void unsubscribe (BaseListener*) = 0;
					virtual void unsubscribe (BaseListener*, BaseSender*) = 0;
				};
			}

			// шаблонный класс менеджера событий
			template <typename Event>
			class manager: public details::BaseEventsManager
			{
				//подписка на получение событий
				struct Subscription
				{
					details::BaseListener                *m_pListener; //кто хочет получать события
					details::BaseSender                  *m_pSender;   //от кого хочет получать события (0 - от всех)
					boost::function<void(Event)>  m_func;      //какой метод вызывать

					bool operator==(const Subscription &s)
					{
						return m_pListener == s.m_pListener;
					}
				};

				 typedef std::list<Subscription> SubscriptionsList;

			public:
				//синглтон
				static manager& get()
				{
					static manager instance;
					return instance;
				}

				//подписать pListener на получение событий в функтор func от отправителя pSender (если равен 0 - то от всех)
				void subscribe (details::BaseListener *pListener, boost::function<void(Event)> func, 
					details::BaseSender *pSender)
				{
					Subscription subs;
					subs.m_pListener = pListener;
					subs.m_func      = func;
					subs.m_pSender   = pSender;
					m_subscriptions.push_back(subs);
				}

				//отписать pListener от получения всех событий типа Event
				void unsubscribe (details::BaseListener *listener)
				{
					SubscriptionsList::iterator i = m_subscriptions.begin();
					while (i != m_subscriptions.end())
					{
						if (i->m_pListener == listener)
							i = m_subscriptions.erase(i);
						else
							++i;
					}
				}

				//отписать pListener от получения всех событий типа Event
				void unsubscribe (details::BaseListener *listener, details::BaseSender *sender)
				{
					SubscriptionsList::iterator i = m_subscriptions.begin();
					while (i != m_subscriptions.end())
					{
						if (i->m_pListener == listener && i->m_pSender == sender)
							i = m_subscriptions.erase(i);
						else
							++i;
					}
				}

				//отправить событие event от отправителя pSender
				void sendEvent (const Event& event, const details::BaseSender *pSender)
				{
					SubscriptionsList tmp(m_subscriptions.begin(), m_subscriptions.end());

					SubscriptionsList::iterator i = tmp.begin();
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
				manager () {}
				~manager() {}

				manager(const manager&);
				manager& operator= (const manager&);

				SubscriptionsList m_subscriptions;
			};

			namespace details
			{
				// базовый класс получателя
				class BaseListener
				{
				protected:
					BaseListener();
					virtual ~BaseListener();

					//подписаться на получение событий
					template <typename Event>
					void subscribe( boost::function<void(Event)> f, BaseSender *sender = 0)
					{
						manager<Event>::get().subscribe(this,f,sender);
					}

					//отписаться от получения событий
					template <typename Event>
					void unsubscribe(BaseSender *sender)
					{
						manager<Event>::get().unsubscribe(this, sender);
					}

					template <typename Event>
					void unsubscribe()
					{
						manager<Event>::get().unsubscribe(this);
					}


				private:
					BaseListener(const BaseListener&);
					BaseListener& operator= (const BaseListener&);
				};


				// базовый класс отправителя
				class BaseSender
				{
				protected:
					BaseSender();
					virtual ~BaseSender();

					template<typename Event>
					void base_send_event(const Event& event)
					{
						manager<Event>::get().sendEvent(event,this);
					}

				private:
					BaseSender(const BaseSender&);
					BaseSender& operator= (const BaseSender&);
				};
			}


			// получатель
			class listener	: private details::BaseListener
			{
			public:
				//подписаться на получение событий
				//  func    - объект с сигнатурой void(Event)
				//  pSender - от кого хотим получать события (0 или ничего, если хотим получать от всех)
				template <typename Event>
				void subscribe( boost::function<void(Event)> func, details::BaseSender *pSender=0 )
				{
					details::BaseListener::subscribe<Event>(func,pSender);
				}

				//подписаться на получение событий
				//  ptr     - указатель на член-функцию с сигнатурой void(Event)
				//  pSender - от кого хотим получать события (0 или ничего, если хотим получать от всех)
				template<typename Event, typename Class, typename EventArg>
				void subscribe (void (Class::*ptr)(EventArg), details::BaseSender *pSender=0)
				{
					details::BaseListener::subscribe<Event>( 
						boost::bind(ptr, static_cast<Class*>(this), _1), pSender );
				}

				//отписаться от получения событий
				template <typename Event>
				void unsubscribe(details::BaseSender* sender)
				{
					details::BaseListener::unsubscribe<Event>(sender);
				}

				template <typename Event>
				void unsubscribe()
				{
					details::BaseListener::unsubscribe<Event>();
				}
			};



			// отправитель
			class sender: public listener, public details::BaseSender
			{
			public:
				template<typename Event>
				void send_event(const Event& event)
				{
					base_send_event(event);
				}
			};
		}
	}
}