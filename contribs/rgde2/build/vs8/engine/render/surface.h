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
	}
}