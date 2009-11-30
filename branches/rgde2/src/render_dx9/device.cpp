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
			bool res = m_pimpl->frame_begin();

			IDirect3DDevice9* dev = get_impl().get_dx_device();

			// Set default states

			// Set the base texture operation and args.
			dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			dev->SetTextureStageState(0,D3DTSS_COLORARG2, D3DTA_DIFFUSE );

			dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			dev->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

			for (int i = 0; i < 8; ++i)
			{
				dev->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				dev->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
				dev->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

				dev->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
				dev->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
			}

			return res;
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

		void device::set_cull_mode(cull_mode mode)
		{
			get_impl().get_dx_device()->SetRenderState(D3DRS_CULLMODE, (D3DCULL)mode );
		}

		void device::set_alpha_test(bool enable)
		{
			get_impl().get_dx_device()->SetRenderState(D3DRS_ALPHATESTENABLE, enable? TRUE : FALSE);
		}

		void device::set_alpha_blend(bool enable)
		{
			get_impl().get_dx_device()->SetRenderState(D3DRS_ALPHABLENDENABLE, enable? TRUE : FALSE);
		}

		void device::set_texture(texture_ptr texture, size_t index)
		{
			IDirect3DTexture9* dx_texture = texture->get_impl()->get_dx_texture();
			get_impl().get_dx_device()->SetTexture(index, dx_texture);
		}

		void device::set_transform(transform_type type, const math::mat44f& m)
		{
			static D3DTRANSFORMSTATETYPE trasform_maping[texture3_transform + 1] =  
			{
				D3DTS_VIEW,
				D3DTS_PROJECTION,
				D3DTS_WORLD,
				D3DTS_TEXTURE0,
				D3DTS_TEXTURE1,
				D3DTS_TEXTURE2,
				D3DTS_TEXTURE3,
			};

			assert(type < texture3_transform + 1 && "transform_type out of range!");

			D3DTRANSFORMSTATETYPE ttype = trasform_maping[type];

			const D3DMATRIX* d3d_matrix = (const D3DMATRIX*)(float*)&m;

			get_impl().get_dx_device()->SetTransform(ttype, d3d_matrix);
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

		D3DPRIMITIVETYPE convert(primitive_type type)
		{
			D3DPRIMITIVETYPE dx_type = (D3DPRIMITIVETYPE)type;
			return dx_type;
		}

		D3DPOOL convert(resource::pool pool)
		{
			static D3DPOOL pool_mapings[resource::systemmem + 1] =
			{
				D3DPOOL_DEFAULT, D3DPOOL_MANAGED, D3DPOOL_SYSTEMMEM
			};

			assert(pool < resource::systemmem + 1);

			D3DPOOL dx_pool = pool_mapings[pool] ;

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
		inline DWORD GET_D3DFVF_XYZB(uint index)		
		{
			assert(index > 0 && index < 6);
			return (0x006 + (index-1)*2)&0x00F;
		}

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

		D3DBLEND convert_blend_mode(blend_mode mode)
		{
			static D3DBLEND blend_mapings[blend_invdest_color + 1] =
			{
				D3DBLEND_ONE, D3DBLEND_ZERO, D3DBLEND_SRCCOLOR, D3DBLEND_INVSRCCOLOR,
				D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA, D3DBLEND_DESTALPHA, 
				D3DBLEND_INVDESTALPHA, D3DBLEND_DESTCOLOR, D3DBLEND_INVDESTCOLOR
			};

			D3DBLEND dx_blend_mode = blend_mapings[mode];

			return dx_blend_mode;
		}

	}
}