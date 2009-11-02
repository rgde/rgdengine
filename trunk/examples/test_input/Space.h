//Space.h
#pragma once
#include <rgde/engine.h>

class Space
{
	struct ParticleSprite: public render::sprite
	{
        void initParticle(render::texture_ptr &texture);
		void initStar(render::texture_ptr &texture);
        void initAlien(render::texture_ptr &texture);

        void initParticle();
		void initStar();
		void initAlien();

		math::Vec2f velocity;

        //только для чужих
        float damage;
        float hide;
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

	render::font_ptr          m_font;
	render::texture_ptr       m_textureShip;
	render::texture_ptr       m_textureParticle;
	render::texture_ptr       m_textureLazer;
	render::texture_ptr       m_textureStars;
    render::texture_ptr       m_textureAlien;
	render::sprite        m_spriteShip;
	render::sprite        m_spriteLazer;
	render::sprite        m_spriteStars;
	render::SpriteManager m_sprite_renderer;
};