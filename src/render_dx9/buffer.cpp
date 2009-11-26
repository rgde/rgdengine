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

//////////////////////////////////////////////////////////////////////////

		index_buffer::index_buffer_impl::index_buffer_impl(device& dev, size_t size, uint usage, bool use_32bit, resource::pool pool)
			: m_device(dev), m_dx_index_buffer(NULL)
		{
			IDirect3DDevice9* dx_dev = m_device.get_impl().get_dx_device();
			D3DFORMAT ib_format = use_32bit ? D3DFMT_INDEX32 : D3DFMT_INDEX16;
			DWORD ib_usage = ((usage & buffer::dynamic) > 0 ? D3DUSAGE_DYNAMIC : 0) |
				((usage & buffer::write_only) > 0 ? D3DUSAGE_WRITEONLY : 0);

			D3DPOOL dx_pool = convert(pool);

			if (S_OK != dx_dev->CreateIndexBuffer((unsigned int)size,ib_usage,ib_format, dx_pool, 
				&m_dx_index_buffer, NULL))
			{
				throw std::bad_exception("index_buffer_impl():Can't create device index buffer object!");
			}
		}

		index_buffer::index_buffer_impl::~index_buffer_impl()
		{
			safe_release(m_dx_index_buffer);
		}

		void* index_buffer::index_buffer_impl::lock(uint offset_to_lock, uint size_to_lock, ulong flags)
		{
			DWORD dx_lock_flags = convert_lock_flags(flags);
			void *data = NULL;

			m_dx_index_buffer->Lock(offset_to_lock, size_to_lock, &data, dx_lock_flags);
			return data;
		}

		void index_buffer::index_buffer_impl::unlock()
		{
			m_dx_index_buffer->Unlock();
		}


		vertex_declaration::vertex_declaration_impl::vertex_declaration_impl(device& dev, const vertex_elements_vector& vertex_elements)
		{
			std::vector<D3DVERTEXELEMENT9> out = convert(vertex_elements);

			IDirect3DDevice9* dx_dev = dev.get_impl().get_dx_device();
			if (S_OK != dx_dev->CreateVertexDeclaration(&out.front(),&m_decl))
			{
				throw std::bad_exception("vertex_declaration_impl():Can't create device vertex declaration object!");
			}
		}

		vertex_declaration::vertex_declaration_impl::~vertex_declaration_impl()
		{
			safe_release(m_decl);
		}

		D3DVERTEXELEMENT9 vertex_declaration::vertex_declaration_impl::convert(const vertex_element& element)
		{
			D3DDECLTYPE type = D3DDECLTYPE_D3DCOLOR;
			switch(element.type)
			{
			case vertex_element::float1:
				type = D3DDECLTYPE_FLOAT1;
				break;
			case vertex_element::float2:
				type = D3DDECLTYPE_FLOAT2;
				break;
			case vertex_element::float3:
				type = D3DDECLTYPE_FLOAT3;
				break;
			case vertex_element::float4:
				type = D3DDECLTYPE_FLOAT4;
				break;

			case vertex_element::color4ub:
				type = D3DDECLTYPE_FLOAT4;
				break;

			case vertex_element::ubyte4:
				type = D3DDECLTYPE_D3DCOLOR;
				break;

			case vertex_element::short2:
				type = D3DDECLTYPE_SHORT2;
				break;

			case vertex_element::short4:
				type = D3DDECLTYPE_SHORT4;
				break;

			case vertex_element::unused:
				type = D3DDECLTYPE_UNUSED;
				break;
			}

			D3DDECLMETHOD method = D3DDECLMETHOD_DEFAULT;

			D3DDECLUSAGE usage = D3DDECLUSAGE_POSITION;

			switch(element.usage)
			{
			case vertex_element::position:
				usage = D3DDECLUSAGE_POSITION;
				break;
			case vertex_element::blend_weight:
				usage = D3DDECLUSAGE_BLENDWEIGHT;
				break;
			case vertex_element::blend_indices:
				usage = D3DDECLUSAGE_BLENDINDICES;
				break;
			case vertex_element::normal:
				usage = D3DDECLUSAGE_NORMAL;
				break;
			case vertex_element::psize:
				usage = D3DDECLUSAGE_PSIZE;
				break;
			case vertex_element::texcoord:
				usage = D3DDECLUSAGE_TEXCOORD;
				break;
			case vertex_element::tangent:
				usage = D3DDECLUSAGE_TANGENT;
				break;
			case vertex_element::binormal:
				usage = D3DDECLUSAGE_BINORMAL;
				break;
			case vertex_element::positiont:
				usage = D3DDECLUSAGE_POSITIONT;
				break;
			case vertex_element::color:
				usage = D3DDECLUSAGE_COLOR;
				break;
			case vertex_element::fog:
				usage = D3DDECLUSAGE_FOG;
				break;
			case vertex_element::depth:
				usage = D3DDECLUSAGE_DEPTH;
				break;
			}

			D3DVERTEXELEMENT9 el = {element.stream,element.offset,type, method,usage,element.usage_index};
			return el;
		}

		std::vector<D3DVERTEXELEMENT9> vertex_declaration::vertex_declaration_impl::convert(const vertex_elements_vector& vertex_elements)
		{
			size_t in_size = vertex_elements.size();
			std::vector<D3DVERTEXELEMENT9> out(in_size);

			//out.reserve(in_size+1);
			//out.resize(in_size);
			for (size_t i = 0; i < in_size; ++i)
			{
				out[i] = convert(vertex_elements[i]);
			}
			return out;
		}


		vertex_buffer::vertex_buffer_impl::vertex_buffer_impl(device& dev, size_t size, uint usage, resource::pool pool)
			: m_device(dev), m_dx_vertex_buffer(NULL)
		{
			IDirect3DDevice9* dx_dev = m_device.get_impl().get_dx_device();

			DWORD vb_usage = ((usage & buffer::dynamic) > 0 ? D3DUSAGE_DYNAMIC : 0) |
				((usage & buffer::write_only) > 0 ? D3DUSAGE_WRITEONLY : 0);

			D3DPOOL dx_pool = convert(pool);

			if (S_OK != dx_dev->CreateVertexBuffer((uint)size,vb_usage, 0, dx_pool, &m_dx_vertex_buffer, NULL))
			{
				throw std::bad_exception("vertex_buffer_impl():Can't create device vertex buffer object!");
			}
		}

		vertex_buffer::vertex_buffer_impl::vertex_buffer_impl(device& dev, vertex_declaration_ptr decl, size_t size, uint usage, resource::pool pool)
			: m_device(dev), m_dx_vertex_buffer(NULL)
		{
			IDirect3DDevice9* dx_dev = m_device.get_impl().get_dx_device();

			//DWORD fvf = convert_to_fvf(const vertex_elements_vector& elements);

			DWORD vb_usage = ((usage & buffer::dynamic) > 0 ? D3DUSAGE_DYNAMIC : 0) |
				((usage & buffer::write_only) > 0 ? D3DUSAGE_WRITEONLY : 0);

			D3DPOOL dx_pool = convert(pool);

			if (S_OK != dx_dev->CreateVertexBuffer((uint)size,vb_usage, 0, dx_pool, &m_dx_vertex_buffer, NULL))
			{
				throw std::bad_exception("vertex_buffer_impl():Can't create device vertex buffer object!");
			}
		}

		vertex_buffer::vertex_buffer_impl::~vertex_buffer_impl()
		{
			safe_release(m_dx_vertex_buffer);
		}

		void* vertex_buffer::vertex_buffer_impl::lock(uint offset_to_lock, uint size_to_lock, ulong flags)
		{
			DWORD dx_lock_flags = convert_lock_flags(flags);
			void *data = NULL;

			m_dx_vertex_buffer->Lock(offset_to_lock, size_to_lock, &data, dx_lock_flags);
			return data;
		}

		void vertex_buffer::vertex_buffer_impl::unlock()
		{
			m_dx_vertex_buffer->Unlock();
		}

	}
}
