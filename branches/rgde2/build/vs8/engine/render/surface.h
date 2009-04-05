#pragma once

#include <engine/render/resource.h>
#include <string>

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
			base_texture(device& dev, const std::wstring& file_name);

		public:
			static texture_ptr create(device& dev, void* data, size_t size);
			static texture_ptr create(device& dev, const std::wstring& file_name);

			impl_ptr get_pimpl() {return pimpl;}
			const impl_ptr& get_pimpl() const {return pimpl;}
		};
	}
}