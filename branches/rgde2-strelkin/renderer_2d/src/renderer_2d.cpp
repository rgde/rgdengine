#include "stdafx.h"
#include "primitives.h"
#include "renderer_2d.h"

namespace rgde
{
	namespace render
	{
		renderer_2d::renderer_2d(const device& dev, const uint buff_size, const uint butch_size) : m_device(dev),
											m_buffer_offset(0), m_buffer_size(buff_size), m_butch_size(butch_size)
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
			vertex_decl = vertex_declaration::create(m_device, lines_vertex_desc, 4);
			//default_texture = base_texture::create(m_device, "");

			m_vb = vertex_buffer::create
			(
				m_device, vertex_decl, 
				m_buffer_size * sizeof(prim_vertex)*4, 
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

		void renderer_2d::render_all()
		{
			m_device.frame_begin();
			m_device.clear(math::color::White);

			if (!m_sprites.empty() /*|| m_lines.empty()*/)
			{
				 // если достигнут конец буфера, возвращаемся в начало
				if(m_buffer_offset >= m_buffer_size)
					m_buffer_offset = 0;

				primitive::prim_vertex* vertices = (primitive::prim_vertex*)m_vb->lock(m_buffer_offset * sizeof(prim_vertex), 
																			m_butch_size * sizeof(prim_vertex), 
																			m_buffer_offset ? lock_flags::nooverwrite | lock_flags::discard);

				// текущий размер данных
				ulong current_data_size = 0;
				// счётчик вершин
				uint i;

				// батчинг примитивов
				for(sprites_iter it = m_sprites.begin(); it != m_sprites.end(); ++it)
				{
					const sprite_desc &s = *it;
					const math::color &color= s.color;
					const math::rect &rect	= s.rect;

					math::vec2f hsize = s.size*0.5f;
					math::vec2f pos	= s.pos + hsize;

					float cosa	= ::cos(s.spin);
					float sina	= ::sin(s.spin);

					// Top left
					math::vec2f rotPos		= rotate_pos(-hsize[0], -hsize[1], sina, cosa) + pos;
					vertices[i].pos         = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
					vertices[i].tex_coord   = rect.get_top_left();
					vertices[i].color       = color;
					++i;

					current_data_size += sizeof(prim_vertex);
					if(current_data_size == m_butch_size)
					{
						current_data_size = 0;
						draw_butch();
					}

					// Top right
					math::vec2f rotPos      = rotate_pos(hsize[0], -hsize[1], sina, cosa) + pos;
					vertices[i].position    = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
					vertices[i].tex         = rect.get_top_right();
					vertices[i].color       = color;
					++i;

					current_data_size += sizeof(prim_vertex);
					if(current_data_size == m_butch_size)
					{
						current_data_size = 0;
						draw_butch();
					}

					// Bottom right
					math::vec2f rotPos      = rotate_pos(hsize[0], hsize[1], sina, cosa) + pos;
					vertices[i].position    = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
					vertices[i].tex         = rect.get_bottom_right();
					vertices[i].color       = color;
					++i;

					current_data_size += sizeof(prim_vertex);
					if(current_data_size == m_butch_size)
					{
						current_data_size = 0;
						draw_butch();
					}

					// Bottom left
					math::vec2f rotPos      = rotate_pos(-hsize[0], hsize[1], sina, cosa) + pos;
					vertices[i].position    = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
					vertices[i].tex         = rect.get_bottom_left();
					vertices[i].color       = color;
					++i;

					current_data_size += sizeof(prim_vertex);
					if(current_data_size == m_butch_size)
					{
						current_data_size = 0;
						draw_butch();
					}
				}

				m_vb->unlock();

			    if( current_data_size )
				{
					m_device.draw(render::triangle_list, 0, 0, m_buffer_offset, 0, current_data_size/sizeof(primitive::prim_vertex));
				}

				m_buffer_offset += m_butch_size;
			}
			m_device.frame_end();
			m_device.present();
		}

		void renderer_2d::drawButch(primitive::prim_vertex& vert)
		{
			// рисуем батч буфера, который был залит последним
			m_vb->unlock;
			m_device.draw(render::triangle_list, 0, 0, m_buffer_offset, 0, m_butch_size);

			// пока текущий батч рисуется, заполняем следующий
			m_buffer_offset += m_butch_size;
			// если достигнут конец буфера, возвращаемся в начало
			if(m_buffer_offset >= m_buffer_size)
				m_buffer_offset = 0;

			vert = m_vb->lock(m_buffer_offset * sizeof(prim_vertex), m_butch_size * sizeof(prim_vertex), 
							  m_buffer_offset ? lock_flags::nooverwrite | lock_flags::discard);
		}

	}
}