#pragma once

#include <rgde/render/render_forward.h>
#include <rgde/render/resource.h>
#include <string>

namespace rgde
{
	namespace core
	{
		namespace vfs
		{
			typedef boost::shared_ptr<struct istream> istream_ptr;
			typedef boost::shared_ptr<struct ostream> ostream_ptr;
		}
	}

	namespace render
	{
		class surface : public resource
		{
		protected:
			explicit surface();

		public:
			size_t get_width() const;
			size_t get_height() const;

			struct lock_data 
			{
				/// Pitch of surface, in bytes. 
				int pitch; 
				/// Pointer to the locked bits.
				void *bytes;
			};

			bool lock(lock_data& data) const;
			bool unlock() const;
		};

		class texture : public resource
		{
		public:
			class texture_impl;
			typedef boost::shared_ptr<texture_impl> impl_ptr;

			static texture_ptr create(device& dev, void* data, size_t size);
			static texture_ptr create(device& dev, core::vfs::istream_ptr file);

			impl_ptr		get_impl()		 {return m_impl;}
			const impl_ptr& get_impl() const {return m_impl;}

			/// Returns the number of texture mip levels
			size_t get_level_count() const;

			/// returns empty ptr for not 2d texture
			surface_ptr get_surface(size_t surface_level);
			/// returns empty ptr for not cube texture
			surface_ptr get_surface(cube_face face, size_t surface_level);

			size_t get_width() const;
			size_t get_height() const;
			/// valid only for 3d textures
			size_t get_depth() const;

		protected:			
			texture(impl_ptr impl, resource::type type, resource::format format);

		protected:
			impl_ptr m_impl;
		};
	}
}