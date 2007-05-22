//TestInput.cpp
#include <rgde/engine.h>
#include "TestInput.h"

TestInput::TestInput():
    m_space            (200),
    m_keyupQuit        (L"Quit"),
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
    render::TheDevice::Get().setClearColor(math::Color(0,0,0,255));

    input::Input::LoadFromFile("TestInput/input.xml");

    //биндим функции для перехвата событий
    m_keyupQuit        += boost::bind(&TestInput::onQuit,          this);
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
}

void TestInput::update (float dt)
{
    const float deltaspeed = 10.f;
    const float movespeed  = 300.f;

    if (m_trigPause)
        m_space.setPositionAndSpeed(m_x,m_y,0);
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
    core::IApplication::Get()->close();
}

void TestInput::onPause (bool pause)
{
    if (pause)
    {
        input::Input::Get().SetMode(false);
    }
    else
    {
        input::Input::Get().SetMode(true);
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
    if (!m_trigPause)
        m_space.fireSecondaryWeapon();
}
