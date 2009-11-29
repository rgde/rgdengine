#include "stdafx.h"

#include "device.h"
#include "buffer.h"

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

namespace rgde
{
	namespace render
	{
		device::device(core::windows::handle hwnd, bool windowed)
			: m_pimpl(new device_impl(hwnd, windowed))
		{
		}

		device::~device()
		{
		}

		bool device::frame_begin()
		{
			return m_pimpl->frame_begin();
		}

		bool device::frame_end()
		{
			return m_pimpl->frame_end();
		}

		bool device::present()
		{
			return m_pimpl->present();
		}

		void device::clear(unsigned int color, float depth)
		{
			m_pimpl->clear(color, depth);
		}

		device::device_impl& device::get_impl()
		{
			return *m_pimpl;
		}

		const device::device_impl& device::get_impl() const
		{
			return *m_pimpl;
		}

		void device::set_index_buffer(index_buffer_ptr ib)
		{
			get_impl().get_dx_device()->SetIndices(ib->get_impl().get_dx_index_buffer());
		}

		void device::set_stream_source(uint stream_number, vertex_buffer_ptr stream_data, uint stride)
		{
			get_impl().get_dx_device()->SetStreamSource(stream_number, stream_data->get_impl().get_dx_vertex_buffer(),0, stride);
			DWORD fvf = convert_to_fvf(stream_data->get_declaration()->get_vertex_elemets());
			HRESULT hr = get_impl().get_dx_device()->SetFVF(fvf);
		}

		void device::set_lighting(bool enable)
		{
			get_impl().get_dx_device()->SetRenderState(D3DRS_LIGHTING, enable? TRUE : FALSE);
		}

		void device::set_ztest(bool enable)
		{
			get_impl().get_dx_device()->SetRenderState(D3DRS_ZENABLE, enable? TRUE : FALSE);
		}

		DWORD convert_cull_mode(cull_mode mode)
		{
			DWORD dx_cull_mode;

			switch(mode)
			{
			case cull_none:
				dx_cull_mode = D3DCULL_NONE;
				break;
			case cull_ccw:
				dx_cull_mode = D3DCULL_CCW;
				break;
			case cull_cw:
				dx_cull_mode = D3DCULL_CW;
				break;
			}

			return dx_cull_mode;
		}

		void device::set_cull_mode(cull_mode mode)
		{
			DWORD cull_mode = convert_cull_mode(mode);
			get_impl().get_dx_device()->SetRenderState(D3DRS_CULLMODE, cull_mode);
		}

		void device::set_alpha_test(bool enable)
		{
			get_impl().get_dx_device()->SetRenderState(D3DRS_ALPHATESTENABLE, enable? TRUE : FALSE);
		}

		void device::set_alpha_blend(bool enable)
		{
			get_impl().get_dx_device()->SetRenderState(D3DRS_ALPHABLENDENABLE, enable? TRUE : FALSE);
		}

		void device::set_transform(transform_type type, const math::mat44f& m)
		{
			D3DTRANSFORMSTATETYPE ttype = D3DTS_VIEW;

			switch(type)
			{
			case projection_transform:
				ttype = D3DTS_PROJECTION;
				break;
			case view_transform:
				ttype = D3DTS_VIEW;
				break;
			case world_transform:
				ttype = D3DTS_WORLD;
				break;
			case texture0_transform:
				ttype = D3DTS_TEXTURE0;
				break;
			case texture1_transform:
				ttype = D3DTS_TEXTURE1;
				break;
			case texture2_transform:
				ttype = D3DTS_TEXTURE2;
				break;
			case texture3_transform:
				ttype = D3DTS_TEXTURE3;
				break;
			}

			const D3DMATRIX* d3d_matrix = (const D3DMATRIX*)(float*)&m;

			get_impl().get_dx_device()->SetTransform(ttype, d3d_matrix);
		}

		void device::set_texture(texture_ptr texture, size_t index)
		{
			IDirect3DTexture9* dx_texture = texture->get_impl()->get_dx_texture();
			get_impl().get_dx_device()->SetTexture((DWORD)index, dx_texture);
		}

		DWORD convert_blend_mode(blend_mode mode)
		{
			DWORD dx_blend_mode;

			switch(mode)
			{
			case blend_one:
				dx_blend_mode = D3DBLEND_ZERO;
				break;
			case blend_zero:
				dx_blend_mode = D3DBLEND_ONE;
				break;
			case blend_srccolor:
				dx_blend_mode = D3DBLEND_SRCCOLOR;
				break;
			case blend_invsrccolor:
				dx_blend_mode = D3DBLEND_INVSRCCOLOR;
				break;
			case blend_srcalpha:
				dx_blend_mode = D3DBLEND_SRCALPHA;
				break;
			case blend_invsrcalpha:
				dx_blend_mode = D3DBLEND_INVSRCALPHA;
				break;
			case blend_destalpha:
				dx_blend_mode = D3DBLEND_DESTALPHA;
				break;
			case blend_invdestalpha:
				dx_blend_mode = D3DBLEND_INVDESTALPHA;
				break;
			case blend_destcolor:
				dx_blend_mode = D3DBLEND_DESTCOLOR;
				break;
			case blend_invdestcolor:
				dx_blend_mode = D3DBLEND_INVDESTCOLOR;
				break;
			case blend_srcalphasat:
				dx_blend_mode = D3DBLEND_SRCALPHASAT;
				break;
			}

			return dx_blend_mode;
		}

		void device::set_blend_mode(blend_mode src, blend_mode dest)
		{
			DWORD src_mode = convert_blend_mode(src);
			DWORD dest_mode = convert_blend_mode(dest);

			HRESULT hr = get_impl().get_dx_device()->SetRenderState(D3DRS_SRCBLEND, src_mode);
			HRESULT hr2 = get_impl().get_dx_device()->SetRenderState(D3DRS_DESTBLEND, dest_mode);
		}

		D3DPRIMITIVETYPE convert(primitive_type type)
		{
			D3DPRIMITIVETYPE dx_type = D3DPT_LINELIST;

			switch(type)
			{
			case line_list:
				dx_type = D3DPT_LINELIST;
				break;
			case line_strip:
				dx_type = D3DPT_LINESTRIP;
				break;
			case point_list:
				dx_type = D3DPT_POINTLIST;
				break;
			case triangle_list:
				dx_type = D3DPT_TRIANGLELIST;
				break;
			case triangle_strip:
				dx_type = D3DPT_TRIANGLESTRIP;
				break;
			case triangle_fan:
				dx_type = D3DPT_TRIANGLEFAN;
				break;
			}

			return dx_type;
		}

		void device::draw(primitive_type type, uint start_vertex, uint primitive_count)
		{
			D3DPRIMITIVETYPE prim_type = convert(type);
			HRESULT hr = get_impl().get_dx_device()->DrawPrimitive(prim_type, start_vertex, primitive_count);
			__asm nop;
		}

		void device::draw(primitive_type type, int base_vertex_index, uint min_vertex_index,
			uint num_vertices, uint start_index, uint primitive_count)
		{
			D3DPRIMITIVETYPE prim_type = convert(type);

			IDirect3DDevice9* dev = get_impl().get_dx_device();
			dev->DrawIndexedPrimitive(prim_type, base_vertex_index, min_vertex_index,
				num_vertices, start_index, primitive_count);
		}

		//////////////////////////////////////////////////////////////////////////
		D3DPOOL convert(resource::pool pool)
		{
			D3DPOOL dx_pool;
			switch(pool)
			{
			case resource::default:
				dx_pool = D3DPOOL_DEFAULT;
				break;
			case resource::systemmem:
				dx_pool = D3DPOOL_SYSTEMMEM;
				break;
			case resource::managed:
				dx_pool = D3DPOOL_MANAGED;
				break;
			}
			return dx_pool;
		}

		//#define D3DFVF_XYZB1            0x006
		//#define D3DFVF_XYZB2            0x008
		//#define D3DFVF_XYZB3            0x00a
		//#define D3DFVF_XYZB4            0x00c
		//#define D3DFVF_XYZB5            0x00e

		//===========================================================================================
		//Data type:		|	Usage:					|	Usage index:  | FVF:					|
		//-------------------------------------------------------------------------------------------
		//D3DDECLTYPE_FLOAT3	D3DDECLUSAGE_POSITION		0				D3DFVF_XYZ				|*
		//D3DDECLTYPE_FLOAT4	D3DDECLUSAGE_POSITIONT		0				D3DFVF_XYZRHW			|*
		//D3DDECLTYPE_FLOATn	D3DDECLUSAGE_BLENDWEIGHT	0				D3DFVF_XYZBn			|
		//D3DDECLTYPE_UBYTE4	D3DDECLUSAGE_BLENDINDICES	0				D3DFVF_XYZB (nWeights+1)|
		//D3DDECLTYPE_FLOAT3	D3DDECLUSAGE_NORMAL			0				D3DFVF_NORMAL			|*
		//D3DDECLTYPE_D3DCOLOR	D3DDECLUSAGE_COLOR			0				D3DFVF_DIFFUSE			|*
		//D3DDECLTYPE_D3DCOLOR	D3DDECLUSAGE_COLOR			1				D3DFVF_SPECULAR			|*
		//D3DDECLTYPE_FLOATm	D3DDECLUSAGE_TEXCOORD		n				D3DFVF_TEXCOORDSIZEm(n) |*
		//===========================================================================================

		//D3DFVF_TEXCOORDSIZE3(CoordIndex)

		//#define D3DFVF_XYZB1            0x006
		//#define D3DFVF_XYZB2            0x008
		//#define D3DFVF_XYZB3            0x00a
		//#define D3DFVF_XYZB4            0x00c
		//#define D3DFVF_XYZB5            0x00e
		inline DWORD GET_D3DFVF_XYZB(uint index) {assert(index > 0 && index < 6); return (0x006 + (index-1)*2)&0x00F;}

		DWORD convert_to_fvf(const vertex_elements_vector& elements)
		{
			typedef vertex_elements_vector::const_iterator iter;
			iter begin = elements.begin();
			iter end = elements.end();

			DWORD fvf = 0;
			byte nWeights = 0;

			for (iter it = begin; it != end; ++it)
			{
				const vertex_element& el = *it;
				if (el.usage_index == 0 && el.type == vertex_element::float3 && el.usage == vertex_element::normal)
					fvf |= D3DFVF_NORMAL;
				else if (el.usage_index == 0 && el.type == vertex_element::float3 && el.usage == vertex_element::position)
					fvf |= D3DFVF_XYZ;
				else if (el.usage_index == 0 && el.type == vertex_element::float4 && el.usage == vertex_element::positiont)
					fvf |= D3DFVF_XYZRHW;
				else if (el.usage_index == 0 && el.type == vertex_element::color4ub && el.usage == vertex_element::color)
					fvf |= D3DFVF_DIFFUSE;
				else if (el.usage_index == 1 && el.type == vertex_element::color4ub && el.usage == vertex_element::color)
					fvf |= D3DFVF_SPECULAR;
				else if (el.usage == vertex_element::texcoord)
				{
					fvf |= (D3DFVF_TEX1 * (el.usage_index + 1) );

					switch(el.type)
					{
					case vertex_element::float1:
						fvf |= D3DFVF_TEXCOORDSIZE1(el.usage_index);
						break;
					case vertex_element::float2:
						fvf |= D3DFVF_TEXCOORDSIZE2(el.usage_index);
						break;
					case vertex_element::float3:
						fvf |= D3DFVF_TEXCOORDSIZE3(el.usage_index);
						break;
					case vertex_element::float4:
						fvf |= D3DFVF_TEXCOORDSIZE4(el.usage_index);
						break;
					}
				}
				else if (el.usage == vertex_element::blend_indices)
				{

				}
				else if (el.usage == vertex_element::blend_weight)
				{
					switch(el.type)
					{
					case vertex_element::float1:
						fvf |= D3DFVF_XYZB1; nWeights = 1;
						break;
					case vertex_element::float2:
						fvf |= D3DFVF_XYZB2; nWeights = 2;
						break;
					case vertex_element::float3:
						fvf |= D3DFVF_XYZB3; nWeights = 3;
						break;
					case vertex_element::float4:
						fvf |= D3DFVF_XYZB4; nWeights = 4;
						break;
					}
				}
			}

			if (nWeights > 0)
				fvf |= GET_D3DFVF_XYZB(nWeights+1);

			return fvf;
		}

		DWORD convert_lock_flags(uint lock_flags)
		{
			DWORD flags = 0;
			flags |= (lock_flags & buffer::discard) > 0 ? D3DLOCK_DISCARD : 0;
			flags |= (lock_flags & buffer::nooverwrite) > 0 ? D3DLOCK_NOOVERWRITE : 0;
			flags |= (lock_flags & buffer::donotwait) > 0 ? D3DLOCK_DONOTWAIT : 0;
			flags |= (lock_flags & buffer::nosyslock) > 0 ? D3DLOCK_NOSYSLOCK : 0;
			flags |= (lock_flags & buffer::no_dirty_update) > 0 ? D3DLOCK_NO_DIRTY_UPDATE : 0;
			return flags;
		}

	}
}