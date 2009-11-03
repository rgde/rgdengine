//Space.cpp
#include "Space.h"

void Space::ParticleSprite::initParticle(render::texture_ptr &t)
{
    texture = t;
    initParticle();
}

void Space::ParticleSprite::initStar(render::texture_ptr &t)
{
    texture = t;
    initStar();
}

void Space::ParticleSprite::initAlien(render::texture_ptr &t)
{
    texture = t;
    initAlien();
}

void Space::ParticleSprite::initParticle()
{
	using namespace math;
	float s = rangeRandom(2.0f, 4.0f);
	priority = (uint)s;
	size = Vec2f(s, s*2);
	uint c = (uint)(255*s/4.0f);
	color = Color(105, 105, 105, c);
	rect = Rect(0, 0, 1, 1);
	pos = Vec2f(rangeRandom(0,800.0f), rangeRandom(0,620.0f));
	spin = 0;

	velocity = Vec2f(0.f, 20.f)*s;
}

void Space::ParticleSprite::initStar()
{
	using namespace math;

	float c = rangeRandom(1.0f, 5.0f) / 4.0f;

	priority = (uint).1f;
	size = Vec2f(128.f, 128.f);
	color = Color(255*c, 255*c, 255*c, 255);
	rect = Rect(0, 0, 1, 1);
	pos = Vec2f(rangeRandom(0.f,800.f), rangeRandom(-128.f*2,600.0f+128.f));
	spin = rangeRandom(0.f,360.f);

	velocity = Vec2f(0.f, 20.f*c);
}

void Space::ParticleSprite::initAlien()
{
	using namespace math;

	float c = rangeRandom(1.0f, 5.0f) / 4.0f;

	priority = (uint).11f;

	float sx = rangeRandom(46.0f, 87.0f);
	float sy = rangeRandom(46.0f, 87.0f);
	size = Vec2f(sx, sy);

	color = Color(255, 255, 255, 255);
	rect = Rect(0, 0, 1, 1);
	pos = Vec2f(rangeRandom(0.f,800.f), rangeRandom(-128.f*2,600.0f+128.f));
	spin = rangeRandom(0.f,360.f);

	velocity = Vec2f(0.f, 20.f*c);
    damage = 1.f;
    hide = 0.f;
}

Space::Space (int nStars)
{
    m_x = 0;
    m_y = 0;
    m_speed = 0;
    m_bFirePrimaryWeapon = false;

    m_font = render::font::create(12, L"Arial", render::font::Heavy);

    //кораблик
    m_textureShip = render::texture::create("TestInput/SpaceShip.png"); 

    m_spriteShip.texture = m_textureShip;
    m_spriteShip.size = math::Vec2f(m_textureShip->get_width(),m_textureShip->get_height());
    m_spriteShip.pos = math::Vec2f(m_x,m_y);
    m_spriteShip.rect = math::Rect(0, 0, 1, 1);
    m_spriteShip.color = math::Color(255, 255, 255, 255);
    m_spriteShip.spin = 0;

    //пыль
	m_textureParticle = render::texture::create("TestInput/particle.png");
    m_particles.resize(nStars/4);

    std::for_each(m_particles.begin(), m_particles.end(), 
        boost::bind(&Space::ParticleSprite::initParticle, _1, m_textureParticle));

    //звезды
	m_textureStars = render::texture::create("TestInput/Stars.png");
    m_stars.resize(nStars/2);

    std::for_each(m_stars.begin(), m_stars.end(), 
        boost::bind(&Space::ParticleSprite::initStar, _1, m_textureStars));

    //чужие
	m_textureAlien = render::texture::create("TestInput/Alien.png");
    m_aliens.resize(nStars/10);

    std::for_each(m_aliens.begin(), m_aliens.end(), 
        boost::bind(&Space::ParticleSprite::initAlien, _1, m_textureAlien));

    //лазер
    m_textureLazer = render::texture::create("TestInput/Lazer.png"); 

    m_spriteLazer.texture = m_textureLazer;
    m_spriteLazer.size = math::Vec2f(m_textureLazer->get_width()*2,m_textureLazer->get_height());
    m_spriteLazer.pos = math::Vec2f(m_x,m_y);
    m_spriteLazer.rect = math::Rect(0, 0, 1, 1);
    m_spriteLazer.color = math::Color(255, 255, 255, 255);
    m_spriteLazer.spin = 0;
}

void Space::update (float dt)
{
	//FPS
	render::TheDevice::get().showFPS(m_font);

    //ship
    m_spriteShip.pos = math::Vec2f(int(m_x)+.5f,int(m_y)+.5f);
    m_sprite_renderer.add_sprite(m_spriteShip);

    //dust
    std::for_each(m_particles.begin(), m_particles.end(), 
        boost::bind(&Space::updateSprite, this, _1, dt));

    std::for_each(m_particles.begin(), m_particles.end(), 
        boost::bind(&render::sprite_manager::add_sprite, &m_sprite_renderer, _1));

    //stars
    std::for_each(m_stars.begin(), m_stars.end(), 
        boost::bind(&Space::updateStar, this, _1, dt));

    std::for_each(m_stars.begin(), m_stars.end(), 
        boost::bind(&render::sprite_manager::add_sprite, &m_sprite_renderer, _1));

    //aliens
    std::for_each(m_aliens.begin(), m_aliens.end(), 
        boost::bind(&Space::updateAlien, this, _1, dt));

    std::for_each(m_aliens.begin(), m_aliens.end(), 
        boost::bind(&render::sprite_manager::add_sprite, &m_sprite_renderer, _1));

    //primary weapon
    if (m_bFirePrimaryWeapon)
    {
        float x = int(m_x) - .5f;
        float y = int(m_y) - 34.5f;

        float h = m_textureLazer->get_height();
        for (; y>-h/2; y -= h)
        {
            m_spriteLazer.rect = math::Rect(0, math::rangeRandom(0,.2f), 1, 1-math::rangeRandom(0,.2f));
            m_spriteLazer.pos = math::Vec2f(x,y);
            m_sprite_renderer.add_sprite(m_spriteLazer);
        }
    }
}

void Space::setPositionAndSpeed (float x, float y, float speed)
{
    m_x     = x;
    m_y     = y;
    m_speed = speed < 0 ? 0 : speed;
}

void Space::firePrimaryWeapon (bool bFire)
{
    m_bFirePrimaryWeapon = bFire;
}

void Space::fireSecondaryWeapon ()
{
    //...
	//if (s.damage == 0.f && s.hide >= 0)
	for (size_t i = 0; i < m_aliens.size(); ++i)
	{
		m_aliens[i].damage = 0;
		m_aliens[i].hide = 0;
	}
}

void Space::initSprite(ParticleSprite& s)
{
    s.initParticle();
    s.pos = math::Vec2f(math::rangeRandom (0, 800.0f), m_speed>0 ? -s.size[1]*2 : 600.0f+s.size[1]*2  );
}

void Space::initStar(ParticleSprite& s)
{
    s.initStar();
    s.pos = math::Vec2f(math::rangeRandom (0, 800.0f), m_speed>0 ? -s.size[1]*2 : 600.0f+s.size[1]*2  );
}

void Space::initAlien(ParticleSprite& s)
{
    s.initAlien();
    s.pos = math::Vec2f(math::rangeRandom (0, 800.0f), m_speed>0 ? -s.size[1]*2 : 600.0f+s.size[1]*2  );
}

void Space::updateSprite(ParticleSprite& s, float dt)
{
    if (isInsideScreen(s))
    {
        initSprite(s);
        return;
    }

    s.pos += s.velocity*dt*m_speed;
    float size = s.size[0]*m_speed;
    s.size[1] = math::abs(size) < 4 ? 4 : size;
}

void Space::updateStar(ParticleSprite& s, float dt)
{
    if (isInsideScreen(s))
    {
        initStar(s);
        return;
    }

    s.pos += s.velocity*dt*m_speed;
}

void Space::updateAlien(ParticleSprite& s, float dt)
{
    if (isInsideScreen(s))
    {
        initAlien(s);
        return;
    }

    //сдвигаем вниз
    s.pos += s.velocity*dt*m_speed;

    //дамажим
    if (m_bFirePrimaryWeapon)
    {
        if (math::abs(s.pos[0]-m_x) < s.size[0]/2.f && s.damage > 0 && s.pos[1] < m_y)
        {
            s.damage -= 2.f * dt;
            if (s.damage <= 0)
            {
                s.damage = 0;
                s.hide = 1.f;
            }
            s.color = math::Color(255.f, 255.f*s.damage, 255.f*s.damage, 255.f);
            s.pos[0] += math::rangeRandom(-5.f, 5.f)*(1-s.damage);
            s.spin += math::rangeRandom(-.1f,.1f);
        }
    }

    //таем
    if (s.damage == 0.f && s.hide >= 0)
    {
        s.hide -= 4.f * dt;
        if (s.hide <= 0)
        {
            initAlien(s);
            return;
        }
        s.color = math::Color(255.f, 0, 0, 255.f*s.hide);
        s.pos[0] += math::rangeRandom(-5.f, 5.f);
        s.spin += math::rangeRandom(-.1f,.1f);
    }

    //лечимся
    if (!m_bFirePrimaryWeapon || math::abs(s.pos[0]-m_x) > s.size[0]/2.f)
    {
        if (s.hide == 0 && s.damage < 1.f)
        {
            s.damage += .5f*dt;
            if (s.damage > 1.f)
                s.damage = 1.f;
            s.color = math::Color(255.f, 255.f*s.damage, 255.f*s.damage, 255.f);

            s.pos[0] += math::rangeRandom(-5.f, 5.f)*(1-s.damage);
            s.spin += math::rangeRandom(-.04f,.04f);
        }
    }
}

bool Space::isInsideScreen(const ParticleSprite& s)
{
    const float offset = s.size[1]*4;
    return s.pos[1] - s.size[1] > 600 + offset ||
           s.pos[1] + s.size[1] <     - offset;
}
