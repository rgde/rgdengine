#pragma once

namespace zcollumns
{
	class Game : public game::dynamic_object
	{
	public:
		Game(){}

		render::SpriteManager& getSpriteRenderer() {return m_sprite_renderer;}

	private:
		void update(float dt){}

	private:
		render::SpriteManager m_sprite_renderer;
	};
}