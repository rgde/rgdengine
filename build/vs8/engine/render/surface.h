#pragma once

#include <engine/render/resource.h>

namespace rgde
{
	namespace render
	{
		class surface : public resource
		{
		public:
			explicit surface(device& dev);

			virtual size_t get_width() const = 0;
			virtual size_t get_height() const = 0;

			virtual format get_format() const = 0;
		};

		class base_texture : public resource
		{
		public:
			explicit base_texture(device& dev);

			virtual void set_lod(unsigned int new_lod) = 0;
			virtual unsigned int get_lod() = 0;

			virtual unsigned int get_level_count() const = 0;

			//STDMETHOD(SetAutoGenFilterType)(THIS_ D3DTEXTUREFILTERTYPE FilterType) PURE;
			//STDMETHOD_(D3DTEXTUREFILTERTYPE, GetAutoGenFilterType)(THIS) PURE;
			//STDMETHOD_(void, GenerateMipSubLevels)(THIS) PURE;
		};

	}
}