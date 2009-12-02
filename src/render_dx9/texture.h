#pragma once

namespace rgde
{
	namespace render
	{
		class texture::texture_impl
		{
		public:
			texture_impl(device& dev, const void* data, size_t size);
			texture_impl(device& dev, core::vfs::istream_ptr file);

			IDirect3DTexture9* get_dx_texture() {return m_dx_texture;}
			const D3DXIMAGE_INFO& get_dx_info() const {return m_info;}

			void init(const void* data, size_t size);

			bool created() {return m_dx_texture ? true : false;}

		private:
			D3DXIMAGE_INFO m_info;
			device& m_device;
			IDirect3DTexture9* m_dx_texture;
		};
	}
}