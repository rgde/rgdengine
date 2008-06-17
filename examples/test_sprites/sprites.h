#pragma once
#include <rgde/engine.h>

namespace rgde
{
	namespace exapmles
	{
		namespace sprites
		{
			class SpriteExample : public game::IDynamicObject
			{
			public:
				SpriteExample();

			protected:
				void onEsc ();
				void update (float dt);

			protected:
				render::SpriteManager			m_sprite_renderer;
				render::SpriteManager::SpritesVector m_sprites;
				input::KeyDown                  m_cEsc;
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
				::render::PFont					m_font;
				math::PCamera					m_camera;
			};
		}
	}
}
