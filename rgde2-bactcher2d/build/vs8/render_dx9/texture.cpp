#include "stdafx.h"

#include "texture.h"
#include "device.h"

#include <fstream>

namespace rgde
{
	namespace render
	{
		base_texture::base_texture(device& dev, void* data, size_t size)
			: resource(dev, resource::texture, resource::x8r8g8b8, resource::default)
			, pimpl(new texture_impl(dev, data, size))
		{
		}

		base_texture::base_texture(device& dev, const std::wstring& file_name)
			: resource(dev, resource::texture, resource::x8r8g8b8, resource::default)
			, pimpl(new texture_impl(dev, file_name))
		{
		}

		texture_ptr base_texture::create(device& dev, void* data, size_t size)
		{
			return texture_ptr(new base_texture(dev,data, size));
		}

		texture_ptr base_texture::create(device& dev, const std::wstring& file_name)
		{
			return texture_ptr(new base_texture(dev, file_name));
		}

		base_texture::texture_impl::texture_impl(device& dev, const void* data, size_t size)
			: m_dx_texture(0), m_device(dev)
		{
			IDirect3DDevice9* dx_dev = m_device.get_impl().get_dx_device();

			HRESULT hr = D3DXGetImageInfoFromFileInMemory(
				(LPCVOID)data,
				(UINT)size,
				&m_info);

			hr = D3DXCreateTextureFromFileInMemory(
				dx_dev,
				(LPCVOID)data,
				(UINT)size,
				&m_dx_texture);
		}

		base_texture::texture_impl::texture_impl(device& dev, const std::wstring& file_name)
			: m_dx_texture(0), m_device(dev)
		{
			IDirect3DDevice9* dx_dev = m_device.get_impl().get_dx_device();

			HRESULT hr = D3DXGetImageInfoFromFileW(file_name.c_str(), &m_info);

			hr = D3DXCreateTextureFromFileW(dx_dev, file_name.c_str(), &m_dx_texture);
		}
	}
}