//Space.cpp
#include "Space.h"

void Space::ParticleSprite::initParticle(render::PTexture &texture)
{
    pTexture = texture;
    initParticle();
}

void Space::ParticleSprite::initStar(render::PTexture &texture)
{
    pTexture = texture;
    initStar();
}

void Space::ParticleSprite::initParticle()
{
	using namespace math;
	float s = rangeRandom(2.0f, 4.0f);
	uPriority = (uint)s;
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

	uPriority = (uint).1f;
	size = Vec2f(128.f, 128.f);
	color = Color(255*c, 255*c, 255*c, 255);
	rect = Rect(0, 0, 1, 1);
	pos = Vec2f(rangeRandom(0.f,800.f), rangeRandom(-128.f*2,600.0f+128.f));
	spin = 0;

	velocity = Vec2f(0.f, 20.f*c);
}

Space::Space (int nStars)
{
    m_x = 0;
    m_y = 0;
    m_speed = 0;
    m_bFirePrimaryWeapon = false;

    m_font = render::IFont::Create(12, L"Arial", render::IFont::Heavy);

    //кораблик
    m_textureShip = render::ITexture::Create("TestInput/SpaceShip.png"); 

    m_spriteShip.pTexture = m_textureShip;
    m_spriteShip.size = math::Vec2f(m_textureShip->getWidth(),m_textureShip->getHeight());
    m_spriteShip.pos = math::Vec2f(m_x,m_y);
    m_spriteShip.rect = math::Rect(0, 0, 1, 1);
    m_spriteShip.color = math::Color(255, 255, 255, 255);
    m_spriteShip.spin = 0;

    //пыль
	m_textureParticle = render::ITexture::Create("TestInput/Particle.png");
    m_particles.resize(nStars/4);

    std::for_each(m_particles.begin(), m_particles.end(), 
        boost::bind(&Space::ParticleSprite::initParticle, _1, m_textureParticle));

    //звезды
	m_textureStars = render::ITexture::Create("TestInput/Stars.png");
    m_stars.resize(nStars/2);

    std::for_each(m_stars.begin(), m_stars.end(), 
        boost::bind(&Space::ParticleSprite::initStar, _1, m_textureStars));

    //лазер
    m_textureLazer = render::ITexture::Create("TestInput/Lazer.png"); 

    m_spriteLazer.pTexture = m_textureLazer;
    m_spriteLazer.size = math::Vec2f(m_textureLazer->getWidth()*2,m_textureLazer->getHeight());
    m_spriteLazer.pos = math::Vec2f(m_x,m_y);
    m_spriteLazer.rect = math::Rect(0, 0, 1, 1);
    m_spriteLazer.color = math::Color(255, 255, 255, 255);
    m_spriteLazer.spin = 0;
}

void Space::update (float dt)
{
    //‘ыѕыЁс
    render::TheDevice::Get().showFPS(m_font);

    //кораблик
    m_spriteShip.pos = math::Vec2f(int(m_x)+.5f,int(m_y)+.5f);
    m_sprite_renderer.addSprite(m_spriteShip);

    //пыль
    std::for_each(m_particles.begin(), m_particles.end(), 
        boost::bind(&Space::updateSprite, this, _1, dt));

    std::for_each(m_particles.begin(), m_particles.end(), 
        boost::bind(&render::CSpriteManager::addSprite, &m_sprite_renderer, _1));

    //звезды
    std::for_each(m_stars.begin(), m_stars.end(), 
        boost::bind(&Space::updateStar, this, _1, dt));

    std::for_each(m_stars.begin(), m_stars.end(), 
        boost::bind(&render::CSpriteManager::addSprite, &m_sprite_renderer, _1));

    //primary weapon
    if (m_bFirePrimaryWeapon)
    {
        float x = int(m_x) - .5f;
        float y = int(m_y) - 34.5f;

        float h = m_textureLazer->getHeight();
        for (; y>-h/2; y -= h)
        {
            m_spriteLazer.rect = math::Rect(0, math::rangeRandom(0,.2f), 1, 1-math::rangeRandom(0,.2f));
            m_spriteLazer.pos = math::Vec2f(x,y);
            m_sprite_renderer.addSprite(m_spriteLazer);
        }
    }
}

void Space::setPositionAndSpeed (float x, float y, float speed)
{
    m_x     = x;
    m_y     = y;
    m_speed = speed;
}

void Space::firePrimaryWeapon (bool bFire)
{
    m_bFirePrimaryWeapon = bFire;
}

void Space::fireSecondaryWeapon ()
{
    //...
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

void Space::updateSprite(ParticleSprite& s, float dt)
{
    if (isInsideScreen(s))
    {
        initSprite(s);
        return;
    }

    s.pos += s.velocity*dt*m_speed;
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

bool Space::isInsideScreen(const ParticleSprite& s)
{
    const float offset = s.size[1]*4;
    return s.pos[1] - s.size[1] > 600 + offset ||
           s.pos[1] + s.size[1] <     - offset;
}
