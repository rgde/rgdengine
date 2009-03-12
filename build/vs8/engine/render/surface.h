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
		protected:
			explicit base_texture(device& dev);

		public:
			void set(size_t slot);

			//virtual size_t get_width() const = 0;
			//virtual size_t get_height() const = 0;

			//virtual format get_format() const = 0;

			static texture_ptr create(device& dev, void* data, size_t size);

			//virtual void set_lod(unsigned int new_lod) = 0;
			//virtual unsigned int get_lod() = 0;

			//virtual unsigned int get_level_count() const = 0;

			//STDMETHOD(SetAutoGenFilterType)(THIS_ D3DTEXTUREFILTERTYPE FilterType) PURE;
			//STDMETHOD_(D3DTEXTUREFILTERTYPE, GetAutoGenFilterType)(THIS) PURE;
			//STDMETHOD_(void, GenerateMipSubLevels)(THIS) PURE;
		};

	}
}