#include "stdafx.h"
#include "renderer_2d.h"

namespace rgde
{
	namespace render
	{
		namespace
		{
			inline math::vec2f rotate_pos(float x, float y, float sina, float cosa)
			{
				return math::vec2f(x /** cosa - y * sina*/, /*x * sina +*/ y /** cosa*/);
			}
		}

		// декларация вершины 2d-примитива
		vertex_element prim_vertex_desc[] = 
		{
			{0, 0,  vertex_element::float4, vertex_element::default_method, vertex_element::positiont, 0},
			{0, 16, vertex_element::color4ub, vertex_element::default_method, vertex_element::color, 0},
			{0, 20, vertex_element::float2,	vertex_element::default_method, vertex_element::texcoord, 0},
			vertex_element::end_element
		};

		renderer_2d::renderer_2d(device& dev, const uint buff_size) : m_device(dev), current_data_size(0),
											m_buffer_offset(0), m_buffer_size(buff_size), m_batch_size(0),
											m_need_update(false)
		{
			init_primitives_data();
		}

		renderer_2d::~renderer_2d()
		{
		}

		void renderer_2d::init_primitives_data()
		{
			vertex_decl = vertex_declaration::create(m_device, prim_vertex_desc, 4);

			m_vb = vertex_buffer::create
			(
				m_device, vertex_decl, 
				m_buffer_size * sizeof(primitives_2d::prim_vertex)*4, 
				resource::default, 
				buffer::write_only | buffer::dynamic
			);

			m_ib = index_buffer::create(
				m_device, 
				m_buffer_size * sizeof(ushort)*6, 
				false, 
				resource::default, 
				buffer::write_only | buffer::dynamic
			);

			// индексы примитивов
			ushort *indices = (ushort*)m_ib->lock( 0, sizeof(ushort)*m_buffer_size*4, 0 );

			for (uint k = 0; k < m_buffer_size; ++k)
			{
				indices[k * 6 + 0] = k * 4 + 0;
				indices[k * 6 + 1] = k * 4 + 1;
				indices[k * 6 + 2] = k * 4 + 2;
				indices[k * 6 + 3] = k * 4 + 0;
				indices[k * 6 + 4] = k * 4 + 2;
				indices[k * 6 + 5] = k * 4 + 3;
			}

			m_ib->unlock();
			m_device.set_index_buffer(m_ib);
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

		void renderer_2d::add_sprite(const primitives_2d::sprite_desc& sprite)
		{
			m_sprites.push_back(sprite);
			m_need_update = true;
		}

		void renderer_2d::add_sprite(const math::vec2f& pos_, const math::vec2f& size_,
									 const math::rect tex_coord_, const float spin_, 
									 const math::color& color_, const texture_ptr texture_)
		{
			primitives_2d::sprite_desc sprite;
			
			sprite.pos       = pos_;
			sprite.size      = size_;
			sprite.tex_coord = tex_coord_;
			sprite.spin      = spin_;
			sprite.color     = color_;
			sprite.texture   = texture_;

			m_sprites.push_back(sprite);
			m_need_update = true;
		}

		renderer_2d::sprites_vector& renderer_2d::get_sprites()
		{
			return m_sprites;
		}

		void renderer_2d::clear_sprites()
		{
			m_sprites.clear();
		}

		void renderer_2d::clear_lines()
		{
			m_lines.clear();
		}

		void renderer_2d::clear_all()
		{
			clear_sprites();
			clear_lines();
		}

		void renderer_2d::prepare_data()
		{
			if (m_sprites.empty())
				return;

			m_device.set_lighting(false);
			m_device.set_alpha_blend(true);
			m_device.set_alpha_test(true);
			m_device.set_blend_mode(render::blend_srcalpha, render::blend_invsrcalpha);

			// если достигнут конец буфера, возвращаемся в начало
//			if(m_buffer_offset >= m_buffer_size)
//				m_buffer_offset = 0;

			// вершины примитивов
			primitives_2d::prim_vertex* vertices;

			//if (m_need_update)
			//{
				vertices = (primitives_2d::prim_vertex*)m_vb->lock(m_buffer_offset * sizeof(primitives_2d::prim_vertex), 
																				m_batch_size * sizeof(primitives_2d::prim_vertex), 
																				m_buffer_offset ? buffer::nooverwrite : buffer::discard);
			//	m_need_update = false;
			//}
			
			current_data_size = 0;

			for(sprites_iter it = m_sprites.begin(); it != m_sprites.end(); ++it)
			{
				const primitives_2d::sprite_desc &s = *it;
				const math::color &color= s.color;
				const math::rect &tex_coord	= s.tex_coord;

				math::vec2f hsize = s.size*0.5f;
				math::vec2f pos	= s.pos + hsize;

				float cosa	= ::cos(s.spin);
				float sina	= ::sin(s.spin);

				// Top left
				math::vec2f rotPos		= rotate_pos(-hsize[0], -hsize[1], sina, cosa) + pos;
				vertices->pos   = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
				vertices->tex   = tex_coord.get_top_left();
				vertices->color = color;
				++vertices;

					// Top right
				rotPos			= rotate_pos(hsize[0], -hsize[1], sina, cosa) + pos;
				vertices->pos	= math::Vec4f(rotPos[0], rotPos[1], 0, 0);
				vertices->tex   = tex_coord.get_top_right();
				vertices->color = color;
				++vertices;

					// Bottom right
				rotPos			= rotate_pos(hsize[0], hsize[1], sina, cosa) + pos;
				vertices->pos	= math::Vec4f(rotPos[0], rotPos[1], 0, 0);
				vertices->tex   = tex_coord.get_bottom_right();
				vertices->color = color;
				++vertices;

					// Bottom left
				rotPos			= rotate_pos(-hsize[0], hsize[1], sina, cosa) + pos;
				vertices->pos   = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
				vertices->tex   = tex_coord.get_bottom_left();
				vertices->color = color;
				++vertices;

				current_data_size += sizeof(primitives_2d::prim_vertex)*4;
			}

			m_vb->unlock();
		}

		void renderer_2d::render_all()
		{
			prepare_data();

			primitives_2d::sprite_desc* s = &m_sprites.front();
			texture_ptr cur_tex = s->texture;
			int size = m_sprites.size();
			int start_sprite = 0;
			for (int i = 0; i < size; ++i)
			{
				primitives_2d::sprite_desc &sprite = s[i];

				if (cur_tex != sprite.texture)
				{
					int cur_sprite = i - 1;
					int num_sprites = cur_sprite - start_sprite + 1;

					if (num_sprites > 0)
					{
						m_device.set_texture(cur_tex, 0);
						m_device.set_stream_source( 0, m_vb, sizeof(primitives_2d::prim_vertex) );
						m_device.draw(render::triangle_list, 0, 0, num_sprites * 4, 6 * start_sprite, num_sprites * 2);
					}
					cur_tex = sprite.texture;
				}
			}		
		}

		void renderer_2d::draw_batch(primitives_2d::prim_vertex* vert)
		{
		}
	}
}