//TestInput.h
#pragma once
#include "input/input.h"
#include "input/helper.h"

class TestInput: public game::IDynamicObject
{
public:
    TestInput();

protected:
    void update (float dt);

private:
    void onQuit ();
    void onHorz (int dx);
    void onVert (int dy);

    input::CKeyUp        m_keyupQuit;
    input::CButton       m_btnLeft;
    input::CButton       m_btnRight;
    input::CButton       m_btnUp;
    input::CButton       m_btnDown;
    input::CButton       m_btnSpeedUp;
    input::CButton       m_btnSpeedDown;
    input::CTrigger      m_trigPause;
    input::CRelativeAxis m_raxisHorz;
    input::CRelativeAxis m_raxisVert;
};
