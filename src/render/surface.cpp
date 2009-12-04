#include "stdafx.h"

#include <rgde/render/surface.h>

namespace rgde
{
	namespace render
	{
		surface::surface(impl_ptr impl, resource::format format, resource::pool res_pool)
			: m_impl(impl)
			, resource(resource::surface, format, res_pool)
		{

		}


		texture::texture(impl_ptr impl, resource::type type, resource::format format)
			: resource(type, format, resource::default)
			, m_impl(impl)
		{
		}
	}
}