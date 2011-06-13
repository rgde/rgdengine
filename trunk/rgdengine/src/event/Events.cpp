//EventBase.cpp
#include "precompiled.h"

#include "rgde/event/Events.h"

namespace event
{
	namespace
	{
		class ListManagers
		{
		public:
			ListManagers() {}
			~ListManagers() {}

			//отписать данного получателя ото всех менеджеров
			void unsubscribeAll (void *l);

			//менеджер добавляет себя в общий список менеджеров
			void add_manager     (base_manager *manager);

			//менеджер удаляет себя из общего списка менеджеров
			void remove_manager     (base_manager *manager);

		private:
			std::list<base_manager*> m_managers;
		} list_manager;
	}


	base_manager::base_manager()
	{
		list_manager.add_manager(this);
	}

	base_manager::~base_manager()
	{
		list_manager.remove_manager(this);
	}

	listener::listener() 
	{
	}

	listener::~listener()
	{		
		list_manager.unsubscribeAll(this);
	}

	sender::sender() 
	{
	}

	sender::~sender()
	{
	}

    //отписать данного получателя ото всех менеджеров
    void ListManagers::unsubscribeAll(void *l)
    {
        std::list<base_manager*>::iterator i = m_managers.begin();
        while (i != m_managers.end())
        {
            (*i)->unsubscribe(l);
            ++i;
        }
    }

    //менеджер добавляет себя в общий список менеджеров
    void ListManagers::add_manager (base_manager *manager)
    {
        m_managers.push_back(manager);
    }

    //менеджер удаляет себя из общего списка менеджеров
    void ListManagers::remove_manager (base_manager *manager)
    {
        std::list<base_manager*>::iterator i = m_managers.begin();
        while (i != m_managers.end())
        {
            if ((*i) == manager)
                i = m_managers.erase(i);
            else
                ++i;
        }
    }
}