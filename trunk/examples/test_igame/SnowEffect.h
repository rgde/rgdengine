#pragma once

#include <engine.h>

namespace game
{
	namespace effects
	{
		struct SnowSprite : public render::SSprite
		{
			void init();

			float rot_speed;
			math::Vec2f velocity;
		};

		class SnowEffect : public game::IDynamicObject
		{
		public:
			SnowEffect(uint numSprites = 500);
			void update(float dt);

		protected:
			void initSprite(SnowSprite& s);
			void updateSprite(SnowSprite& s, float dt);
			bool isInsideScreen(const SnowSprite& s);

		private:
			float m_rate;
			float m_time;
			float m_gravity;
			render::CSpriteManager m_sprite_renderer;

			render::PTexture m_texture;

			typedef std::vector<SnowSprite> Sprites;
			Sprites m_sprites;

			render::PFont m_font;
		};
	}
}