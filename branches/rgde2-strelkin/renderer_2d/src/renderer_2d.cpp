#include "stdafx.h"
#include "primitives.h"
#include "renderer_2d.h"

namespace rgde
{
	namespace render
	{
		renderer_2d::renderer_2d() : m_device(get_handle())
		{
			init_device();
			init_primitives_data();
		}

		renderer_2d::~renderer_2d()
		{
			for(lines_iter it = m_lines.begin(); it != m_lines.end(); ++it)
			{
				line_desc* l = *it;
				delete l;
			}
			for(sprites_iter it = m_sprites.begin(); it != m_sprites.end(); ++it)
			{
				sprite_desc* s = *it;
				delete s;
			}
		}

		void renderer_2d::init_device()
		{
			m_device.set_lighting(false);
			m_device.set_alpha_blend(false);
			m_device.set_alpha_test(false);
		}

		void renderer_2d::init_primitives_data()
		{
			vertex_decl = vertex_declaration::create(m_device, lines_vertex_desc, 3);
			//default_texture = base_texture::create(m_device, "");
			
			int size = (m_sprites.size() + m_lines.size())*2;

			m_vb = vertex_buffer::create
			(
				m_device, vertex_decl, 
				size * sizeof(prim_vertex)*3, 
				resource::default, 
				buffer::write_only
			);

			m_ib = index_buffer::create(
				m_device, 
				size * sizeof(ushort)*6, 
				false, 
				resource::default, 
				buffer::write_only
			);
		}

		void renderer_2d::add_line(const primitives_2d::line_desc& line)
		{
			m_lines.push_back(line);
		}

		void renderer_2d::add_line(/* param*/)
		{
			primitives_2d::line_desc line;
			// setup line params
			m_lines.push_back(line);
		}

		void renderer_2d::add_sprite(const primitives_2d::sprite_desc& line)
		{
			m_sprites.push_back(sprite);
		}

		void renderer_2d::add_sprite(const math::vec2f& pos_, const math::vec2f& size_,
									 const math::rect tex_coord_, const float spin_, 
									 const texture_ptr texture_)
		{
			primitives_2d::sprite_desc sprite;
			
			sprite.pos       = pos_;
			sprite.size      = size_;
			sprite.tex_coord = tex_coord_;
			sprite.spin      = spin_;
			sprite.texture   = texture_;

			m_sprites.push_back(sprite);
		}

		void renderer_2d::clear_sprites()
		{
			m_sprites.clear();
		}

		void renderer_2d::clear_lines()
		{
			m_lines.clear();
		}

		void renderer_2d::render_all()
		{
			m_device.frame_begin();
			m_device.clear(math::color::White);

			// render

			m_device.frame_end();
			m_device.present();
		}

	}
}