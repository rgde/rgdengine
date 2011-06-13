#include "SnowEffect.h"

namespace game
{
	namespace effects
	{
	
		void SnowSprite::init() 
		{
			using namespace math;

			float s = rangeRandom(5.0f, 25.0f);
			priority = (uint)s;
			size = vec2f(s, s);
			uint c = (uint)(255*s/25.0f);
			color = Color(255, 255, 255, c);
			//color = 0xFFFFFFFF;
			rect = Rect(0, 0, 1, 1);
			pos = vec2f(rangeRandom(0,800.0f), rangeRandom(0,620.0f));

			spin = 0;
			rot_speed = rangeRandom(0,1.0f);

			velocity = vec2f(rangeRandom(0,0.55f), 1.5f)*s;
		}

		SnowEffect::SnowEffect(uint numSprites)
		{
			m_font = render::font::create(12, L"Arial", render::font::Heavy);
			m_texture = render::texture::create("ZColumns/snow.dds"); 

			m_sprites.resize(numSprites);

			std::for_each(m_sprites.begin(), m_sprites.end(), 
				boost::bind(&SnowSprite::init, _1));

			for (Sprites::iterator it = m_sprites.begin(); it != m_sprites.end(); ++it)
			{
				it->texture = m_texture;
			}
		}

		void SnowEffect::update(float dt)
		{
			float abs_time = game::game_system::get().get_timer().absolute_time();
			render::render_device::get().draw_fps(abs_time, m_font);


			std::for_each(m_sprites.begin(), m_sprites.end(), 
				boost::bind(&SnowEffect::updateSprite, this, _1, dt));


			std::for_each(m_sprites.begin(), m_sprites.end(), 
				boost::bind(&render::sprite_manager::add, &m_sprite_renderer, _1));

			//for (Sprites::iterator it = m_sprites.begin(); it != m_sprites.end(); ++it)
			//{
			//	m_sprite_renderer.add(*it);
			//}
		}

		void SnowEffect::initSprite(SnowSprite& s)
		{				
			s.init();
			s.pos = math::vec2f(math::rangeRandom (0, 800.0f), -s.size[1]*2);
			//s.pTexture = m_texture;
		}

		void SnowEffect::updateSprite(SnowSprite& s, float dt)
		{
			if (isInsideScreen(s))
			{
				initSprite(s);
				return;
			}

			//s.velocity[1] += m_gravity * dt;
			s.spin += s.rot_speed*dt;
			s.pos += s.velocity*dt;
		}

		bool SnowEffect::isInsideScreen(const SnowSprite& s)
		{
			return s.pos[0] - s.size[0] > 800 || s.pos[1] - s.size[1] > 600;
		}

	}
}