#include "stdafx.h"

#include <engine/render/device.h>
#include <engine/render/resource.h>

namespace rgde
{
	namespace render
	{
		resource::resource(device& dev, type res_type, format res_format, 
						  pool res_pool, bool register_on_lost_reset)
			: device_object(dev, register_on_lost_reset),
			m_type(res_type),
			m_pool(res_pool),
			m_format(res_format)
		{

		}

	}
}

