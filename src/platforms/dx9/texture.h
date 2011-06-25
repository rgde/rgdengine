#pragma once

namespace rgde
{
	namespace render
	{
		class surface::surface_impl
		{
		public:
			surface_impl(device& dev, IDirect3DSurface9* dx_surface)
				: m_device(dev)
				, m_dx_surface(dx_surface)
			{
				m_dx_surface->GetDesc(&m_desc);
			}

			~surface_impl() {m_dx_surface->Release();}

			IDirect3DSurface9* get_dx_surface() {return m_dx_surface;}
			const D3DSURFACE_DESC& get_dx_desc() const {return m_desc;}

			bool created() const {return m_dx_surface ? true : false;}

		private:
			D3DSURFACE_DESC m_desc;
			device& m_device;
			IDirect3DSurface9* m_dx_surface;
		};


		class texture::texture_impl
		{
		public:
			texture_impl(device& dev, const void* data, size_t size);
			texture_impl(device& dev, core::vfs::istream_ptr file);
			texture_impl(device& dev, IDirect3DTexture9* dx_texture);

			~texture_impl() {m_dx_texture->Release();}

			IDirect3DTexture9* get_dx_texture() {return m_dx_texture;}
			const D3DXIMAGE_INFO& get_dx_info() const {return m_info;}

			void init(const void* data, size_t size);

			bool created() {return m_dx_texture ? true : false;}

			device& m_device;

		private:
			D3DXIMAGE_INFO m_info;			
			IDirect3DTexture9* m_dx_texture;
		};
	}
}