#pragma once

namespace zcollumns
{
	class Game : public game::dynamic_object
	{
	public:
		Game(){}

		render::sprite_manager& getSpriteRenderer() {return m_sprite_renderer;}

	private:
		void update(float dt){}

	private:
		render::sprite_manager m_sprite_renderer;
	};
}