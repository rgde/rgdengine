#pragma once

#include "engine.h"
#include "scene/SceneManager.h"
#include "scene/SceneTree.h"

class TestPhysic :  public game::IDynamicObject
{
public:
	TestPhysic();	
	~TestPhysic();

protected:
	void initInput();
	void initCamera();

	void createModel(render::PModel &pModel, const std::string& strFileName);
	void createLights();

	virtual void update(float dt);
	
	void onEsc(const input::CButtonEvent&);	//выход из программы
	void onO(const input::CButtonEvent&);	// open model

	void onL(const input::CButtonEvent& event);
	void onK(const input::CButtonEvent& event);
	void onB(const input::CButtonEvent& event);
	void onG(const input::CButtonEvent& event);

	void onR(const input::CButtonEvent& event);
	void onSpace(const input::CButtonEvent& event);
	void onMinus(const input::CButtonEvent& event);
	void onEquals(const input::CButtonEvent& event);
	void onT(const input::CButtonEvent& event);
	void onI(const input::CButtonEvent& event);
	void onF(const input::CButtonEvent& event);
	void onC(const input::CButtonEvent& event);
	void onH(const input::CButtonEvent& event);
	void onE(const input::CButtonEvent& event);
	void onXAxis(const input::CRelativeAxisEvent &event);
	void onYAxis(const input::CRelativeAxisEvent &event);
	void onWheelAxis(const input::CRelativeAxisEvent &event);

protected:
	render::PModel			m_spModelPlane, m_spModel;
	render::PFont			m_spFont;
	math::PCamera			m_spCamera;            //указатель на камеру

	bool						m_bCollideCamera;
	bool						m_bShowHelp;
	bool						m_bRenderGrid;
	bool						m_bUseFPSCamera;	// use fps cam
	unsigned					m_iRenderSpaceTree;
	bool						m_bMove;

	unsigned					m_iSpeed;

	//данные для ввода
	input::CButtonCommand       m_cEsc, m_cO, m_cL, m_cK, m_cB, m_cG, m_cR, m_cSpace,
								m_cW, m_cA, m_cS, m_cD, m_cC, m_cT, m_cI, m_cF, m_cH, m_cE,
								m_cUp, m_cDown, m_cLeft, m_cRight, m_cMinus, m_cEquals;
	input::CRelativeAxisCommand	m_cXAxis, m_cYAxis, m_cZAxis;

	//данные для камеры
	math::PTargetCamera      m_spTargetCamera;      //контроллер камеры "нацеленная камера"	
	math::PFirstPersonCamera m_spFirstPersonCamera;
};