//Space.h
#pragma once
#include <engine.h>

class Space
{
	struct StarSprite: public render::SSprite
	{
		void init(render::PTexture t);
		void init();
		math::Vec2f velocity;
	};
public:
    Space(int nStars);

    void update(float dt);
    void setPositionAndSpeed(float x, float y, float speed);
    void firePrimaryWeapon(bool bFire);
    void fireSecondaryWeapon();

private:    
    void initSprite(StarSprite& s);
    void updateSprite(StarSprite& s, float dt);
    bool isInsideScreen(const StarSprite& s);

private:

	typedef std::vector<StarSprite> Stars;


	Stars m_stars;

	float m_x;
	float m_y;
	float m_speed;

	render::PFont          m_font;
	render::PTexture       m_textureShip;
	render::PTexture       m_textureStar;
	render::SSprite        m_spriteShip;
	render::CSpriteManager m_sprite_renderer;
};