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
				return resource::a8r8g8b8; 
//				D3DFMT_UNKNOWN              =  0,
//
//					D3DFMT_R8G8B8               = 20,
//					D3DFMT_A8R8G8B8             = 21,
//					D3DFMT_X8R8G8B8             = 22,
//					D3DFMT_R5G6B5               = 23,
//					D3DFMT_X1R5G5B5             = 24,
//					D3DFMT_A1R5G5B5             = 25,
//					D3DFMT_A4R4G4B4             = 26,
//					D3DFMT_R3G3B2               = 27,
//					D3DFMT_A8                   = 28,
//					D3DFMT_A8R3G3B2             = 29,
//					D3DFMT_X4R4G4B4             = 30,
//					D3DFMT_A2B10G10R10          = 31,
//					D3DFMT_A8B8G8R8             = 32,
//					D3DFMT_X8B8G8R8             = 33,
//					D3DFMT_G16R16               = 34,
//					D3DFMT_A2R10G10B10          = 35,
//					D3DFMT_A16B16G16R16         = 36,
//
//					D3DFMT_A8P8                 = 40,
//					D3DFMT_P8                   = 41,
//
//					D3DFMT_L8                   = 50,
//					D3DFMT_A8L8                 = 51,
//					D3DFMT_A4L4                 = 52,
//
//					D3DFMT_V8U8                 = 60,
//					D3DFMT_L6V5U5               = 61,
//					D3DFMT_X8L8V8U8             = 62,
//					D3DFMT_Q8W8V8U8             = 63,
//					D3DFMT_V16U16               = 64,
//					D3DFMT_A2W10V10U10          = 67,
//
//					D3DFMT_UYVY                 = MAKEFOURCC('U', 'Y', 'V', 'Y'),
//					D3DFMT_R8G8_B8G8            = MAKEFOURCC('R', 'G', 'B', 'G'),
//					D3DFMT_YUY2                 = MAKEFOURCC('Y', 'U', 'Y', '2'),
//					D3DFMT_G8R8_G8B8            = MAKEFOURCC('G', 'R', 'G', 'B'),
//					D3DFMT_DXT1                 = MAKEFOURCC('D', 'X', 'T', '1'),
//					D3DFMT_DXT2                 = MAKEFOURCC('D', 'X', 'T', '2'),
//					D3DFMT_DXT3                 = MAKEFOURCC('D', 'X', 'T', '3'),
//					D3DFMT_DXT4                 = MAKEFOURCC('D', 'X', 'T', '4'),
//					D3DFMT_DXT5                 = MAKEFOURCC('D', 'X', 'T', '5'),
//
//					D3DFMT_D16_LOCKABLE         = 70,
//					D3DFMT_D32                  = 71,
//					D3DFMT_D15S1                = 73,
//					D3DFMT_D24S8                = 75,
//					D3DFMT_D24X8                = 77,
//					D3DFMT_D24X4S4              = 79,
//					D3DFMT_D16                  = 80,
//
//					D3DFMT_D32F_LOCKABLE        = 82,
//					D3DFMT_D24FS8               = 83,
//
//					/* D3D9Ex only -- */
//#if !defined(D3D_DISABLE_9EX)
//
//					/* Z-Stencil formats valid for CPU access */
//					D3DFMT_D32_LOCKABLE         = 84,
//					D3DFMT_S8_LOCKABLE          = 85,
//
//#endif // !D3D_DISABLE_9EX
//					/* -- D3D9Ex only */
//
//
//					D3DFMT_L16                  = 81,
//
//					D3DFMT_VERTEXDATA           =100,
//					D3DFMT_INDEX16              =101,
//					D3DFMT_INDEX32              =102,
//
//					D3DFMT_Q16W16V16U16         =110,
//
//					D3DFMT_MULTI2_ARGB8         = MAKEFOURCC('M','E','T','1'),
//
//					// Floating point surface formats
//
//					// s10e5 formats (16-bits per channel)
//					D3DFMT_R16F                 = 111,
//					D3DFMT_G16R16F              = 112,
//					D3DFMT_A16B16G16R16F        = 113,
//
//					// IEEE s23e8 formats (32-bits per channel)
//					D3DFMT_R32F                 = 114,
//					D3DFMT_G32R32F              = 115,
//					D3DFMT_A32B32G32R32F        = 116,
//
//					D3DFMT_CxV8U8               = 117,
//
//					/* D3D9Ex only -- */
//#if !defined(D3D_DISABLE_9EX)
//
//					// Monochrome 1 bit per pixel format
//					D3DFMT_A1                   = 118,
//
//					// 2.8 biased fixed point
//					D3DFMT_A2B10G10R10_XR_BIAS  = 119,
//
//
//					// Binary format indicating that the data has no inherent type
//					D3DFMT_BINARYBUFFER         = 199,
			}

		}
//////////////////////////////////////////////////////////////////////////
		//texture::texture(device& dev, void* data, size_t size)
		//	: resource(dev, resource::texture, resource::x8r8g8b8, resource::default)
		//	, m_impl(new texture_impl(dev, data, size))
		//{
		//	m_impl->get_dx_info().ResourceType
		//	m_type = resource::
		//}

		//texture::texture(device& dev, core::vfs::istream_ptr file)
		//	: resource(dev, resource::texture, resource::x8r8g8b8, resource::default)
		//	, m_impl(new texture_impl(dev, file))
		//{
		//}

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

		 //resource::type type, resource::format format)

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

		texture::texture_impl::texture_impl(device& dev, core::vfs::istream_ptr file)
			: m_dx_texture(0), m_device(dev)
		{
			size_t size = file->get_size();

			boost::scoped_array<byte> data(new byte(size));

			file->read(data.get(), size);

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