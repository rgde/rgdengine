//Space.cpp
#include "Space.h"

void Space::StarSprite::init(render::PTexture t)
{
	pTexture = t;
	init();
}

void Space::StarSprite::init()
{
	using namespace math;
	float s = rangeRandom(2.0f, 8.0f);
	uPriority = (uint)s;
	size = Vec2f(s, s);
	uint c = (uint)(255*s/8.0f);
	color = Color(255, 255, 255, c);
	rect = Rect(0, 0, 1, 1);
	pos = Vec2f(rangeRandom(0,800.0f), rangeRandom(0,620.0f));
	spin = 0;

	velocity = Vec2f(0.f, 10.f)*s;
}

Space::Space (int nStars)
{
    m_x = 0;
    m_y = 0;

    m_font = render::IFont::Create(12, L"Arial", render::IFont::Heavy);

    //кораблик
    m_textureShip = render::ITexture::Create("TestInput/SpaceShip.png"); 

    m_spriteShip.pTexture = m_textureShip;
    m_spriteShip.size = math::Vec2f(m_textureShip->getWidth(),m_textureShip->getHeight());
    m_spriteShip.pos = math::Vec2f(m_x,m_y);
    m_spriteShip.rect = math::Rect(0, 0, 1, 1);
    m_spriteShip.color = math::Color(255, 255, 255, 255);
    m_spriteShip.spin = 0;

    //звезды    
	m_textureStar = render::ITexture::Create("TestInput/Star.png");
    m_stars.resize(nStars);

    std::for_each(m_stars.begin(), m_stars.end(), 
        boost::bind(&Space::StarSprite::init, _1, m_textureStar));
}

void Space::update (float dt)
{
    //‘ыѕыЁс
    render::TheDevice::Get().showFPS(m_font);

    //кораблик
    m_spriteShip.pos = math::Vec2f(int(m_x)+.5f,int(m_y)+.5f);
    m_sprite_renderer.addSprite(m_spriteShip);

    //звезды
    std::for_each(m_stars.begin(), m_stars.end(), 
        boost::bind(&Space::updateSprite, this, _1, dt));

    std::for_each(m_stars.begin(), m_stars.end(), 
        boost::bind(&render::CSpriteManager::addSprite, &m_sprite_renderer, _1));
}

void Space::setPositionAndSpeed (float x, float y, float speed)
{
    m_x     = x;
    m_y     = y;
    m_speed = speed;
}

void Space::firePrimaryWeapon (bool bFire)
{
    //...
}

void Space::fireSecondaryWeapon ()
{
    //...
}

void Space::initSprite(StarSprite& s)
{
    s.init();
    s.pos = math::Vec2f(math::rangeRandom (0, 800.0f), m_speed>0 ? -s.size[1]*2 : 600.0f+s.size[1]*2  );
}

void Space::updateSprite(StarSprite& s, float dt)
{
    if (isInsideScreen(s))
    {
        initSprite(s);
        return;
    }

    s.pos += s.velocity*dt*m_speed;
}

bool Space::isInsideScreen(const StarSprite& s)
{
    const float offset = 40.f;
    return s.pos[0] - s.size[0] > 800 + offset ||
           s.pos[1] - s.size[1] > 600 + offset ||
           s.pos[0] - s.size[0] < -offset      ||
           s.pos[1] - s.size[1] < -offset;
}
