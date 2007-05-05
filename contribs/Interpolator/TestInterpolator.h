//TestInterpolator.h
#pragma once
#include "input/input.h"
#include "input/helper.h"
#include "math/mathTargetCamera.h"
#include "math/mathSplines.h"

class TestInterpolator: public game::IDynamicObject
{
public:
    TestInterpolator();
    ~TestInterpolator();

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
	std::vector<math::Vec3f> m_lines;

    render::PModel m_spModel;  //модель
    render::PFont  m_spFont;   //шрифт
    math::PCamera  m_spCamera; //камера

    math::Vec3fCatmulRomDistSpline m_spline; //сплайн

    //контролы ввода
    input::CKeyUp        m_cEsc;   //выход
    input::CKeyDown      m_cR;     //сбросить программу в начальное состояние
    input::CButton       m_cQ;     //поворот против часовой
    input::CButton       m_cE;     //поворот по часовой
    input::CButton       m_cW;     //приблизить
    input::CButton       m_cS;     //удалить
    input::CRelativeAxis m_cXAxis; //горизонтальная ось поворота
    input::CRelativeAxis m_cYAxis; //вертикальная ось поворота

    //контроллер камеры
    math::PTargetCamera m_spTargetCamera; //нацеленная камера
};