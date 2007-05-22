#pragma once

#include <rgde/math/mathTrack.h>
#include <rgde/math/mathFlyCamera.h>
#include <rgde/math/mathTrackCamera.h>

#include <rgde/input/input.h>

class TrackEditor: public game::IDynamicObject
{
public:
    TrackEditor();
    ~TrackEditor();

protected:
    void initInput();
    void initCamera();
    void initLights();
    void initEditor();

    void update (float dt);

    void onEsc        ();
    void onInfo       ();
    void onReset      ();
    void onLoadModel  ();
    void onLoadTrack  ();
    void onSaveTrack  ();
    void onChangeMode ();
    void onPrevKey    ();
    void onNextKey    ();
    void onFaster     ();
    void onSlower     ();
    void onAddKey     ();
    void onDelKey     ();
    void onXAxis      (int dx);
    void onYAxis      (int dy);

private:
    render::PModel m_spModel;  //модель
    render::PFont  m_spFont;   //шрифт
    math::PCamera  m_spCamera; //камера

    bool m_bPlay;              //режим проигрывани€
    bool m_bInfo;              //режим отображени€ информации
    math::CTrack::iterator i; //итератор, указывающий на текущий ключ трека

    //->
    math::Vec3fCatmulRomSpline m_cSpline;
    void moveTrack2Spline();
    //-<

    //контролы ввода
    input::KeyDown      m_cEsc;   //выход
    input::KeyDown      m_cTab;   //скрыть / показать информацию
    input::KeyDown      m_c0;     //сбросить программу в начальное состо€ние
    input::Button       m_cQ;     //поворот против часовой
    input::Button       m_cW;     //вперед
    input::Button       m_cE;     //поворот по часовой
    input::Button       m_cA;     //боком влево
    input::Button       m_cS;     //назад
    input::Button       m_cD;     //боком вправо
    input::Button       m_cF;     //вверх
    input::Button       m_cC;     //вниз
    input::KeyDown      m_cF1;    //загрузть модель
    input::KeyDown      m_cF2;    //загрузть трек
    input::KeyDown      m_cF3;    //сохранить трек
    input::KeyDown      m_cF4;    //проигрывание / редактирование трека
    input::KeyDown      m_cZ;     //предыдущий ключ
    input::KeyDown      m_cX;     //следующий ключ
    input::KeyDown      m_cO;     //увеличить скорость
    input::KeyDown      m_cP;     //уменьшить скорость
    input::KeyDown      m_cSpace; //добавить ключ
    input::KeyDown      m_cDel;   //удалить ключ
    input::RelativeAxis m_cXAxis; //горизонтальна€ ось поворота
    input::RelativeAxis m_cYAxis; //вертикальна€ ось поворота

    //контроллеры камеры
    math::PFlyCamera   m_spFlyCamera;   //летающа€€ камера
    math::PTrackCamera m_spTrackCamera; //камера перемещающа€с€ по заданной траектории
};