/**--------------------------------------------------------------------------------------
RGDEngine
----------------------------------------------------------------------------------------
\author Andrew "Zlobnik" Chouprina, 2005
mail:		dxprg@mail.ru
Created:	8 june 2005
---------------------------------------------------------------------------------------*/

#include "sprites.h"


namespace rgde
{
	namespace examples
	{
		namespace sprites
		{
			sprite_example::sprite_example()
				: m_priorities (1)
				, m_num_sprites_per_priority (1000)
				, m_enable_debug_log (false)
				, m_do_sort_before_log (true)
				, m_sprites_seed (math::vec2f (770, 570))
				, m_esc("Quit")
			{
				{
					using namespace input;

					Input::get_control(device::keyboard, KeyEscape)->bind("Quit");
					m_esc += boost::bind(&sprite_example::onEsc, this);
				}

				m_font = render::font::create(12, L"Arial", render::font::Heavy);

				using render::texture;

				std::vector<render::texture_ptr> textures;
				textures.push_back(texture::create( "Sprites/test01.jpg" ));
				textures.push_back(texture::create( "Sprites/test02.jpg" ));
				textures.push_back(texture::create( "Sprites/test03.jpg" ));
				textures.push_back(texture::create( "Sprites/test04.jpg" ));

				const math::Rect rect(0, 0, 1, 1);

				// creating sprites in order of worst texture distribution
				// note: total number of sprites = m_priorities * textures.size()
				for( unsigned priority = m_priorities; priority > 0; --priority )
					for( unsigned sprite = m_num_sprites_per_priority; sprite > 0; --sprite )
					{
						float x = m_sprites_seed[0];
						float y = m_sprites_seed[1];

						using math::unitRandom;
						using math::rangeRandom;

						for (uint i = 0; i < textures.size(); ++i)
						{
							math::vec2f size(rangeRandom(0,60),rangeRandom(0,40));
							math::vec2f pos(rangeRandom(0, x), rangeRandom(0, y));
							math::Color color = math::White;
							color.set(unitRandom(),	unitRandom(), unitRandom(),	unitRandom());

							float spin = rangeRandom (0, math::TWO_PI);

							render::sprite sprite(pos, size, color, textures[i], spin, rect, priority);
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
				static bool drawen = false;

				float time = game::game_system::get().get_timer().get_absolute_time();
				render::render_device::get().draw_fps(time, m_font);

				//if (drawen)
				//	return;

				typedef render::sprite_manager::sprites_iter iter;
				const float delta_spin = math::PI / 180.0f;

				for (iter it = m_sprites.begin(); it != m_sprites.end(); ++it)
				{
					it->spin += delta_spin;
					m_sprite_renderer.add_sprite(*it);
				}

				drawen = true;
			}
		}
	}
}