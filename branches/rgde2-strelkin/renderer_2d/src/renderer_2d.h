#pragma once

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

			renderer_2d();
			~renderer_2d();

			void init_device();
			void init_primitives_data();

			void add_line(const primitives_2d::line_desc& line);
			void add_line(/* param*/);


			void add_sprite(const primitives_2d::sprite_desc& sprite);
			void add_sprite(const math::vec2f& pos_, const math::vec2f& size_,
						    const math::rect tex_coord_, const float spin_, 
						    const texture_ptr texture_=default_texture);

			void load_buffers();

			void clear_sprites();
			void clear_lines();

			void render_all();
		private:
			lines_vector m_lines;
			sprites_vector m_sprites;

			vertex_declaration_ptr vertex_decl;

			vertex_buffer_ptr m_vb;
			index_buffer_ptr m_ib;

			base_texture_ptr default_texture;

			device& m_device;
		};
	}
}