#pragma once

#include "texture.h"

#include <rgde/core/file_system.h>

namespace rgde
{
	namespace render
	{
		D3DPOOL convert(resource::pool pool);
		D3DPRIMITIVETYPE convert(primitive_type type);
		DWORD convert_lock_flags(uint lock_flags);
		DWORD convert_to_fvf(const vertex_elements_vector& elements); // to FVF
		D3DBLEND convert_blend_mode(blend_mode mode);

		D3DVIEWPORT9 convert(const view_port& vp);
		view_port convert(const D3DVIEWPORT9& dx_vp);
		D3DPRIMITIVETYPE convert(primitive_type type);
		resource::pool convert(D3DPOOL pool);
		D3DPOOL convert(resource::pool pool);

		struct dx_include_impl : public ID3DXInclude
		{
			dx_include_impl(core::vfs::system& vfs);

			STDMETHOD(Open)(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)	;
			STDMETHOD(Close)(LPCVOID data);

			core::vfs::system& m_vfs;
		};

		class device::device_impl
		{
		public:
			device_impl(core::windows::handle hwnd, bool windowed, core::vfs::system& vfs);
			~device_impl();

			bool frame_begin();
			bool frame_end();

			bool present();

			void clear(unsigned int color, float depth);	

			operator IDirect3DDevice9* () {return get_dx_device(); }
			IDirect3DDevice9* operator->() {return get_dx_device(); }

			IDirect3DDevice9* get_dx_device() {return m_device;}

			core::windows::handle m_hwnd;
			IDirect3D9* m_d3d;
			IDirect3DDevice9* m_device;

			dx_include_impl m_dx_include;
		};
	}
}