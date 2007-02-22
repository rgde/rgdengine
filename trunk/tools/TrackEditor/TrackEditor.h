//TrackEditor.h
#pragma once
#include "math/mathTrack.h"
#include "math/mathFlyCamera.h"
#include "math/mathTrackCamera.h"

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
    input::CKeyDown      m_cEsc;   //выход
    input::CKeyDown      m_cTab;   //скрыть / показать информацию
    input::CKeyDown      m_c0;     //сбросить программу в начальное состо€ние
    input::CButton       m_cQ;     //поворот против часовой
    input::CButton       m_cW;     //вперед
    input::CButton       m_cE;     //поворот по часовой
    input::CButton       m_cA;     //боком влево
    input::CButton       m_cS;     //назад
    input::CButton       m_cD;     //боком вправо
    input::CButton       m_cF;     //вверх
    input::CButton       m_cC;     //вниз
    input::CKeyDown      m_cF1;    //загрузть модель
    input::CKeyDown      m_cF2;    //загрузть трек
    input::CKeyDown      m_cF3;    //сохранить трек
    input::CKeyDown      m_cF4;    //проигрывание / редактирование трека
    input::CKeyDown      m_cZ;     //предыдущий ключ
    input::CKeyDown      m_cX;     //следующий ключ
    input::CKeyDown      m_cO;     //увеличить скорость
    input::CKeyDown      m_cP;     //уменьшить скорость
    input::CKeyDown      m_cSpace; //добавить ключ
    input::CKeyDown      m_cDel;   //удалить ключ
    input::CRelativeAxis m_cXAxis; //горизонтальна€ ось поворота
    input::CRelativeAxis m_cYAxis; //вертикальна€ ось поворота

    //контроллеры камеры
    math::PFlyCamera   m_spFlyCamera;   //летающа€€ камера
    math::PTrackCamera m_spTrackCamera; //камера перемещающа€с€ по заданной траектории
};