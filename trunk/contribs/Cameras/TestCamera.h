#pragma once

#include <rgde/input/input.h>
#include <rgde/input/helper.h>

#include <rgde/math/mathFlyCamera.h>
#include <rgde/math/mathTargetCamera.h>
#include <rgde/math/mathFirstPersonCamera.h>

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
    input::KeyUp        m_cEsc;   //выход
    input::KeyDown      m_cSpace; //переключение режима wirframe/solid
    input::KeyDown      m_cTab;   //переключить камеру
    input::KeyDown      m_cR;     //сбросить программу в начальное состо€ние
    input::Button       m_cQ;     //поворот против часовой
    input::Button       m_cW;     //вперед
    input::Button       m_cE;     //поворот по часовой
    input::Button       m_cA;     //боком влево
    input::Button       m_cS;     //назад
    input::Button       m_cD;     //боком вправо
    input::Button       m_cF;     //вверх
    input::Button       m_cC;     //вниз
    input::RelativeAxis m_cXAxis; //горизонтальна€ ось поворота
    input::RelativeAxis m_cYAxis; //вертикальна€ ось поворота

    //контроллеры камеры
    math::PFlyCamera         m_spFlyCamera;         //летающа€€ камера
    math::PTargetCamera      m_spTargetCamera;      //нацеленна€ камера
    math::PFirstPersonCamera m_spFirstPersonCamera; //fps камера
};