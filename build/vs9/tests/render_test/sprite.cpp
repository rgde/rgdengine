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
			: x(0),y(0),w(0),h(0)
			, uvx(0), uvy(0), uvw(1.0f), uvh(1.0f)
			, spin(0)
			, priority(0)
			, color(0xFFFFFFFF)
		{
		}

		sprite::sprite( const math::vec2f& pos, const math::vec2f& size, 
			math::color color_,texture_ptr texture_,
			float spin_, const math::rect& uv,
			unsigned long priority_)
			: x(pos[0]),y(pos[1]),w(size[0]),h(size[1])
			, uvx(uv.x), uvy(uv.y), uvw(uv.w), uvh(uv.h)
			, spin (spin_)
			, priority (priority_)
			, texture (texture_)
			, color (color_.data)
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
				//math::vec4f		position;		// 3D position
				float x,y,z,w;
				ulong color;			// Hex Color Value
				//math::vec2f		tex;				// texure coords
				float u,v;
			};
		}

		canvas::canvas(device& dev)
			: m_device(dev)
			, m_updated(false)
			, m_scale(1.0f, 1.0f)
			, m_reserved_size(0)
			, m_need_ib_update(false)
		{
			sprites_decl = vertex_declaration::create(m_device, sprites_vertex_desc, 4);
		}

		canvas::~canvas()
		{
		}

		bool canvas::check_size()
		{
			if (m_sprites.size() > m_reserved_size)
			{
				m_vb.reset();
				m_ib.reset();
				return false;
			}
			return true;
		}

		void canvas::create_buffers()
		{
			size_t size = m_sprites.size()*2;

			m_vb = vertex_buffer::create
			(
				m_device, 
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

		void canvas::fill_buffers()
		{
			if (m_need_ib_update)
			{
				m_need_ib_update = false;
				ushort *indices = (ushort*)m_ib->lock( 0, 
					sizeof(ushort)*m_reserved_size*4, 0 );
			
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

			sprite_vertex* v = (sprite_vertex*)m_vb->lock( 0, 
				sizeof(sprite_vertex)*m_sprites.size()*4, 0 );

			unsigned i	= 0;
			for (sprite_iter it = m_sprites.begin(); it != m_sprites.end(); ++it)
			{
				// Срайты масштабируются только при записи в буфер
				const sprite &s = *it;

				float hsize_w = s.w * 0.5f;
				float hsize_h = s.h * 0.5f;

				// Сразу же масштабируем позицию и размер
				math::vec2f pos(s.x + hsize_w, s.y + hsize_h); //= math::vec2f(s.pos[0] * m_scale[0], s.pos[1] * m_scale[1]);

				float cosa				= ::cos(s.spin);
				float sina				= ::sin(s.spin);

				// Top left
				math::vec2f rotPos		= rotate_pos(-hsize_w, -hsize_h, sina, cosa) + pos;
				v->x = rotPos[0]; v->y = rotPos[1]; v->z = v->w = 0;
				v->u = s.uvx; v->v = s.uvy;
				v->color = s.color;
				++v;

				// Top right
				rotPos = rotate_pos(hsize_w, -hsize_h, sina, cosa) + pos;
				v->x = rotPos[0]; v->y = rotPos[1]; v->z = v->w = 0;
				v->u = s.uvx + s.uvw; v->v = s.uvy;
				v->color = s.color;
				++v;

				// Bottom right
				rotPos = rotate_pos(hsize_w, hsize_h, sina, cosa) + pos;
				v->x = rotPos[0]; v->y = rotPos[1]; v->z = v->w = 0;
				v->u = s.uvx + s.uvw; v->v = s.uvy + s.uvh;
				v->color = s.color;
				++v;

				// Bottom left
				rotPos = rotate_pos(-hsize_w, hsize_h, sina, cosa) + pos;
				v->x = rotPos[0]; v->y = rotPos[1]; v->z = v->w = 0;
				v->u = s.uvx; v->v = s.uvy + s.uvh;
				v->color = s.color;
				++v;
			}

			//
			//memcpy( pVertices, cube_geom, sizeof(cube_geom) );
			m_vb->unlock();
		}

		void canvas::update()
		{
			if (m_sprites.empty())
				return;

			if (!check_size())
				create_buffers();

			fill_buffers();

			m_updated = true;
		}

		void canvas::render()
		{
			if (m_sprites.empty() || !m_updated || !m_ib || !m_vb)
				return;

			m_device.set( 0, m_vb, sizeof(sprite_vertex) );
			m_device.set(m_ib);

			uint sprites_num = (uint)m_sprites.size();

			//TODO: currently only first texture is used!
			m_device.set(m_sprites.front().texture, 0);
			m_device.set(sprites_decl);

			m_device.draw(render::triangle_list, 0, 0, sprites_num * 4, 0, sprites_num *2);

			m_device.set(texture_ptr(), 0);
		}

		void canvas::draw(const sprite& s)
		{
			m_sprites.push_back(s);
			m_updated = false;
		}

		void canvas::clear()
		{
			m_sprites.clear();
		}

		void canvas::draw(const math::rect& r, const math::color& c, texture_ptr t)
		{
			m_sprites.push_back(sprite(r.position, r.size, c, t));
		}
	}
}