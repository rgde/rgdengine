#include "stdafx.h"

#include "buffer.h"

namespace rgde
{
	namespace render
	{
		buffer::buffer(device& dev, resource::type type, resource::format format, 
			resource::pool pool, uint buffer_usage, bool register_on_lost_reset)
			: resource(dev, type, format, pool, register_on_lost_reset), 
			m_buffer_usage(buffer_usage)
		{
		}

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

		vertex_declaration::vertex_declaration(device& dev, const vertex_element* vertex_elements_array, size_t num_elements)
			: m_vertex_elements(num_elements), device_object(dev)
		{
			for (size_t i = 0; i < num_elements; ++i)
			{
				m_vertex_elements[i] = vertex_elements_array[i];
			}

			m_impl.reset(new vertex_declaration_impl(dev, m_vertex_elements));
		}

		vertex_declaration::vertex_declaration(device& dev, const vertex_elements_vector& vertex_elements)
			: m_vertex_elements(vertex_elements),
			m_impl(new vertex_declaration_impl(dev, m_vertex_elements)),
			device_object(dev)
		{
		}

		vertex_buffer_ptr vertex_buffer::create(device& dev, vertex_declaration_ptr decl, 
			size_t size, resource::pool pool, uint usage_flags)
		{
			return vertex_buffer_ptr(
					new vertex_buffer(dev, decl, size, pool, usage_flags)
				);
		}

		vertex_buffer::vertex_buffer(device& dev, vertex_declaration_ptr decl, size_t size, 
									resource::pool pool, uint usage_flags)
			: buffer(dev, resource::vertexbuffer, resource::vertex_data, pool, usage_flags, false),
			m_decl(decl),
			m_pimpl(new vertex_buffer_impl(dev, decl, size, usage_flags, pool))			
		{
		}

		vertex_buffer::~vertex_buffer() 
		{
		}

		void* vertex_buffer::lock(uint offset_to_lock, uint size_to_lock, ulong flags)
		{
			return m_pimpl->lock(offset_to_lock, size_to_lock, flags);
		}

		void vertex_buffer::unlock()
		{
			m_pimpl->unlock();
		}

		vertex_buffer::vertex_buffer_impl& vertex_buffer::get_impl()
		{
			return *m_pimpl;
		}

		const vertex_buffer::vertex_buffer_impl& vertex_buffer::get_impl() const
		{
			return *m_pimpl;
		}

		index_buffer::index_buffer(device& dev, size_t size, bool use_32bit, resource::pool pool, uint usage_flags)
			: buffer(dev, resource::indexbuffer, use_32bit ? resource::index32 : resource::index16, pool, usage_flags, false),
			m_pimpl(new index_buffer_impl(dev, size, usage_flags, use_32bit, pool))	
		{

		}

		index_buffer_ptr index_buffer::create(device& dev, size_t size, bool use_32bit, 
			resource::pool pool, uint usage_flags)
		{
			return index_buffer_ptr(new index_buffer(dev, size, use_32bit, pool, usage_flags));
		}

		index_buffer::~index_buffer()
		{

		}

		void* index_buffer::lock(uint offset_to_lock, uint size_to_lock, ulong flags)
		{
			return m_pimpl->lock(offset_to_lock, size_to_lock, flags);
		}

		void index_buffer::unlock()
		{
			m_pimpl->unlock();
		}

		index_buffer::index_buffer_impl& index_buffer::get_impl()
		{
			return *m_pimpl;
		}

		const index_buffer::index_buffer_impl& index_buffer::get_impl() const
		{
			return *m_pimpl;
		}
	}
}
