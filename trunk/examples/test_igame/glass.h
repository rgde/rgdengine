#pragma once

#include <rgde/engine.h>

namespace game
{
	struct Brick
	{
		Brick();
		int id;			//texture id, -1 for empty brick
		
		bool managed;	// managed by user input
		
		bool falling;	

		math::Rect rect;

		uint x, y;
	};

	class Glass  : public dynamic_object
	{
	public:
		typedef std::vector<Brick> Line;
		typedef std::vector<Line>  GameField;		

		Glass(const math::Rect& r, float brickSize = 10); // Rect for placement on window
		
	private:
		void initField(uint w, uint h);
		void loadTextures();
		void update(float dt);
		void renderSprites();
		void moveDown(float dt);
		void checkBricks();

		enum MoveDirection
		{
			Down, Left, Right
		};

		bool isCanMove(const Brick& b, MoveDirection dir);
		math::Vec2i recalcPos(const Brick& b);
		void fixPos(Brick& b);

	private:
		render::sprite_manager m_spriteRenderer;
		std::vector<render::texture_ptr> m_textures;
		GameField m_field;
		float m_brickSize;
		math::Rect m_rect;
	};
}