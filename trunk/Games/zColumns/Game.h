#pragma once

namespace zcollumns
{
	class Game : public game::IDynamicObject
	{
	public:
		Game(){}

		render::CSpriteManager& getSpriteRenderer() {return m_sprite_renderer;}

	private:
		void update(float dt){}

	private:
		render::CSpriteManager m_sprite_renderer;
	};
}