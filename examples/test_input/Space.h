//Space.h
#pragma once
#include <engine.h>

class Space
{
public:
    Space (int nStars);

    void update (float dt);
    void setPositionAndSpeed (float x, float y, float speed);
    void firePrimaryWeapon (bool bFire);
    void fireSecondaryWeapon ();

private:    
    float m_x,
          m_y,
          m_speed;

    render::PFont          m_font;
    render::PTexture       m_textureShip;
    render::PTexture       m_textureStar;
    render::SSprite        m_spriteShip;
    render::CSpriteManager m_sprite_renderer;

	struct StarSprite: public render::SSprite
	{
		void init();
		math::Vec2f velocity;
	};
    typedef std::vector<StarSprite> Stars;
    Stars m_stars;

    void initSprite(StarSprite& s);
    void updateSprite(StarSprite& s, float dt);
    bool isInsideScreen(const StarSprite& s);

    //...
};