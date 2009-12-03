#include "stdafx.h"

#include <rgde/core/event/events.h>

namespace rgde
{
	namespace core
	{
		namespace events
		{
			namespace
			{
				class list_managers
				{
				public:
					list_managers() {}
					~list_managers() {}

					//отписать данного получателя ото всех менеджеров
					void unsubscribe_all (details::base_listener *listener);

					//менеджер добавляет себя в общий список менеджеров
					void add_manager     (details::base_events_manager *manager);

					//менеджер удаляет себя из общего списка менеджеров
					void remove_manager     (details::base_events_manager *manager);

				private:
					std::list<details::base_events_manager*> m_managers;
				} list_manager;
			}

			namespace details
			{

				base_events_manager::base_events_manager()
				{
					list_manager.add_manager(this);
				}

				base_events_manager::~base_events_manager()
				{
					list_manager.remove_manager(this);
				}

				base_listener::base_listener() 
				{
				}

				//отписать получателя от всех менеджеров
				base_listener::~base_listener()
				{		
					list_manager.unsubscribe_all(this);
				}

				base_sender::base_sender() 
				{
				}

				base_sender::~base_sender()
				{
				}

			}

			//отписать данного получателя ото всех менеджеров
			void list_managers::unsubscribe_all(details::base_listener *listener)
			{
				std::list<details::base_events_manager*>::iterator i = m_managers.begin();
				while (i != m_managers.end())
				{
					(*i)->unsubscribe(listener);
					++i;
				}
			}

			//менеджер добавляет себя в общий список менеджеров
			void list_managers::add_manager (details::base_events_manager *manager)
			{
				m_managers.push_back(manager);
			}

			//менеджер удаляет себя из общего списка менеджеров
			void list_managers::remove_manager (details::base_events_manager *manager)
			{
				std::list<details::base_events_manager*>::iterator i = m_managers.begin();
				while (i != m_managers.end())
				{
					if ((*i) == manager)
						i = m_managers.erase(i);
					else
						++i;
				}
			}

		}
	}
}