#include "glass.h"

namespace game
{
	Brick::Brick()
		: id(-1), managed(false), falling(false)
	{
	}

	Glass::Glass(const math::Rect& r, float brickSize)
		: m_brickSize(brickSize), m_rect(r)
	{	
		loadTextures();	
		initField((uint)(r.w/brickSize), (uint)(r.h/brickSize));		
	}

	void Glass::loadTextures()
	{
		m_textures.resize(3);

		m_textures[0] = render::texture::create("ZColumns/Circle.jpg");
		m_textures[1] = render::texture::create("ZColumns/Rect.jpg");
		m_textures[2] = render::texture::create("ZColumns/Triangle.jpg");
	}
	
	void Glass::initField(uint w, uint h)
	{
		math::Vec2f c = m_rect.getBottomLeft();
		float size = m_brickSize;

		m_field.resize(w);
		for (uint i = 0; i < w; ++i)
		{
			Line& l = m_field[i];
			l.resize(h);

			for(uint j = 0; j < h; ++j)
			{
				Brick& b = l[j];
				b.x = i;
				b.y = j;
				//b.id = j % m_textures.size();
				b.id = h-1 == j ? i % m_textures.size() : -1;
				b.rect = math::Rect(c[0] + i*size, c[1]-j*size, size, size);
			}
		}

		m_field[5][0].id = 1;
		m_field[5][1].id = 1;
		m_field[5][2].id = 1;
		m_field[5][3].id = 1;
	}

	math::Vec2i Glass::recalcPos(const Brick& b)
	{
		math::Vec2f c = m_rect.getBottomLeft();
		float size = m_brickSize;
		uint x = (b.rect.position[0] - c[0])/size;
		uint y = (c[1] - b.rect.position[1])/size;
		return math::Vec2i(x,y);
		//b.rect = math::Rect(c[0] + x*size, c[1]-y*size, size, size);
	}

	void Glass::fixPos(Brick& b)
	{
		math::Vec2i pos = recalcPos(b);
		math::Vec2f c = m_rect.getBottomLeft();
		float size = m_brickSize;
		b.rect.position = math::Vec2f(c[0] + pos[0]*size, c[1]-pos[1]*size);
	}

	bool Glass::isCanMove(const Brick& b, MoveDirection dir)
	{
		math::Vec2f c = m_rect.getBottomLeft();
		uint x = (b.rect.position[0] - c[0])/m_brickSize;
		uint y = (c[1] - b.rect.position[1])/m_brickSize;

		switch(dir)
		{
		case Left:
			x--;
			return x >= 0 ? (-1 == m_field[x][y].id) : false;
			break;

		case Down:
			y--;
			return y >= 0 && y < m_field[0].size()? (-1 == m_field[x][y].id) : false;
			break;

		case Right:
			x++;
			return x <  m_field.size() ? (-1 == m_field[x][y].id) : false;
			break;
		};

		return false;
	}

	void updateBrick(Brick& b, float dt)
	{
		b.rect.position[1] += dt*100;
	}

	void Glass::update(float dt)
	{
		checkBricks();
		moveDown(dt);
		renderSprites();
	}

	void Glass::checkBricks()
	{
		for (uint i = 0; i < m_field.size(); ++i)
		{
			Line& l = m_field[i];
			for(uint j = 0; j < l.size(); ++j)
			{
				Brick& b = l[j]; 
				if (-1 != b.id)
				{
					b.falling = !b.falling && isCanMove(b, Down);					
				}
			}
		}
	}

	void Glass::moveDown(float dt)
	{
		for (uint i = 0; i < m_field.size(); ++i)
		{
			Line& l = m_field[i];
			for(uint j = 0; j < l.size(); ++j)
			{
				Brick& b = l[j]; 
				if (-1 != b.id)
				{
					if (b.falling)
						updateBrick(b, dt);
				}
			}
		}

	}

	void Glass::renderSprites()
	{
		render::Sprite sprite;
		sprite.color = 0xFFFFFFFF;
		sprite.rect = math::Rect(0,0,1,1);
		sprite.spin = 0;
		sprite.uPriority = 0;
		sprite.size = math::Vec2f(m_brickSize, m_brickSize);

		for (uint i = 0; i < m_field.size(); ++i)
		{
			Line& l = m_field[i];
			for(uint j = 0; j < l.size(); ++j)
			{
				if (-1 != l[j].id)
				{
					sprite.texture = m_textures[l[j].id];
					sprite.pos = l[j].rect.position;
					m_spriteRenderer.addSprite(sprite);
				}
			}
		}
	}
}