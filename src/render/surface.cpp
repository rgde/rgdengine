#include "stdafx.h"

#include <rgde/render/surface.h>

namespace rgde
{
	namespace render
	{
		texture::texture(impl_ptr impl, resource::type type, resource::format format)
			: resource(type, format, resource::default)
			, m_impl(impl)
		{
		}
	}
}