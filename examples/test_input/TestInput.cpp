//TestInput.cpp
#include <rgde/engine.h>
#include "TestInput.h"

TestInput::TestInput():
    m_space            (200),
    m_key_quit        (L"Quit"),
    m_btnLeft          (L"Left"),
    m_btnRight         (L"Right"),
    m_btnUp            (L"Up"),
    m_btnDown          (L"Down"),
    m_btnSpeedUp       (L"SpeedUp"),
    m_btnSpeedDown     (L"SpeedDown"),
    m_trigPause        (L"Pause"),
    m_raxisHorz        (L"Horz"),
    m_raxisVert        (L"Vert"),
    m_btnPrimaryFire   (L"PrimaryFire"),
    m_btnSecondaryFire (L"SecondaryFire")
{
    render::render_device::get().setClearColor(math::Color(0,0,0,255));

    input::Input::LoadFromFile("TestInput/input.xml");

    //биндим функции для перехвата событий
    m_key_quit        += boost::bind(&TestInput::onQuit,          this);
    m_trigPause        += boost::bind(&TestInput::onPause,         this, _1);
    m_raxisHorz        += boost::bind(&TestInput::onHorz,          this, _1);
    m_raxisVert        += boost::bind(&TestInput::onVert,          this, _1);
    m_btnPrimaryFire   += boost::bind(&TestInput::onPrimaryFire,   this, _1);
    m_btnSecondaryFire += boost::bind(&TestInput::onSecondaryFire, this, _1);

    //настроим "игру"
    m_x = 100.f;
    m_y = 100.f;
    m_speed = 5.f;
    m_space.setPositionAndSpeed(m_x,m_y,m_speed);

    //->
    {
	    using namespace math;
        m_cursor.texture = render::texture::create("TestInput/cursor.png");
	    m_cursor.priority = (uint)1000;
	    m_cursor.size = vec2f(32, 32);
	    m_cursor.color = Color(255, 255, 255, 255);
	    m_cursor.rect = Rect(0, 0, 1, 1);
	    m_cursor.pos = vec2f(m_mouse.getX(), m_mouse.getY());
	    m_cursor.spin = 0;
    }
    //-<
}

void TestInput::update (float dt)
{
    const float deltaspeed = 10.f;
    const float movespeed  = 300.f;

    if (m_trigPause)
    {
        m_space.setPositionAndSpeed(m_x,m_y,0);
        //->
        if (m_mouse.isLeftPressed())
            m_cursor.color = math::Color(255,0,0,128);
        else
            m_cursor.color = math::Color(0,255,0,255);

        if (m_mouse.isRightPressed())
        {
            m_cursor.color = math::Color(0,0,255,128);
            //...
        }
        else
        {
            //...
        }
		m_cursor.pos = math::vec2f(m_mouse.getX(), m_mouse.getY());
        m_sprite_renderer.add_sprite(m_cursor);
        //-<
    }
    else
    {
        //изменение скорости
        if (m_btnSpeedUp)   m_speed += dt * deltaspeed;
        if (m_btnSpeedDown) m_speed -= dt * deltaspeed;
        if (m_speed < 0)    m_speed = 0;

        //перемещение корабля
        if (m_btnUp)    m_y -= dt * movespeed;
        if (m_btnDown)  m_y += dt * movespeed;
        if (m_btnLeft)  m_x -= dt * movespeed;
        if (m_btnRight) m_x += dt * movespeed;

        m_space.setPositionAndSpeed(m_x,m_y,m_speed);
    }


    m_space.update(dt);
}

void TestInput::onQuit ()
{
    core::application::get()->close();
}

void TestInput::onPause (bool pause)
{
    if (pause)
    {
        input::Input::get().set_mode(false);
        m_mouse.set_pos(400,300);
        ShowCursor(FALSE);
    }
    else
    {
        ShowCursor(TRUE);
        input::Input::get().set_mode(true);
    }
}

void TestInput::onHorz (int dx)
{
    const float deltaspeed = 1.3f;

    if (!m_trigPause)
        m_x += dx * deltaspeed;
}

void TestInput::onVert (int dy)
{
    const float deltaspeed = 1.3f;

    if (!m_trigPause)
        m_y += dy * deltaspeed;
}

void TestInput::onPrimaryFire (bool press)
{
    if (!m_trigPause)
        m_space.firePrimaryWeapon(press);
}

void TestInput::onSecondaryFire (bool press)
{
    if (!m_trigPause && press)
        m_space.fireSecondaryWeapon();
}
