#include "stdafx.h"

#include <rgde/render/device.h>
#include <rgde/render/resource.h>
#include <rgde/render/buffer.h>

namespace rgde
{
	namespace render
	{

		const vertex_element vertex_element::end_element = 
		{
			0xFF,
			0,
			vertex_element::unused, 
			(vertex_element::decl_method)0,
			(vertex_element::decl_usage)0,
			0
		};

		vertex_declaration_ptr vertex_declaration::create(device& dev, 
			const vertex_element* vertex_elements_array, 
			size_t size)
		{
			return vertex_declaration_ptr(
				new vertex_declaration(dev, vertex_elements_array, size)
				);
		}

		vertex_declaration_ptr vertex_declaration::create(device& dev, 
			const vertex_elements_vector& vertex_elements)
		{
			return vertex_declaration_ptr(
				new vertex_declaration(dev, vertex_elements)
				);
		}

		vertex_declaration::vertex_declaration_impl& vertex_declaration::get_impl()
		{
			return *m_impl;
		}

		const vertex_declaration::vertex_declaration_impl& vertex_declaration::get_impl() const
		{
			return *m_impl;
		}

		buffer::buffer(device& dev, resource::type type, resource::format format, 
			resource::pool pool, uint buffer_usage, bool register_on_lost_reset)
			: resource(type, format, pool), m_buffer_usage(buffer_usage)
		{
		}


		vertex_buffer_ptr vertex_buffer::create(device& dev, 
			size_t size, resource::pool pool, uint flags) {
			return vertex_buffer_ptr(new vertex_buffer(dev, size, pool, flags));
		}


		index_buffer_ptr index_buffer::create(device& dev, size_t size, bool use_32bit, 
			resource::pool pool, uint usage_flags)
		{
			return index_buffer_ptr(new index_buffer(dev, size, use_32bit, pool, usage_flags));
		}
	}
}