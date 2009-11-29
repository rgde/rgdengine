#pragma once

#include "texture.h"

namespace rgde
{
	namespace render
	{
		D3DPOOL convert(resource::pool pool);
		DWORD convert_lock_flags(uint lock_flags);
		DWORD convert_to_fvf(const vertex_elements_vector& elements); // to FVF
		D3DBLEND convert_blend_mode(blend_mode mode);
		DWORD convert_cull_mode(cull_mode mode);

		class device::device_impl
		{
		public:
			device_impl(core::windows::handle hwnd, bool windowed)
				: m_hwnd(hwnd), m_d3d(NULL), m_device(NULL)
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
					D3DCREATE_HARDWARE_VERTEXPROCESSING,
					&d3dpp, &m_device ) ) )
				{
					MessageBox(m_hwnd, L"Can't Create DirectX 3D Device.", L"ERROR",MB_OK|MB_ICONEXCLAMATION);
				}
			}

			~device_impl()
			{
				safe_release(m_device);
				safe_release(m_d3d);
			}

			bool frame_begin()
			{
				return S_OK == m_device->BeginScene();
			}

			bool frame_end()
			{
				return S_OK == m_device->EndScene();
			}

			bool present()
			{
				return S_OK == m_device->Present(NULL, NULL, NULL, NULL);
			}

			void clear(unsigned int color, float depth)
			{
				unsigned int flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL;
				m_device->Clear( 0, NULL, flags ,color, depth, 0 );
			}

			IDirect3DDevice9* get_dx_device() {return m_device;}

			core::windows::handle m_hwnd;
			IDirect3D9* m_d3d;
			IDirect3DDevice9* m_device;
		};
	}
}