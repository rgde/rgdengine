#include "stdafx.h"

#include <rgde/render/device.h>
#include <rgde/render/resource.h>

namespace rgde
{
	namespace render
	{
		resource::resource(type res_type, format res_format, pool res_pool)
			: m_type(res_type)
			, m_pool(res_pool)
			, m_format(res_format)
		{
		}
	}
}

