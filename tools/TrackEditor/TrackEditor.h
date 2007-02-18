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

    void onEsc        (const input::CButtonEvent &event);
    void onInfo       (const input::CButtonEvent &event);
    void onReset      (const input::CButtonEvent &event);
    void onLoadModel  (const input::CButtonEvent &event);
    void onLoadTrack  (const input::CButtonEvent &event);
    void onSaveTrack  (const input::CButtonEvent &event);
    void onChangeMode (const input::CButtonEvent &event);
    void onPrevKey    (const input::CButtonEvent &event);
    void onNextKey    (const input::CButtonEvent &event);
    void onFaster     (const input::CButtonEvent &event);
    void onSlower     (const input::CButtonEvent &event);
    void onAddKey     (const input::CButtonEvent &event);
    void onDelKey     (const input::CButtonEvent &event);
    void onXAxis      (const input::CRelativeAxisEvent &event);
    void onYAxis      (const input::CRelativeAxisEvent &event);

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
    input::CButtonCommand       m_cEsc;   //выход
    input::CButtonCommand       m_cTab;   //скрыть / показать информацию
    input::CButtonCommand       m_c0;     //сбросить программу в начальное состо€ние
    input::CButtonCommand       m_cQ;     //поворот против часовой
    input::CButtonCommand       m_cW;     //вперед
    input::CButtonCommand       m_cE;     //поворот по часовой
    input::CButtonCommand       m_cA;     //боком влево
    input::CButtonCommand       m_cS;     //назад
    input::CButtonCommand       m_cD;     //боком вправо
    input::CButtonCommand       m_cF;     //вверх
    input::CButtonCommand       m_cC;     //вниз
    input::CButtonCommand       m_cF1;    //загрузть модель
    input::CButtonCommand       m_cF2;    //загрузть трек
    input::CButtonCommand       m_cF3;    //сохранить трек
    input::CButtonCommand       m_cF4;    //проигрывание / редактирование трека
    input::CButtonCommand       m_cZ;     //предыдущий ключ
    input::CButtonCommand       m_cX;     //следующий ключ
    input::CButtonCommand       m_cO;     //увеличить скорость
    input::CButtonCommand       m_cP;     //уменьшить скорость
    input::CButtonCommand       m_cSpace; //добавить ключ
    input::CButtonCommand       m_cDel;   //удалить ключ
    input::CRelativeAxisCommand m_cXAxis; //горизонтальна€ ось поворота
    input::CRelativeAxisCommand m_cYAxis; //вертикальна€ ось поворота

    //контроллеры камеры
    math::PFlyCamera   m_spFlyCamera;   //летающа€€ камера
    math::PTrackCamera m_spTrackCamera; //камера перемещающа€с€ по заданной траектории
};