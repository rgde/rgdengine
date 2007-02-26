//Space.h
#pragma once
#include <engine.h>

class Space
{
	struct ParticleSprite: public render::SSprite
	{
        void initParticle(render::PTexture &texture);
		void initStar(render::PTexture &texture);
        void initAlien(render::PTexture &texture);

        void initParticle();
		void initStar();
		void initAlien();

		math::Vec2f velocity;

        //только для чужих
        float       damage;
        float       hide;
	};

public:
    Space(int nStars);

    void update(float dt);
    void setPositionAndSpeed(float x, float y, float speed);
    void firePrimaryWeapon(bool bFire);
    void fireSecondaryWeapon();

private:    
    void initSprite(ParticleSprite& s);
    void initStar(ParticleSprite& s);
    void initAlien(ParticleSprite& s);
    void updateSprite(ParticleSprite& s, float dt);
    void updateStar(ParticleSprite& s, float dt);
    void updateAlien(ParticleSprite& s, float dt);
    bool isInsideScreen(const ParticleSprite& s);

private:
	typedef std::vector<ParticleSprite> Particles;
	Particles m_particles;
    Particles m_stars;
    Particles m_aliens;

	float m_x;
	float m_y;
	float m_speed;
    bool  m_bFirePrimaryWeapon;

	render::PFont          m_font;
	render::PTexture       m_textureShip;
	render::PTexture       m_textureParticle;
	render::PTexture       m_textureLazer;
	render::PTexture       m_textureStars;
    render::PTexture       m_textureAlien;
	render::SSprite        m_spriteShip;
	render::SSprite        m_spriteLazer;
	render::SSprite        m_spriteStars;
	render::CSpriteManager m_sprite_renderer;
};