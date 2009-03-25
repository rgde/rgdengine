#include "stdafx.h"
#include "sprite.h"

namespace rgde
{
	namespace render
	{
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

			struct sprite_vertex		// Our new vertex struct
			{
				float x, y, z, w;		// 3D position
				ulong color;			// Hex Color Value
				float u,v;				// texure coords
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
		{
			sprites_decl = vertex_declaration::create(m_device, sprites_vertex_desc, 3);

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
			int size = m_sprites.size()*2;

			m_vb = vertex_buffer::create
			(
				m_device, sprites_decl, 
				size * sizeof(sprite_vertex), 
				resource::default, 
				buffer::write_only
			);

			m_ib = index_buffer::create(
				m_device, 
				size * sizeof(ushort), 
				false, 
				resource::default, 
				buffer::write_only
			);

			m_reserved_size = size;
		}

		void renderer2d::fill_buffers()
		{
			//void *pVertices = m_vb->lock( 0, sizeof(cube_geom), 0 );
			//memcpy( pVertices, cube_geom, sizeof(cube_geom) );
			//m_vb->unlock();

			//void *indices = m_ib->lock( 0, sizeof(cube_ib), 0 );
			//memcpy( indices, cube_ib, sizeof(cube_ib) );
			//m_ib->unlock();

		}

		void renderer2d::update()
		{
			if (!check_size())
				create_buffers();

			fill_buffers();

			m_updated = true;
		}

		void renderer2d::render()
		{
			if (!m_updated || !m_ib || !m_vb)
				return;

			m_device.set_stream_source( 0, m_vb, sizeof(sprite_vertex) );
			m_device.set_index_buffer(m_ib);

			int sprites_num = m_sprites.size();

			m_device.draw(render::triangle_list, 0, 0, sprites_num * 4, 0, sprites_num *2);
		}

		void renderer2d::draw(const sprite& s)
		{
			m_sprites.push_back(s);
			m_updated = false;
		}
	}
}