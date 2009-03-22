#pragma once

namespace rgde
{
	namespace render
	{
		class base_texture::texture_impl
		{
		public:
			texture_impl(device& dev, const void* data, size_t size);

		private:
			D3DXIMAGE_INFO m_info;
			device& m_device;
			IDirect3DTexture9* m_dx_texture;
		};
	}
}