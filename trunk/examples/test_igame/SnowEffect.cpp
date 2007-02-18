#include "SnowEffect.h"

namespace game
{
	namespace effects
	{
	
		void SnowSprite::init() 
		{
			using namespace math;

			float s = rangeRandom(5.0f, 25.0f);
			uPriority = (uint)s;
			size = Vec2f(s, s);
			uint c = (uint)(255*s/25.0f);
			color = Color(255, 255, 255, c);
			//color = 0xFFFFFFFF;
			rect = Rect(0, 0, 1, 1);
			pos = Vec2f(rangeRandom(0,800.0f), rangeRandom(0,620.0f));

			spin = 0;
			rot_speed = rangeRandom(0,1.0f);

			velocity = Vec2f(rangeRandom(0,0.55f), 1.5f)*s;
		}

		SnowEffect::SnowEffect(uint numSprites)
		{
			m_font = render::IFont::Create(12, L"Arial", render::IFont::Heavy);
			m_texture = render::ITexture::Create("ZColumns/Snowflake.png"); 

			m_sprites.resize(numSprites);

			std::for_each(m_sprites.begin(), m_sprites.end(), 
				boost::bind(&SnowSprite::init, _1));

			for (Sprites::iterator it = m_sprites.begin(); it != m_sprites.end(); ++it)
			{
				it->pTexture = m_texture;
			}
		}

		void SnowEffect::update(float dt)
		{
			render::TheDevice::Get().showFPS(m_font);


			std::for_each(m_sprites.begin(), m_sprites.end(), 
				boost::bind(&SnowEffect::updateSprite, this, _1, dt));


			std::for_each(m_sprites.begin(), m_sprites.end(), 
				boost::bind(&render::CSpriteManager::addSprite, &m_sprite_renderer, _1));

			//for (Sprites::iterator it = m_sprites.begin(); it != m_sprites.end(); ++it)
			//{
			//	m_sprite_renderer.addSprite(*it);
			//}
		}

		void SnowEffect::initSprite(SnowSprite& s)
		{				
			s.init();
			s.pos = math::Vec2f(math::rangeRandom (0, 800.0f), -s.size[1]*2);
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