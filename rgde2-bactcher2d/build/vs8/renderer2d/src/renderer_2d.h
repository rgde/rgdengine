#pragma once

#include "primitives.h"

namespace rgde
{
	namespace render
	{
		class renderer_2d
		{
		public:
			typedef std::vector<primitives_2d::line_desc> lines_vector;
			typedef std::vector<primitives_2d::sprite_desc> sprites_vector;
			typedef lines_vector::iterator lines_iter;
			typedef sprites_vector::iterator sprites_iter;

			renderer_2d(device& dev, const uint buff_size = 2097152);
			~renderer_2d();

			void init_primitives_data();

			void add_line(const primitives_2d::line_desc& line);
			void add_line(/* param*/);

			void add_sprite(const primitives_2d::sprite_desc& sprite);
			void add_sprite(const math::vec2f& pos_, const math::vec2f& size_,
						    const math::rect tex_coord_, const float spin_, 
							const math::color& color_=0xffffffff,
						    const texture_ptr texture_=texture_ptr());

			sprites_vector& get_sprites();

			void load_buffers();

			void clear_sprites();
			void clear_lines();
			void clear_all();
		
			void render_all();

		private:
			void prepare_data();
			void draw_batch(primitives_2d::prim_vertex* vert);

			lines_vector m_lines;
			sprites_vector m_sprites;

			device& m_device;

			vertex_declaration_ptr vertex_decl;

			vertex_buffer_ptr m_vb;
			index_buffer_ptr m_ib;

			texture_ptr default_texture;

			// текущий размер данных
			ulong current_data_size;	
			// смещение вершин в буфере
			uint m_buffer_offset;
			// размер буфера
			uint m_buffer_size;
			// размер батча буфера
			uint m_batch_size;
			// флаг апдейта буфера
			bool m_need_update;
		};
	}
}