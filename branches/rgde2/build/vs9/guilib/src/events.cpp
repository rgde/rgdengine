//EventBase.cpp
#include "stdafx.h"
#include "events.h"

namespace gui
{
namespace events
{
	namespace
	{
		class CListManagers
		{
		public:
			CListManagers() {}
			~CListManagers() {}

			//отписать данного получателя ото всех менеджеров
			void unsubscribeAll (BaseListener *pListener);

			//менеджер добавляет себя в общий список менеджеров
			void addManager     (BaseEventsManager *pManager);

			//менеджер удаляет себя из общего списка менеджеров
			void delManager     (BaseEventsManager *pManager);

		private:
			std::list<BaseEventsManager*> m_managers;
		} list_manager;
	}


	BaseEventsManager::BaseEventsManager()
	{
		list_manager.addManager(this);
	}

	BaseEventsManager::~BaseEventsManager()
	{
		list_manager.delManager(this);
	}

	BaseListener::BaseListener() 
	{
	}

	//отписать получателя от всех менеджеров
	BaseListener::~BaseListener()
	{		
		list_manager.unsubscribeAll(this);
	}

	BaseSender::BaseSender() 
	{
	}

	BaseSender::~BaseSender()
	{
	}

    //отписать данного получателя ото всех менеджеров
    void CListManagers::unsubscribeAll(BaseListener *pListener)
    {
        std::list<BaseEventsManager*>::iterator i = m_managers.begin();
        while (i != m_managers.end())
        {
            (*i)->unsubscribe(pListener);
            ++i;
        }
    }

    //менеджер добавляет себя в общий список менеджеров
    void CListManagers::addManager (BaseEventsManager *pManager)
    {
        m_managers.push_back(pManager);
    }

    //менеджер удаляет себя из общего списка менеджеров
    void CListManagers::delManager (BaseEventsManager *pManager)
    {
        std::list<BaseEventsManager*>::iterator i = m_managers.begin();
        while (i != m_managers.end())
        {
            if ((*i) == pManager)
                i = m_managers.erase(i);
            else
                ++i;
        }
    }

}//events
}//gui