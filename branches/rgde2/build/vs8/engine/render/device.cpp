#include "stdafx.h"
//
#include <engine/render/device.h>

namespace rgde
{
	namespace render
	{

		device::objects_iter device::register_obj(device_object* obj)
		{
			m_objects.push_back(obj);
			assert(!m_objects.empty() && "device::register_obj(..) : m_objects can't be empty!");
			return --m_objects.end();
		}

		void device::unregister_obj(objects_iter iter)
		{
			m_objects.erase(iter);
		}
	}
}
