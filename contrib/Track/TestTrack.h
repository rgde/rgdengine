//TestTrack.h
#pragma once
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

    void onEsc   (const input::CButtonEvent &event);
    void onReset (const input::CButtonEvent &event);
    void onXAxis (const input::CRelativeAxisEvent &event);
    void onYAxis (const input::CRelativeAxisEvent &event);

private:
    render::PModel m_spModel;  //модель
    render::PFont  m_spFont;   //шрифт
    math::PCamera  m_spCamera; //камера

    math::CTrack m_track; //трек

    //контролы ввода
    input::CButtonCommand       m_cEsc;   //выход
    input::CButtonCommand       m_cR;     //сбросить программу в начальное состояние
    input::CButtonCommand       m_cQ;     //поворот против часовой
    input::CButtonCommand       m_cE;     //поворот по часовой
    input::CButtonCommand       m_cW;     //приблизить
    input::CButtonCommand       m_cS;     //удалить
    input::CRelativeAxisCommand m_cXAxis; //горизонтальная ось поворота
    input::CRelativeAxisCommand m_cYAxis; //вертикальная ось поворота

    //контроллер камеры
    math::PTargetCamera m_spTargetCamera; //нацеленная камера
};