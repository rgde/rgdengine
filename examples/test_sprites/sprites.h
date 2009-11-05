#pragma once
#include <rgde/engine.h>

namespace rgde
{
	namespace examples
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
				typedef render::sprite_manager::sprites_vector sprites_vector;
				
				render::sprite_manager		m_sprite_renderer;				
				sprites_vector				m_sprites;

				input::key_down             m_esc;
				// num of different sprites priority
				unsigned					m_priorities;
				// num of sprites with equal priority
				unsigned					m_num_sprites_per_priority;
				// do we need debug logging? 
				bool						m_enable_debug_log;
				// do we need sort sprites before debug log output?
				bool						m_do_sort_before_log;
				// sprite size variation
				math::vec2f					m_sprites_seed;
				render::font_ptr			m_font;
				math::camera_ptr			m_camera;
			};
		}
	}
}
