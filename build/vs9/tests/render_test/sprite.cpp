#include "stdafx.h"
#include "sprite.h"

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
		//////////////////////////////////////////////////////////////////////////

		sprite::sprite()
			: spin(0), 
			priority(0), 
			rect(0, 0, 1, 1)
		{
		}

		sprite::sprite( const math::vec2f& pos_, const math::vec2f& size_, 
			const math::color& color_,texture_ptr texture_,
			float spin_, const math::rect& rect_,
			unsigned long priority_)
			: rect (rect_)
			, pos (pos_)
			, size (size_)
			, spin (spin_)
			, priority (priority_)
			, texture (texture_)
			, color (color_)
		{
		}

		//////////////////////////////////////////////////////////////////////////

		namespace 
		{			
			vertex_element sprites_vertex_desc[] = 
			{
				{0, 0,  vertex_element::float4, vertex_element::default_method, vertex_element::positiont, 0},
				{0, 16, vertex_element::color4ub,	vertex_element::default_method, vertex_element::color,	0},
				{0, 20, vertex_element::float2,	vertex_element::default_method, vertex_element::texcoord,0},
				vertex_element::end_element
			};

			
			//math::Color		color;
			//math::vec2f		tex;

			struct sprite_vertex		// Our new vertex struct
			{
				math::vec4f		position;		// 3D position
				ulong color;			// Hex Color Value
				math::vec2f		tex;				// texure coords
			};

			struct line_vertex			// Our new vertex struct
			{
				float x, y, z;			// 3D position
				ulong color;			// Hex Color Value
			};
		}

		renderer2d::renderer2d(device& dev)
			: m_device(dev)
			, m_updated(false)
			, m_scale(1.0f, 1.0f)
			, m_reserved_size(0)
			, m_need_ib_update(false)
		{
			sprites_decl = vertex_declaration::create(m_device, sprites_vertex_desc, 4);
		}

		renderer2d::~renderer2d()
		{
		}

		bool renderer2d::check_size()
		{
			if (m_sprites.size() > m_reserved_size)
			{
				m_vb.reset();
				m_ib.reset();
				return false;
			}
			return true;
		}

		void renderer2d::create_buffers()
		{
			size_t size = m_sprites.size()*2;

			m_vb = vertex_buffer::create
			(
				m_device, sprites_decl, 
				size * sizeof(sprite_vertex)*4, 
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

			m_need_ib_update = true;


			m_reserved_size = size;
		}

		void renderer2d::fill_buffers()
		{
			if (m_need_ib_update)
			{
				m_need_ib_update = false;
				ushort *indices = (ushort*)m_ib->lock( 0, 
					sizeof(ushort)*m_reserved_size*4, 0 );

				if (!indices)
				{
					__asm nop;
				}
				
				for (uint i = 0; i < m_reserved_size; ++i)
				{
					indices[i * 6 + 0] = i * 4 + 0;
					indices[i * 6 + 1] = i * 4 + 1;
					indices[i * 6 + 2] = i * 4 + 2;
					indices[i * 6 + 3] = i * 4 + 0;
					indices[i * 6 + 4] = i * 4 + 2;
					indices[i * 6 + 5] = i * 4 + 3;
				}

				m_ib->unlock();
			}

			sprite_vertex* vertices = (sprite_vertex*)m_vb->lock( 0, 
				sizeof(sprite_vertex)*m_sprites.size()*4, 0 );

			unsigned i	= 0;
			for (sprite_iter it = m_sprites.begin(); it != m_sprites.end(); ++it)
			{
				// Срайты масштабируются только при записи в буфер
				const sprite &s = *it;
				const math::color &color= s.color;
				const math::rect &rect	= s.rect;
				// Сразу же масштабируем позицию и размер
				math::vec2f hsize		= s.size*0.5f;//= math::vec2f(s.size[0] * m_scale[0], s.size[1] * m_scale[1]) / 2.0f;
				math::vec2f pos			= s.pos + hsize; //= math::vec2f(s.pos[0] * m_scale[0], s.pos[1] * m_scale[1]);

				float cosa				= ::cos(s.spin);
				float sina				= ::sin(s.spin);

				// Top left
				math::vec2f rotPos		= rotate_pos(-hsize[0], -hsize[1], sina, cosa) + pos;
				vertices[i].position = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
				vertices[i].tex = rect.get_top_left();
				vertices[i].color = color;
				++i;

				// Top right
				rotPos = rotate_pos(hsize[0], -hsize[1], sina, cosa) + pos;
				vertices[i].position = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
				vertices[i].tex = rect.get_top_right();
				vertices[i].color = color;
				++i;

				// Bottom right
				rotPos = rotate_pos(hsize[0], hsize[1], sina, cosa) + pos;
				vertices[i].position = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
				vertices[i].tex = rect.get_bottom_right();
				vertices[i].color = color;
				++i;

				// Bottom left
				rotPos = rotate_pos(-hsize[0], hsize[1], sina, cosa) + pos;
				vertices[i].position = math::Vec4f(rotPos[0], rotPos[1], 0, 0);
				vertices[i].tex = rect.get_bottom_left();
				vertices[i].color = color;
				++i;
			}

			//
			//memcpy( pVertices, cube_geom, sizeof(cube_geom) );
			m_vb->unlock();
		}

		void renderer2d::update()
		{
			if (m_sprites.empty())
				return;

			if (!check_size())
				create_buffers();

			fill_buffers();

			m_updated = true;
		}

		void renderer2d::render()
		{
			if (m_sprites.empty() || !m_updated || !m_ib || !m_vb)
				return;

			m_device.set_stream_source( 0, m_vb, sizeof(sprite_vertex) );
			m_device.set_index_buffer(m_ib);

			uint sprites_num = (uint)m_sprites.size();

			m_device.draw(render::triangle_list, 0, 0, sprites_num * 4, 0, sprites_num *2);
		}

		void renderer2d::draw(const sprite& s)
		{
			m_sprites.push_back(s);
			m_updated = false;
		}

		void renderer2d::clear()
		{
			m_sprites.clear();
		}

		void renderer2d::draw(const math::rect& r, const math::color& c, texture_ptr t)
		{
			m_sprites.push_back(sprite(r.position, r.size, c, t));
		}
	}
}