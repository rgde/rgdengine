#include "stdafx.h"

#include "device.h"
#include "buffer.h"

#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

namespace rgde
{
	namespace render
	{
		//////////////////////////////////////////////////////////////////////////

		D3DPRIMITIVETYPE convert(primitive_type type)
		{
			D3DPRIMITIVETYPE dx_type = static_cast<D3DPRIMITIVETYPE>(type);
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

		resource::pool convert(D3DPOOL pool)
		{
			static resource::pool pool_mapings[D3DPOOL_SYSTEMMEM + 1] =
			{
				resource::default, resource::managed, resource::systemmem
			};

			assert(pool < D3DPOOL_SYSTEMMEM + 1);

			resource::pool out_pool = pool_mapings[pool] ;

			return out_pool;
		}

//////////////////////////////////////////////////////////////////////////

		D3DVIEWPORT9 convert(const view_port& vp)
		{
			D3DVIEWPORT9 dx_vp;

			dx_vp.X = (DWORD)vp.x;
			dx_vp.Y = (DWORD)vp.y;
			dx_vp.Width = (DWORD)vp.width;
			dx_vp.Height = (DWORD)vp.height;
			dx_vp.MinZ = vp.minz;
			dx_vp.MaxZ = vp.maxz;

			return dx_vp;
		}

		view_port convert(const D3DVIEWPORT9& dx_vp)
		{
			view_port vp;
			vp.x		= (size_t)dx_vp.X;
			vp.y		= (size_t)dx_vp.Y;
			vp.width	= (size_t)dx_vp.Width;
			vp.height	= (size_t)dx_vp.Height;
			vp.minz		= dx_vp.MinZ;
			vp.maxz		= dx_vp.MaxZ;
			return vp;
		}

		//////////////////////////////////////////////////////////////////////////


		device::device(core::windows::handle hwnd, core::vfs::system& vfs, bool windowed)
			: m_pimpl(new device_impl(hwnd, windowed, vfs))
		{
		}

		device::~device()
		{
		}

		view_port device::viewport()
		{			
			D3DVIEWPORT9 dx_vp;
			get_impl()->GetViewport(&dx_vp);
			return convert(dx_vp);
		}

		void device::set(const view_port& vp)
		{
			D3DVIEWPORT9 dx_vp = convert(vp);
			get_impl()->SetViewport(&dx_vp);
		}

		bool device::set(size_t rt_index, surface_ptr rt_surface) 
		{
			IDirect3DSurface9* dx_surface = rt_surface->get_impl()->get_dx_surface();
			HRESULT hr = get_impl()->SetRenderTarget((DWORD)rt_index, dx_surface);
			return hr == S_OK;
		}

		//surface_ptr surface::create(impl_ptr impl)
		//{
		//	return surface_ptr
		//		(
		//		new surface
		//			(
		//			impl, 
		//			(resource::format)impl->get_dx_desc().Format, 
		//			convert(impl->get_dx_desc().Pool)
		//			)
		//		);
		//}

		bool device::set_depth(surface_ptr depth_surface) 
		{
			//TODO: check format
			IDirect3DSurface9* dx_surface = depth_surface->get_impl()->get_dx_surface();
			HRESULT hr = get_impl()->SetDepthStencilSurface(dx_surface);
			return hr == S_OK;
		}

		surface_ptr device::render_target(size_t rt_index) 
		{ 
			IDirect3DSurface9* rt = 0;

			HRESULT hr = get_impl()->GetRenderTarget((DWORD)rt_index, &rt);

			if (hr != S_OK)
				return surface_ptr(); 

			surface::impl_ptr impl(new surface::surface_impl(*this, rt));
			return surface::create(impl);			
		}

		surface_ptr device::depth_surface() 
		{ 
			IDirect3DSurface9* rt = 0;

			HRESULT hr = get_impl()->GetDepthStencilSurface(&rt);

			if (hr != S_OK)
				return surface_ptr(); 

			surface::impl_ptr impl(new surface::surface_impl(*this, rt));
			return surface::create(impl);	
		}

		bool device::begin()
		{
			bool res = m_pimpl->frame_begin();

			IDirect3DDevice9* dev = get_impl();

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

		void device::set(size_t stage, texture_addr modeu, texture_addr modev)
		{
			IDirect3DDevice9* dev = get_impl();
			dev->SetSamplerState((DWORD)stage, D3DSAMP_ADDRESSU, (D3DTEXTUREADDRESS)modeu);
			dev->SetSamplerState((DWORD)stage, D3DSAMP_ADDRESSV, (D3DTEXTUREADDRESS)modev);
		}

		bool device::end()
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

		void device::set(index_buffer_ptr ib)
		{
			HRESULT hr = get_impl()->SetIndices(ib->get_impl().get_dx_index_buffer());
			assert(hr == S_OK);
		}

		void device::set(uint stream_number, vertex_buffer_ptr stream_data, uint stride, uint bytes_offset)
		{
			get_impl().get_dx_device()->SetStreamSource(stream_number, stream_data->get_impl().get_dx_vertex_buffer(),bytes_offset, stride);
			DWORD fvf = convert_to_fvf(stream_data->get_declaration()->get_vertex_elemets());
			HRESULT hr = get_impl().get_dx_device()->SetFVF(fvf);
			assert(hr == S_OK);
		}

		void device::set(vertex_declaration_ptr& decl)
		{
			get_impl().get_dx_device()->SetVertexDeclaration(decl->get_impl().get_dx_decl());
		}

		void device::enable_lighting(bool enable)
		{
			get_impl()->SetRenderState(D3DRS_LIGHTING, enable? TRUE : FALSE);
		}

		void device::enable_ztest(bool enable)
		{
			get_impl()->SetRenderState(D3DRS_ZENABLE, enable? TRUE : FALSE);
		}

		void device::set(cull_mode mode)
		{
			get_impl()->SetRenderState(D3DRS_CULLMODE, (D3DCULL)mode );
		}

		void device::enable_alpha_test(bool enable)
		{
			get_impl()->SetRenderState(D3DRS_ALPHATESTENABLE, enable? TRUE : FALSE);
		}

		void device::enable_alpha_blend(bool enable)
		{
			get_impl()->SetRenderState(D3DRS_ALPHABLENDENABLE, enable? TRUE : FALSE);
		}

		void device::set(texture_ptr texture, size_t index)
		{
			IDirect3DTexture9* dx_texture = texture ? texture->get_impl()->get_dx_texture() : 0;
			get_impl()->SetTexture((DWORD)index, dx_texture);
		}

		void device::set(transform_type type, const math::mat44f& m)
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
			HRESULT hr = get_impl()->SetTransform(ttype, d3d_matrix);
			assert(hr == S_OK);
		}

		void device::draw(primitive_type type, uint num_vertices, uint primitive_count, const void* vertex_data, uint vertex_stride,
						const uint16* index_data)
		{			
			D3DPRIMITIVETYPE prim_type = convert(type);
			HRESULT hr = get_impl().get_dx_device()->DrawIndexedPrimitiveUP
				(
				prim_type, 
				0,
				num_vertices,
				primitive_count,
				index_data,
				D3DFMT_INDEX16,
				vertex_data,
				vertex_stride);

			assert(hr == S_OK);
		}


		void device::draw(primitive_type type, uint primitive_count, void* data, uint stride)
		{
			D3DPRIMITIVETYPE prim_type = convert(type);
			HRESULT hr = get_impl().get_dx_device()->DrawPrimitiveUP(prim_type, primitive_count, data, stride);
			assert(hr == S_OK);
		}

		void device::draw(primitive_type type, uint start_vertex, uint primitive_count)
		{
			D3DPRIMITIVETYPE prim_type = convert(type);
			HRESULT hr = get_impl().get_dx_device()->DrawPrimitive(prim_type, start_vertex, primitive_count);
			assert(hr == S_OK);
		}

		void device::draw(primitive_type type, int base_vertex_index, uint min_vertex_index,
			uint num_vertices, uint start_index, uint primitive_count)
		{
			D3DPRIMITIVETYPE prim_type = convert(type);

			IDirect3DDevice9* dev = get_impl().get_dx_device();
			HRESULT hr = dev->DrawIndexedPrimitive(prim_type, base_vertex_index, min_vertex_index,
				num_vertices, start_index, primitive_count);

			assert(hr == S_OK);
		}

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


		//////////////////////////////////////////////////////////////////////////

		device::device_impl::device_impl(core::windows::handle hwnd, bool windowed, core::vfs::system& vfs)
			: m_hwnd(hwnd)
			, m_d3d(NULL)
			, m_device(NULL)
			, m_dx_include(vfs)
		{
			m_d3d = Direct3DCreate9( D3D_SDK_VERSION );

			if ( m_d3d == NULL )
			{
				MessageBox(m_hwnd, L"Can't init DirectX.", L"ERROR", MB_OK|MB_ICONEXCLAMATION);
			}

			RECT rc;
			GetClientRect((HWND)m_hwnd, &rc);

			using rgde::uint;

			uint width = rc.right - rc.left;
			uint height = rc.bottom - rc.top;

			// Tell the window how we want things to be..
			D3DPRESENT_PARAMETERS d3dpp=
			{
				width,			// Back Buffer Width
				height,			// Back Buffer Height
				D3DFMT_X8R8G8B8,		// Back Buffer Format (Color Depth)
				1,				// Back Buffer Count (Double Buffer)
				D3DMULTISAMPLE_NONE,	// No Multi Sample Type
				0,				// No Multi Sample Quality
				D3DSWAPEFFECT_DISCARD,	// Swap Effect (Fast)
				(HWND)m_hwnd,	// The Window Handle
				windowed,		// Windowed or Fullscreen
				TRUE,			// Enable Auto Depth Stencil  
				D3DFMT_D24S8,	// 16Bit Z-Buffer (Depth Buffer)
				0,				// No Flags
				D3DPRESENT_RATE_DEFAULT,   // Default Refresh Rate
				D3DPRESENT_INTERVAL_IMMEDIATE
				//D3DPRESENT_INTERVAL_DEFAULT	// Presentation Interval (vertical sync)
			};

			// Check The Wanted Surface Format
			if ( FAILED( m_d3d->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
				d3dpp.BackBufferFormat, D3DUSAGE_DEPTHSTENCIL,
				D3DRTYPE_SURFACE, d3dpp.AutoDepthStencilFormat ) ) )
			{
				MessageBox(m_hwnd, L"Can't Find Surface Format.", L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			}

			// Create The DirectX 3D Device 
			if(FAILED( m_d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hwnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_device ) ) )
			{
				MessageBox(m_hwnd, L"Can't Create DirectX 3D Device.", L"ERROR",MB_OK|MB_ICONEXCLAMATION);
			}
		}

		device::device_impl::~device_impl()
		{
			safe_release(m_device);
			safe_release(m_d3d);
		}

		bool device::device_impl::frame_begin()
		{
			return S_OK == m_device->BeginScene();
		}

		bool device::device_impl::frame_end()
		{
			return S_OK == m_device->EndScene();
		}

		bool device::device_impl::present()
		{
			return S_OK == m_device->Present(NULL, NULL, NULL, NULL);
		}

		void device::device_impl::clear(unsigned int color, float depth)
		{
			unsigned int flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL;
			m_device->Clear( 0, NULL, flags ,color, depth, 0 );
		}

		//////////////////////////////////////////////////////////////////////////

		dx_include_impl::dx_include_impl(core::vfs::system& vfs)
			: m_vfs(vfs)
		{
		}

		HRESULT dx_include_impl::Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
		{
			core::vfs::istream_ptr in = m_vfs.open_read(pFileName);

			if (!in)
			{
				*pBytes = 0;
				*ppData = 0;

				return S_FALSE;
			}

			uint size = in->get_size();
			byte* data = new byte[size];
			in->read(data, size);

			*pBytes = size;
			*ppData = data;

			return S_OK;
		}

		HRESULT dx_include_impl::Close(LPCVOID data)
		{
			byte* data_bytes = (byte*)data;
			delete []data_bytes;
			return S_OK;
		}
	}
}