#pragma once

namespace rgde
{
	namespace render
	{
		class renderer_2d
		{
		public:
			typedef std::vector<primitive::line_desc> lines_vector;
			typedef std::vector<primitive::sprite_desc> sprites_vector;
			typedef lines_vector::iterator line_iter;
			typedef sprites_vector::iterator sprite_iter;

			renderer_2d(device& dev);
			~renderer_2d();

			void add_line(const primitive::line_desc& line);
			void add_line(/* param*/);


			void add_sprite(const primitive::sprite_desc& sprite);
			void add_sprite(const math::vec2f& pos_, const math::vec2f& size_,
						    const math::rect tex_coord_, const float spin_, 
						    const texture_ptr texture_=default_texture);
			void render_all();

		private:
			lines_vector m_lines;
			sprites_vector m_sprites;

			vertex_declaration_ptr m_lines_decl;
			vertex_declaration_ptr m_sprites_decl;

			vertex_buffer_ptr m_vb;
			index_buffer_ptr m_ib;

			base_texture_ptr default_texture;

			device& m_device;
		};
	}
}