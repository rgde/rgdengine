#include "stdafx.h"

#include "texture.h"
#include "device.h"

namespace rgde
{
	namespace render
	{
		base_texture::texture_impl::texture_impl(device& dev, const void* data, size_t size)
			: m_dx_texture(0), m_device(dev)
		{
			IDirect3DDevice9* dx_dev = m_device.get_impl().get_dx_device();


			HRESULT hr = 
				D3DXGetImageInfoFromFileInMemory(
				(LPCVOID)data,
				(UINT)size,
				&m_info);

			hr =
				D3DXCreateTextureFromFileInMemory(
				dx_dev,
				(LPCVOID)data,
				(UINT)size,
				&m_dx_texture);
		}
	}
}