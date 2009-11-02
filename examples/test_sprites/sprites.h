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
				render::sprite_manager::SpritesVector m_sprites;
				input::key_down                  m_cEsc;
				// Число приоритетов спрайтов
				unsigned						m_priorities;
				// Число спрайтов с одинаковым приоритетом
				unsigned						m_nSpritesPerPriority;
				// Стоит ли выводить отладочный лог
				bool							m_bDebugLog;
				// Стоит ли сортировать спрайты перед логом
				bool							m_bSortSpritesBeforeLog;
				// Область разброса спрайтов
				math::Vec2f						m_sprites_seed;
				::render::font_ptr					m_font;
				math::camera_ptr					m_camera;
			};
		}
	}
}
