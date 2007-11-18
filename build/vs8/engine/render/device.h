#pragma once

#include <engine/core/types.h>
#include <engine/core/windows_forward.h>
#include <engine/render/render_forward.h>

#include <boost/noncopyable.hpp>
#include <list>

namespace rgde
{
	namespace render
	{
		enum cull_mode
		{
			cull_none, 
			cull_ccw,
			cull_cw
		};

		enum transform_type 
		{
			view_transform,
			projection_transform,
			world_transform,
			texture0_transform,
			texture1_transform,
			texture2_transform,
			texture3_transform
		};

		enum primitive_type 
		{
			point_list = 1,
			line_list,
			line_strip,
			triangle_list,
			triangle_strip,
			triangle_fan
		};

		class device : boost::noncopyable
		{
			friend class device_object;
			class device_impl;
			typedef boost::scoped_ptr<device_impl> pimpl;
		public:
			device(core::windows::handle hwnd = NULL, bool windowed = true);
			virtual ~device();

			bool frame_begin();
			bool frame_end();

			void set_lighting(bool enable);
			void set_ztest(bool enable);
			void set_cull_mode(cull_mode mode);
			void set_alpha_test(bool enable);
			void set_alpha_blend(bool enable);
			void set_transform(transform_type type, const math::mat44f& m);

			void draw(primitive_type type, uint start_vertex, uint primitive_count);
			void draw(primitive_type type, int base_vertex_index, uint min_vertex_index,
					  uint num_vertices, uint start_index, uint primitive_count);

			void clear(unsigned int color, float depth = 1.0f);

			bool present();

			void set_stream_source(uint stream_number, vertex_buffer_ptr stream_data, uint stride);
			
			device_impl& get_impl();
			const device_impl& get_impl() const;

		private:
			typedef std::list<device_object*> objects_list;
			typedef objects_list::iterator objects_iter;

			objects_iter register_obj(device_object* obj);
			void unregister_obj(objects_iter iter);

		private:
			objects_list m_objects;

			core::windows::handle m_hwnd;
			pimpl m_pimpl;
		};
	}
}