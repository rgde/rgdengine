#pragma once

namespace event
{
    class sender;
    class base_manager;

    // events manager base class
    class base_manager
    {
	protected:
		base_manager();
		virtual ~base_manager();

    public:
        virtual void unsubscribe (void*, sender* = 0) = 0;
    };

    // events manager
    template <typename Event>
    class manager : public base_manager
    {
		typedef boost::function<void(Event)> event_handler_t;
        //subscription info for receiving events notifications
        struct subscription {
            void            *m_listener;
            sender          *m_sender;   //if NULL - receive events from all sender;
            event_handler_t  m_func;	 //on_recieve callback

            bool operator==(const subscription &s) {
                return m_listener == s.m_listener;
            }
        };

		typedef std::list<subscription> subscriptions_t;

    public:
        static manager& get()
        {
            static manager instance;
            return instance;
        }

        void subscribe (void *listener, event_handler_t func, sender *s = 0)
        {
            subscription subs;
            subs.m_listener = listener;
            subs.m_func      = func;
            subs.m_sender   = s;
            m_subscriptions.push_back(subs);
        }

        //отписать listener от получения всех событий типа Event
        void unsubscribe (void* l, sender *s = 0)
        {
            subscriptions_t::iterator i = m_subscriptions.begin();
            while (i != m_subscriptions.end())
            {
                if (i->m_listener == l && (i->m_sender == s || !s))
                    i = m_subscriptions.erase(i);
                else
                    ++i;
            }
        }

        //отправить событие event от отправителя Sender
        void send (const Event& event, const sender *s = 0)
        {
            subscriptions_t tmp(m_subscriptions.begin(), m_subscriptions.end());

            subscriptions_t::iterator i = tmp.begin();
            while (i != tmp.end())
            {
                if (std::find(m_subscriptions.begin(), m_subscriptions.end(), *i) != m_subscriptions.end())
                {
                    if (i->m_sender == 0 || i->m_sender == s)
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

        subscriptions_t m_subscriptions;
    };

	class sender;

    class listener
    {
    protected:
        listener();
        virtual ~listener();

		//  func    - объект с сигнатурой void(Event)
		//  pSender - от кого хотим получать события (0 или ничего, если хотим получать от всех)
        template <typename Event>
        void subscribe( boost::function<void(Event)> f, sender *s = 0)
        {
            manager<Event>::get().subscribe(this,f,s);
        }

		//  ptr     - указатель на член-функцию с сигнатурой void(Event)
		//  pSender - от кого хотим получать события (0 или ничего, если хотим получать от всех)
		template<typename Event, typename Class, typename EventArg>
		void subscribe (void (Class::*ptr)(EventArg), sender *s=0)
		{
			subscribe<Event>( boost::bind(ptr, static_cast<Class*>(this), _1), s );
		}

        template <typename Event>
        void unsubscribe(sender *s = 0)
        {
            manager<Event>::get().unsubscribe(this, s);
        }

    private:
        listener(const listener&);
        listener& operator= (const listener&);
    };
	

	class sender
	{
	protected:
		sender();
		virtual ~sender();

		template<typename Event>
		void send(const Event& event)
		{
			manager<Event>::get().send(event,this);
		}

	private:
		sender(const sender&);
		sender& operator= (const sender&);
	};
}