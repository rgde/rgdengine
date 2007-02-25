//TestCamera.h
#pragma once
#include "input/input.h"
#include "input/helper.h"
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

    void onEsc          ();
    void onWireframe    ();
    void onReset        ();
    void onChangeCamera ();
    void onXAxis        (int dx);
    void onYAxis        (int dy);

private:
    render::PModel m_spModel;  //модель
    render::PFont  m_spFont;   //шрифт
    math::PCamera  m_spCamera; //камера

    int m_nCameraType; //номер используемой камеры

    //контролы ввода
    input::CKeyUp        m_cEsc;   //выход
    input::CKeyDown      m_cSpace; //переключение режима wirframe/solid
    input::CKeyDown      m_cTab;   //переключить камеру
    input::CKeyDown      m_cR;     //сбросить программу в начальное состо€ние
    input::CButton       m_cQ;     //поворот против часовой
    input::CButton       m_cW;     //вперед
    input::CButton       m_cE;     //поворот по часовой
    input::CButton       m_cA;     //боком влево
    input::CButton       m_cS;     //назад
    input::CButton       m_cD;     //боком вправо
    input::CButton       m_cF;     //вверх
    input::CButton       m_cC;     //вниз
    input::CRelativeAxis m_cXAxis; //горизонтальна€ ось поворота
    input::CRelativeAxis m_cYAxis; //вертикальна€ ось поворота

    //контроллеры камеры
    math::PFlyCamera         m_spFlyCamera;         //летающа€€ камера
    math::PTargetCamera      m_spTargetCamera;      //нацеленна€ камера
    math::PFirstPersonCamera m_spFirstPersonCamera; //fps камера
};