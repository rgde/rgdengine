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
			void unsubscribeAll (base_listener *pListener);

			//менеджер добавляет себя в общий список менеджеров
			void addManager     (base_manager *pManager);

			//менеджер удаляет себя из общего списка менеджеров
			void delManager     (base_manager *pManager);

		private:
			std::list<base_manager*> m_managers;
		} list_manager;
	}


	base_manager::base_manager()
	{
		list_manager.addManager(this);
	}

	base_manager::~base_manager()
	{
		list_manager.delManager(this);
	}

	base_listener::base_listener() 
	{
	}

	//отписать получателя от всех менеджеров
	base_listener::~base_listener()
	{		
		list_manager.unsubscribeAll(this);
	}

	base_sender::base_sender() 
	{
	}

	base_sender::~base_sender()
	{
	}

    //отписать данного получателя ото всех менеджеров
    void ListManagers::unsubscribeAll(base_listener *pListener)
    {
        std::list<base_manager*>::iterator i = m_managers.begin();
        while (i != m_managers.end())
        {
            (*i)->unsubscribe(pListener);
            ++i;
        }
    }

    //менеджер добавляет себя в общий список менеджеров
    void ListManagers::addManager (base_manager *pManager)
    {
        m_managers.push_back(pManager);
    }

    //менеджер удаляет себя из общего списка менеджеров
    void ListManagers::delManager (base_manager *pManager)
    {
        std::list<base_manager*>::iterator i = m_managers.begin();
        while (i != m_managers.end())
        {
            if ((*i) == pManager)
                i = m_managers.erase(i);
            else
                ++i;
        }
    }

}