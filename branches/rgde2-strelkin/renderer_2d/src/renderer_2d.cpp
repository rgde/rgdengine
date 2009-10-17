#include "stdafx.h"
#include "primitives.h"
#include "renderer_2d.h"

namespace rgde
{
	namespace render
	{
		renderer_2d::renderer_2d(device& dev) : m_device(dev)
		{
			m_lines_decl = vertex_declaration::create(m_device, lines_vertex_desc, 3);
			m_sprites_decl = vertex_declaration::create(m_device, sprites_vertex_desc, 3);

//			default_texture = base_texture::create(m_device, "");
		}

		renderer_2d::~renderer_2d()
		{
		}

		void renderer_2d::add_line(const primitive::line_desc& line)
		{
			m_lines.push_back(line);
		}

		void renderer_2d::add_line(/* param*/)
		{
			primitive::line_desc line;
			// setup line params
			m_lines.push_back(line);
		}

		void renderer_2d::add_sprite(const primitive::sprite_desc& line)
		{
			m_sprites.push_back(sprite);
		}

		void renderer_2d::add_sprite(const math::vec2f& pos_, const math::vec2f& size_,
									 const math::rect tex_coord_, const float spin_, 
									 const texture_ptr texture_)
		{
			primitive::sprite_desc sprite;
			
			sprite.pos       = pos_;
			sprite.size      = size_;
			sprite.tex_coord = tex_coord_;
			sprite.spin      = spin_;
			sprite.texture   = texture_;

			m_sprites.push_back(sprite);
		}
	}
}