#pragma once
#include <rgde/engine.h>

namespace rgde
{
	namespace exapmles
	{
		namespace sprites
		{
			class sprite_example : public game::dynamic_object
			{
			public:
				sprite_example();

			protected:
				void onEsc ();
				void update (float dt);

			protected:
				render::sprite_manager			m_sprite_renderer;
				render::sprite_manager::sprites_vector m_sprites;
				input::key_down                  m_cEsc;
				// num of different sprites priority
				unsigned						m_priorities;
				// num of sprites with equal priority
				unsigned						m_num_sprites_per_priority;
				// do we need debug logging? 
				bool							m_bDebugLog;
				// do we need sort sprites before debug log output?
				bool							m_bSortSpritesBeforeLog;
				// sprite size variation
				math::Vec2f						m_sprites_seed;
				::render::font_ptr				m_font;
				math::camera_ptr				m_camera;
			};
		}
	}
}
