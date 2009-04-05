#pragma once

#include "device.h"

namespace rgde
{
	namespace render
	{
		class index_buffer::index_buffer_impl
		{
		public:
			index_buffer_impl(device& dev, size_t size, uint usage, bool use_32bit, resource::pool pool);
			~index_buffer_impl();

			void* lock(uint offset_to_lock, uint size_to_lock, ulong flags);
			void unlock();

			IDirect3DIndexBuffer9* get_dx_index_buffer() {return m_dx_index_buffer;}
			const IDirect3DIndexBuffer9* get_dx_index_buffer() const {return m_dx_index_buffer;}

		private:
			device& m_device;
			IDirect3DIndexBuffer9* m_dx_index_buffer;
		};



		class vertex_declaration:: vertex_declaration_impl
		{
		public:
			vertex_declaration_impl(device& dev, const vertex_elements_vector& vertex_elements);
			~vertex_declaration_impl();

		private:
			D3DVERTEXELEMENT9 convert(const vertex_element& element);
			std::vector<D3DVERTEXELEMENT9> convert(const vertex_elements_vector& vertex_elements);

			IDirect3DVertexDeclaration9* get_dx_decl() const {return m_decl;}

		private:
			IDirect3DVertexDeclaration9* m_decl;
		};

		class vertex_buffer::vertex_buffer_impl
		{
		public:
			vertex_buffer_impl(device& dev, size_t size, uint usage, resource::pool pool);
			vertex_buffer_impl(device& dev, vertex_declaration_ptr decl, size_t size, uint usage, resource::pool pool);
			~vertex_buffer_impl();

			void* lock(uint offset_to_lock, uint size_to_lock, ulong flags);
			void unlock();

			IDirect3DVertexBuffer9* get_dx_vertex_buffer() const {return m_dx_vertex_buffer;}			

		private:
			device& m_device;
			IDirect3DVertexBuffer9* m_dx_vertex_buffer;
		};
	}
}