#include "precompiled.h"

#include <rgde/render/sprites.h>

namespace render
{
	sprite::sprite()
		: spin(0)
		, priority(0)
		, rect(0, 0, 1, 1)
	{
	}

	sprite::sprite( const math::vec2f& pos_, const math::vec2f& size_, 
		const math::Color& color_,render::texture_ptr pTexture_,
		float spin_, const math::Rect& rect_,
		unsigned long uPriority_) 
		: rect (rect_)
		, pos (pos_)
		, size (size_)
		, spin (spin_)
		, priority (uPriority_)
		, texture (pTexture_)
		, color (color_)
	{
	}

	sprite_manager::sprite_manager(int priority)
		: m_screen_size(800, 600)
		, m_reserved_size(0)
		, m_sprites_rendered(0)
		, m_sorted(false)
		, m_updated(true)
		, rendererable(priority)
		, m_origin(0, 0)
		, m_geometry(true)
		, m_aditive(false)
	{
		//base::lmsg << "sprite_manager::sprite_manager()";
		math::vec2f vFrontBufferSize= render::render_device::get().getBackBufferSize();
		m_scale = vFrontBufferSize / m_screen_size;

		m_effect = effect::create("SpriteManager.fx");
		m_renderInfo.render_func = boost::bind(&sprite_manager::render, this);
	}

	sprite_manager::~sprite_manager()
	{
	}

	void sprite_manager::add_sprite(const sprite &s)
	{
		m_updated = false;
		m_sprites.push_back(s);
	}

	inline bool sorting_pred(const sprite& s1, sprite& s2)
	{
		return s1.priority == s2.priority ?
			s1.texture < s2.texture
			:
			s1.priority < s2.priority;
	}

	inline math::vec2f rotatePos(float x, float y, float sina, float cosa)
	{
		return math::vec2f(x * cosa - y * sina, x * sina + y * cosa);
	}

	inline void rotatePos(math::vec2f& pos, float sina, float cosa)
	{
		float x = pos[0];
		float y = pos[1];
		pos[0] = x * cosa - y * sina;
		pos[1] = x * sina + y * cosa;
	}

	void sprite_manager::update()
	{
		if (m_sprites.empty())
			m_updated = true;

		if (m_updated)
			return;

		using namespace math;

		std::sort( m_sprites.begin(), m_sprites.end(), sorting_pred );

		unsigned num_sprites	= (unsigned)m_sprites.size();
		// На случай, если число спрайтов в векторе больше, чем зарезервировано в буферах
		if ((num_sprites > m_reserved_size))
		{
			m_reserved_size = num_sprites;

			geometry::indexies &indexies = m_geometry.lock_ib();
			
			if (indexies.size() < num_sprites * 6)
				indexies.resize(num_sprites * 6);

			for (unsigned i = 0; i < num_sprites; ++i)
			{
				indexies[i * 6 + 0] = i * 4 + 0;
				indexies[i * 6 + 1] = i * 4 + 1;
				indexies[i * 6 + 2] = i * 4 + 2;
				indexies[i * 6 + 3] = i * 4 + 0;
				indexies[i * 6 + 4] = i * 4 + 2;
				indexies[i * 6 + 5] = i * 4 + 3;
			}
			m_geometry.unlock_ib();
		}

		geometry::vertexies &vertexies	= m_geometry.lock_vb();
		
		if (vertexies.size() < num_sprites * 4)
			vertexies.resize(num_sprites * 4);

		//unsigned i	= 0;

		geometry::vertex_type* v = &(*vertexies.begin());

		for (sprites_iter it = m_sprites.begin(); it != m_sprites.end(); ++it)
		{
			// Срайты масштабируются только при записи в буфер
			const sprite &sprite	= *it;
			const math::Color &color= sprite.color;
			const math::Rect &rect	= sprite.rect;

			// Сразу же масштабируем позицию и размер
			math::vec2f hsize(sprite.size[0] * m_scale[0]*0.5f, sprite.size[1] * m_scale[1]*0.5f);
			math::vec2f pos(sprite.pos[0] * m_scale[0], sprite.pos[1] * m_scale[1]);

			float cosa = ::cos(sprite.spin);
			float sina = ::sin(sprite.spin);

			// Top left
			math::vec2f rotPos = rotatePos(-hsize[0], -hsize[1], sina, cosa) + pos;
			v->position.set(rotPos[0], rotPos[1], 0, 0);
			v->tex = rect.get_top_left();
			v->color = color;
			v++;

			// Top right
			rotPos = rotatePos(hsize[0], -hsize[1], sina, cosa) + pos;
			v->position.set(rotPos[0], rotPos[1], 0, 0);
			v->tex = rect.get_top_right();
			v->color = color;
			v++;

			// Bottom right
			rotPos = rotatePos(hsize[0], hsize[1], sina, cosa) + pos;
			v->position.set(rotPos[0], rotPos[1], 0, 0);
			v->tex = rect.get_bottom_right();
			v->color = color;
			v++;

			// Bottom left
			rotPos = rotatePos(-hsize[0], hsize[1], sina, cosa) + pos;
			v->position.set(rotPos[0], rotPos[1], 0, 0);
			v->tex = rect.get_bottom_left();
			v->color = color;
			v++;
		}
		m_geometry.unlock_vb();

		m_updated = true;
	}

	void sprite_manager::render()
	{
		if (m_sprites.empty()) 
			return;

		update();

		render::effect::technique *pTech = NULL;

		if (m_aditive)
			pTech = m_effect->find_technique("aditive");
		else
			pTech = m_effect->find_technique("alpha");

		assert(0 != pTech && "sprite_manager::render(): Can't find effect technique!");
		pTech->begin();

		for (unsigned iPass = 0; iPass < pTech->get_passes().size(); iPass++)
		{
			effect::technique::pass& pass = *pTech->get_passes()[iPass];
			pass.begin();

			unsigned nSpritesRendered = 0;
			effect::parameter *textureShaderParam	= m_effect->get_params()["spriteTexture"];

			assert(0 != textureShaderParam && "m_effect->get_params()[\"spriteTexture\"] == NULL !");

			uint start_sprite = 0;
			sprite* sp = &m_sprites.front();
			texture_ptr cur_tex = sp->texture;
			for (uint i = 0, size = m_sprites.size(); i < size; ++i)
			{
				sprite &sprite = sp[i];

				// если = то отрисовать
				if (cur_tex != sprite.texture)
				{
					int cur_sprite = i - 1;
					int num_sprites = cur_sprite - start_sprite + 1;
					//i == m_sprites.size()-1
					if (num_sprites > 0)
					{
						textureShaderParam->set(cur_tex);
						m_effect->commit_changes();
						m_geometry.render(TriangleList, 0, 4 * start_sprite, num_sprites * 4, 6 * start_sprite, num_sprites * 2);
						nSpritesRendered += num_sprites;
					}
					cur_tex = sprite.texture;
					start_sprite = i;
				}

				if (i == m_sprites.size()-1)
				{
					int cur_sprite = i;
					int num_sprites = i - start_sprite + 1;
					if (num_sprites > 0)
					{
						textureShaderParam->set(cur_tex);
						m_effect->commit_changes();
						m_geometry.render(TriangleList, 0, 4 * start_sprite, num_sprites * 4, 6 * start_sprite, num_sprites * 2);
						nSpritesRendered += num_sprites;
					}
				}
			}
			
			//sprites_iter it = m_sprites.begin();
			//sprites_iter start_sprite = it;
			//texture_ptr cur_tex = it->texture;
			//for (; it != m_sprites.end(); ++it)
			//{
			//	sprite &sprite = *it;

			//	// если = то отрисовать
			//	if (cur_tex != sprite.texture)
			//	{
			//		int num_sprites = it - start_sprite;
			//		
			//		if (num_sprites > 0)
			//		{
			//			int start_index = start_sprite - m_sprites.begin();
			//			textureShaderParam->set(cur_tex);
			//			m_effect->commit_changes();
			//			m_geometry.render(PrimTypeTriangleList, 0, 4 * start_index, num_sprites * 4, 6 * start_index, num_sprites * 2);
			//			nSpritesRendered += num_sprites;
			//		}
			//		cur_tex = sprite.texture;
			//		start_sprite = it;
			//	}
			//}
			m_sprites_rendered = nSpritesRendered;

			pass.end();
		}
		pTech->end();

		m_sprites.resize(0);
	}

	void sprite_manager::onLostDevice()
	{
	}

	void sprite_manager::onResetDevice()
	{
		// calc scale coefs
		math::vec2f front_buffer_size = render::render_device::get().getBackBufferSize();
		m_scale = front_buffer_size / m_screen_size;
		update();
	}
}