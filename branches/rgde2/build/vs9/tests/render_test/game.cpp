#include "stdafx.h"
#include "game.h"

namespace rgde
{
	namespace game
	{
		system* system::ms_instance = 0;

		system* system::get()
		{
			return ms_instance;
		}

		//////////////////////////////////////////////////////////////////////////
		system::system()
		{
			ms_instance = this;	
		}

		system::~system()
		{
			ms_instance = 0;
		}

		void system::do_update(float dt)
		{
			for(obj_iter it = m_list.begin(); it != m_list.end(); ++it)
			{
				object* obj = *it;
				obj->update(dt);
			}
		}

		handle system::add(object* obj)
		{
			return m_list.insert(m_list.end(), obj);
		}

		void system::remove(handle h)
		{
			m_list.erase(h);
		}

		//////////////////////////////////////////////////////////////////////////

		object::object() 
		{
			m_handle = system::get()->add(this);
		}

		object::~object() 
		{
			system::get()->remove(m_handle);
		}

	}
}