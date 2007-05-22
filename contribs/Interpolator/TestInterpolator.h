//TestInterpolator.h
#pragma once
#include <rgde/input/input.h>
#include <rgde/input/helper.h>
#include <rgde/math/mathTargetCamera.h>
#include <rgde/math/mathSplines.h>

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