#pragma once

#include <engine/render/resource.h>

namespace rgde
{
	namespace render
	{
		class surface : public resource
		{
		protected:
			explicit surface(device& dev);

		public:
			virtual size_t get_width() const = 0;
			virtual size_t get_height() const = 0;

			virtual format get_format() const = 0;
		};

		class base_texture : public resource
		{
		public:
			class texture_impl;
			typedef boost::shared_ptr<texture_impl> impl_ptr;

		protected:
			impl_ptr pimpl;		
			base_texture(device& dev, void* data, size_t size);

		public:
			static texture_ptr create(device& dev, void* data, size_t size);

			impl_ptr get_pimpl() {return pimpl;}
			const impl_ptr& get_pimpl() const {return pimpl;}

			//virtual size_t get_width() const = 0;
			//virtual size_t get_height() const = 0;

			//virtual format get_format() const = 0;			

			//virtual void set_lod(unsigned int new_lod) = 0;
			//virtual unsigned int get_lod() = 0;

			//virtual unsigned int get_level_count() const = 0;

			//STDMETHOD(SetAutoGenFilterType)(THIS_ D3DTEXTUREFILTERTYPE FilterType) PURE;
			//STDMETHOD_(D3DTEXTUREFILTERTYPE, GetAutoGenFilterType)(THIS) PURE;
			//STDMETHOD_(void, GenerateMipSubLevels)(THIS) PURE;
		};

	}
}