#pragma once

#include <rgde/input/input.h>

#include "Space.h"

class TestInput: public game::dynamic_object
{
public:
    TestInput();

protected:
    void update (float dt);

private:
    void onQuit  ();
    void onPause (bool pause);
    void onHorz  (int dx);
    void onVert  (int dy);
    void onPrimaryFire   (bool press);
    void onSecondaryFire (bool press);

    input::key_up       m_key_quit;
    input::Button       m_btnLeft;
    input::Button       m_btnRight;
    input::Button       m_btnUp;
    input::Button       m_btnDown;
    input::Button       m_btnSpeedUp;
    input::Button       m_btnSpeedDown;
    input::Button       m_btnPrimaryFire;
    input::Button       m_btnSecondaryFire;
    input::Trigger      m_trigPause;
    input::RelativeAxis m_raxisHorz;
    input::RelativeAxis m_raxisVert;

    float m_x;
    float m_y;
    float m_speed;
    Space m_space;

    //->
    input::mouse           m_mouse;
    render::sprite        m_cursor;
	render::sprite_manager m_sprite_renderer;
    //-<
};
