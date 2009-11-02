/**--------------------------------------------------------------------------------------
RGDE Engine
----------------------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:		dxprg@mail.ru
Created:	8 june 2005
---------------------------------------------------------------------------------------*/
//RGDE
#include "sprites.h"

namespace math
{
	using namespace Math;
}

namespace rgde
{
	namespace exapmles
	{
		namespace sprites
		{

			sprite_example::sprite_example() : 
				m_priorities (1),
				m_nSpritesPerPriority (100),
				m_bDebugLog (false),
				m_bSortSpritesBeforeLog (true),
				m_sprites_seed (math::Vec2f (770, 570))
			{
				{
					using namespace input;

					m_cEsc.attach(L"Quit");
					Input::get_device(types::Keyboard)->get_control(types::KeyEscape)->bind(L"Quit");            
					m_cEsc += boost::bind(&sprite_example::onEsc, this);
				}

				m_font = render::IFont::create(12, L"Arial", render::IFont::Heavy);

				std::vector<render::texture_ptr> vTextures;
				vTextures.push_back(render::texture::create( "Sprites/test01.jpg" ));
				vTextures.push_back(render::texture::create( "Sprites/test02.jpg" ));
				vTextures.push_back(render::texture::create( "Sprites/test03.jpg" ));
				vTextures.push_back(render::texture::create( "Sprites/test04.jpg" ));
				vTextures.push_back(render::texture::create( "Sprites/test05.bmp" ));

				for( unsigned priority = m_priorities; priority > 0; --priority )
					for( unsigned sprite = m_nSpritesPerPriority; sprite > 0; --sprite )
					{
						float x = m_sprites_seed[0];
						float y = m_sprites_seed[1];

						math::Rect rect(0, 0, 1, 1);

						using math::unitRandom;
						using math::rangeRandom;

						for (uint i = 0; i < vTextures.size(); ++i)
						{
							math::Vec2f size(rangeRandom(0,80),rangeRandom(0,80));
							math::Vec2f pos(rangeRandom(0, x), rangeRandom(0, y));
							math::Color color;
							color.set(unitRandom(),	unitRandom(), unitRandom(),	unitRandom());

							float spin = rangeRandom (0, math::TWO_PI);

							render::sprite sprite(pos, size, color, vTextures[i], spin, rect, priority);
							m_sprites.push_back(sprite);
						}
					}
			}

			void sprite_example::onEsc()
			{
				core::application::get()->close();
			}

			void sprite_example::update(float dt)
			{
				render::TheDevice::get().showFPS(m_font);

				for (render::SpriteManager::SpritesIter it = m_sprites.begin(); it != m_sprites.end(); ++it)
				{
					it->spin += dt;
					m_sprite_renderer.addSprite(*it);
				}
			}

		}
	}
}