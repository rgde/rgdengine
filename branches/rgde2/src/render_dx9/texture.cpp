#include "stdafx.h"

#include "texture.h"
#include "device.h"

#include <rgde/core/file_system.h>
#include <boost/scoped_array.hpp>

namespace rgde
{
	namespace render
	{
		namespace 
		{
			resource::type convert(D3DRESOURCETYPE dx_type)
			{
				return (resource::type)dx_type;
			}

			resource::format convert(D3DFORMAT dx_format)
			{
				switch(dx_format)
				{
					case D3DFMT_R8G8B8:
						return resource::r8g8b8;
					case D3DFMT_A8R8G8B8:
						return resource::a8r8g8b8;
					case D3DFMT_X8R8G8B8:
						return resource::a8r8g8b8;
					case D3DFMT_A8B8G8R8:
						return resource::a8b8g8r8;
					case D3DFMT_DXT1:
						return resource::dxt1;
					case D3DFMT_DXT2:
						return resource::dxt2;
					case D3DFMT_DXT3:
						return resource::dxt3;
					case D3DFMT_DXT4:
						return resource::dxt4;
					case D3DFMT_DXT5:
						return resource::dxt5;
					case D3DFMT_D32:
						return resource::d32;
					case D3DFMT_D24S8:
						return resource::d24s8;
				};
				return resource::unknown; 
			}
		}
//////////////////////////////////////////////////////////////////////////
		texture_ptr texture::create(device& dev, void* data, size_t size)
		{
			texture::impl_ptr impl(new texture_impl(dev, data, size));

			if (impl && impl->created())
			{
				return texture_ptr(
					new texture(
					impl,
					convert(impl->get_dx_info().ResourceType), 
					convert(impl->get_dx_info().Format))
					);
			}

			return texture_ptr();
		}

		texture_ptr texture::create(device& dev, core::vfs::istream_ptr file)
		{
			texture::impl_ptr impl(new texture_impl(dev, file));

			if (impl && impl->created())
			{
				return texture_ptr(
					new texture(
					impl,
					convert(impl->get_dx_info().ResourceType), 
					convert(impl->get_dx_info().Format))
					);
			}

			return texture_ptr();
		}

		size_t texture::get_level_count() const
		{
			return (size_t)m_impl->get_dx_info().MipLevels;
		}

		surface_ptr texture::get_surface(size_t surface_level)
		{
			return surface_ptr();
		}

		surface_ptr texture::get_surface(cube_face face, size_t surface_level)
		{
			return surface_ptr();
		}

		size_t texture::get_width() const
		{
			return (size_t)m_impl->get_dx_info().Width;
		}

		size_t texture::get_height() const
		{
			return (size_t)m_impl->get_dx_info().Height;
		}

		size_t texture::get_depth() const
		{
			return (size_t)m_impl->get_dx_info().Depth;
		}
//////////////////////////////////////////////////////////////////////////
		texture::texture_impl::texture_impl(device& dev, const void* data, size_t size)
			: m_dx_texture(0), m_device(dev)
		{
			init(data, size);
		}

		texture::texture_impl::texture_impl(device& dev, IDirect3DTexture9* dx_texture)
			: m_device(dev), m_dx_texture(dx_texture)
		{
			D3DSURFACE_DESC desc;
			dx_texture->GetLevelDesc(0, &desc);

			m_info.MipLevels = m_dx_texture->GetLevelCount();
			m_info.Width = desc.Width;
			m_info.Height = desc.Height;
			m_info.ImageFileFormat = D3DXIFF_FORCE_DWORD;
			m_info.Depth = 0;
			m_info.Format = desc.Format;
			m_info.ResourceType = D3DRTYPE_TEXTURE;
		}

		texture::texture_impl::texture_impl(device& dev, core::vfs::istream_ptr file)
			: m_dx_texture(0), m_device(dev)
		{
			assert(file->is_valid());

			size_t size = file->get_size();

			boost::scoped_array<byte> data(new byte[size]);

			file->read(data.get(), (unsigned int)size);

			init(data.get(), size);
		}

		void texture::texture_impl::init(const void* data, size_t size)
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