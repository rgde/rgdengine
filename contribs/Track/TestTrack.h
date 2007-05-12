//TestTrack.h
#pragma once
#include "input/input.h"
#include "input/helper.h"
#include "math/mathTargetCamera.h"
#include "math/mathSplines.h"
#include "math/mathTrack.h"

class TestTrack: public game::IDynamicObject
{
public:
    TestTrack();
    ~TestTrack();

protected:
    void initInput();
    void initCamera();
    void initLights();

    void update (float dt);

    void onEsc   ();
    void onReset ();
    void onXAxis (int dx);
    void onYAxis (int dy);

private:
    render::PModel m_spModel;  //модель
    render::PFont  m_spFont;   //шрифт
    math::PCamera  m_spCamera; //камера

    math::CTrack m_track; //трек

    //контролы ввода
    input::KeyUp        m_cEsc;   //выход
    input::KeyDown      m_cR;     //сбросить программу в начальное состояние
    input::Button       m_cQ;     //поворот против часовой
    input::Button       m_cE;     //поворот по часовой
    input::Button       m_cW;     //приблизить
    input::Button       m_cS;     //удалить
    input::RelativeAxis m_cXAxis; //горизонтальная ось поворота
    input::RelativeAxis m_cYAxis; //вертикальная ось поворота

    //контроллер камеры
    math::PTargetCamera m_spTargetCamera; //нацеленная камера
};