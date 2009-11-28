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

		renderer_2d::renderer_2d(device& dev, const uint buff_size, const uint butch_size) : m_device(dev),
											m_buffer_offset(0), m_buffer_size(buff_size), m_butch_size(butch_size)
		{
			init_primitives_data();
		}

		renderer_2d::~renderer_2d()
		{
		}

		void renderer_2d::init_primitives_data()
		{
			vertex_decl = vertex_declaration::create(m_device, prim_vertex_desc, 4);
			//default_texture = base_texture::create(m_device, "");

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
			if (!m_sprites.empty())
			{
				 // если достигнут конец буфера, возвращаемся в начало
				if(m_buffer_offset >= m_buffer_size)
					m_buffer_offset = 0;

				// вершины примитивов
				primitives_2d::prim_vertex* vertices = (primitives_2d::prim_vertex*)m_vb->lock(m_buffer_offset * sizeof(primitives_2d::prim_vertex), 
																			m_butch_size * sizeof(primitives_2d::prim_vertex), 
																			m_buffer_offset ? buffer::nooverwrite : buffer::discard);

				// текущий размер данных
				ulong current_data_size = 0;
				// счётчик вершин
				uint i = 0;
			
				// батчинг примитивов	
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
					vertices[i].pos         = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
					vertices[i].tex   = tex_coord.get_top_left();
					vertices[i].color       = color;
					++i;

					current_data_size += sizeof(primitives_2d::prim_vertex);
					if(current_data_size == m_butch_size)
					{
						current_data_size = 0;
						drawButch(vertices);
					}

					// Top right
					rotPos      = rotate_pos(hsize[0], -hsize[1], sina, cosa) + pos;
					vertices[i].pos			= math::Vec4f(rotPos[0], rotPos[1], 0, 0);
					vertices[i].tex         = tex_coord.get_top_right();
					vertices[i].color       = color;
					++i;

					current_data_size += sizeof(primitives_2d::prim_vertex);
					if(current_data_size == m_butch_size)
					{
						current_data_size = 0;
						drawButch(vertices);
					}

					// Bottom right
					rotPos      = rotate_pos(hsize[0], hsize[1], sina, cosa) + pos;
					vertices[i].pos			= math::Vec4f(rotPos[0], rotPos[1], 0, 0);
					vertices[i].tex         = tex_coord.get_bottom_right();
					vertices[i].color       = color;
					++i;

					current_data_size += sizeof(primitives_2d::prim_vertex);
					if(current_data_size == m_butch_size)
					{
						current_data_size = 0;
						drawButch(vertices);
					}

					// Bottom left
					rotPos      = rotate_pos(-hsize[0], hsize[1], sina, cosa) + pos;
					vertices[i].pos   = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
					vertices[i].tex         = tex_coord.get_bottom_left();
					vertices[i].color       = color;
					++i;

					current_data_size += sizeof(primitives_2d::prim_vertex);
					if(current_data_size == m_butch_size)
					{
						current_data_size = 0;
						drawButch(vertices);
					}
				}

				m_vb->unlock();

			    if( current_data_size )
				{
					m_device.set_stream_source( 0, m_vb, sizeof(primitives_2d::prim_vertex) );
					m_device.draw(render::triangle_list, 0, 0, m_buffer_offset, 0, current_data_size/sizeof(primitives_2d::prim_vertex));
				}

				m_buffer_offset += m_butch_size;
			}
		}

		void renderer_2d::drawButch(primitives_2d::prim_vertex* vert)
		{
			// рисуем батч буфера, который был залит последним
			m_vb->unlock();
			m_device.set_stream_source( 0, m_vb, sizeof(primitives_2d::prim_vertex) );
			m_device.draw(render::triangle_list, 0, 0, m_buffer_offset, 0, m_butch_size);

			// пока текущий батч рисуется, заполняем следующий
			m_buffer_offset += m_butch_size;
			// если достигнут конец буфера, возвращаемся в начало
			if(m_buffer_offset >= m_buffer_size)
				m_buffer_offset = 0;

			vert = (primitives_2d::prim_vertex*)m_vb->lock(m_buffer_offset * sizeof(primitives_2d::prim_vertex), m_butch_size * sizeof(primitives_2d::prim_vertex), 
				m_buffer_offset ? buffer::nooverwrite : buffer::discard);
		}
	}
}