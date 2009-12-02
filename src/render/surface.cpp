#include "stdafx.h"

#include <rgde/render/surface.h>

namespace rgde
{
	namespace render
	{
		surface_ptr surface::create_rt(device& dev, size_t width, size_t heigh, 
						resource::format format, multisample_type sm_type, bool locable)
		{
			throw std::exception("Not implemented yet!");
		}

		texture_ptr texture::create(device& dev, size_t width, size_t heigh, size_t num_levels,
						resource::format format, texture_usage usage)
		{
			throw std::exception("Not implemented yet!");
		}

		texture::texture(impl_ptr impl, resource::type type, resource::format format)
			: resource(type, format, resource::default)
			, m_impl(impl)
		{
		}
	}
}