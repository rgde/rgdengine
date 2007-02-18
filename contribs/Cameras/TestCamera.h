//TestCamera.h
#pragma once
#include "math/mathFlyCamera.h"
#include "math/mathTargetCamera.h"
#include "math/mathFirstPersonCamera.h"

class TestCamera: public game::IDynamicObject
{
public:
    TestCamera();
    ~TestCamera();

protected:
    void initInput();
    void initCamera();
    void initLights();

    void update (float dt);

    void onEsc          (const input::CButtonEvent &event);
    void onReset        (const input::CButtonEvent &event);
    void onChangeCamera (const input::CButtonEvent &event);
    void onXAxis        (const input::CRelativeAxisEvent &event);
    void onYAxis        (const input::CRelativeAxisEvent &event);

private:
    render::PModel m_spModel;  //модель
    render::PFont  m_spFont;   //шрифт
    math::PCamera  m_spCamera; //камера

    int m_nCameraType; //номер используемой камеры

    //контролы ввода
    input::CButtonCommand       m_cEsc;   //выход
    input::CButtonCommand       m_cTab;   //переключить камеру
    input::CButtonCommand       m_cR;     //сбросить программу в начальное состо€ние
    input::CButtonCommand       m_cQ;     //поворот против часовой
    input::CButtonCommand       m_cW;     //вперед
    input::CButtonCommand       m_cE;     //поворот по часовой
    input::CButtonCommand       m_cA;     //боком влево
    input::CButtonCommand       m_cS;     //назад
    input::CButtonCommand       m_cD;     //боком вправо
    input::CButtonCommand       m_cF;     //вверх
    input::CButtonCommand       m_cC;     //вниз
    input::CRelativeAxisCommand m_cXAxis; //горизонтальна€ ось поворота
    input::CRelativeAxisCommand m_cYAxis; //вертикальна€ ось поворота

    //контроллеры камеры
    math::PFlyCamera         m_spFlyCamera;         //летающа€€ камера
    math::PTargetCamera      m_spTargetCamera;      //нацеленна€ камера
    math::PFirstPersonCamera m_spFirstPersonCamera; //fps камера
};