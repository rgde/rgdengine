#include "stdafx.h"

#include "buffer.h"

namespace rgde
{
	namespace render
	{

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
