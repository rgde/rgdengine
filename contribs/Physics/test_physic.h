#pragma once
#include "engine.h"

class TestPhysic :  public game::IDynamicObject
{
public:
    TestPhysic();	
   ~TestPhysic();

protected:
    void initInput();
    void initCamera();
    void initLights();

    void createModel(render::PModel &pModel, const std::string& strFileName);

    virtual void update(float dt);
    
    void onEsc(); //выход из программы
    void onO();   //open model
    void onL();
    void onK();
    void onB();
    void onG();
    void onR();
    void onSpace();
    void onMinus();
    void onEquals();
    void onT();
    void onI();
    void onF();
    void onC();
    void onH();
    void onE();
    void onXAxis(int dx);
    void onYAxis(int dy);
    void onWheelAxis(int dz);

protected:
    render::PModel m_spModelPlane, m_spModel;
    render::PFont  m_spFont;
    math::PCamera  m_spCamera; //указатель на камеру

    bool     m_bCollideCamera;
    bool     m_bShowHelp;
    bool     m_bRenderGrid;
    bool     m_bUseFPSCamera;    // use fps cam
    unsigned m_iRenderSpaceTree;
    bool     m_bMove;
    unsigned m_iSpeed;

    //данные для ввода
    input::Button       m_cW, m_cA, m_cS, m_cD, m_cUp, m_cDown, m_cLeft, m_cRight;
    input::KeyDown      m_cEsc, m_cO, m_cL, m_cK, m_cB, m_cG, m_cR, m_cSpace,
                         m_cC, m_cT, m_cI, m_cF, m_cH, m_cE,
                         m_cMinus, m_cEquals;
    input::RelativeAxis m_cXAxis, m_cYAxis, m_cZAxis;

    //данные для камеры
    math::PTargetCamera      m_spTargetCamera;      //контроллер камеры "нацеленная камера"	
    math::PFirstPersonCamera m_spFirstPersonCamera;
};