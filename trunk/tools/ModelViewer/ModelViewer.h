#pragma once

#include "engine.h"
#include "utils.h"

class ModelViewer :  public game::IDynamicObject
{
public:
    ModelViewer();	
   ~ModelViewer();

protected:
	void initInput();
	void initCamera();

	void createModel(const std::string& strFileName);
	void createLights();

	virtual void update(float dt);
	
	void onEsc(const input::CButtonEvent&);	//выход из программы
	void onO(const input::CButtonEvent&);	// open model

	void onL(const input::CButtonEvent& event);
	void onK(const input::CButtonEvent& event);
	void onB(const input::CButtonEvent& event);
	void onG(const input::CButtonEvent& event);
	void onW(const input::CButtonEvent& event);
	void onXAxis(const input::CRelativeAxisEvent &event);
	void onYAxis(const input::CRelativeAxisEvent &event);
	void onWheelAxis(const input::CRelativeAxisEvent &event);

protected:
	render::PModel			m_spModel;
	render::PFont			m_spFont;
	math::PCamera			m_camera;            //указатель на камеру

	bool m_bRenderGrid;

	//данные для ввода
	input::CButtonCommand       m_cEsc;
	input::CButtonCommand       m_cO;
	input::CButtonCommand       m_cL;
	input::CButtonCommand       m_cK;
	input::CButtonCommand       m_cB;
	input::CButtonCommand       m_cG;
	input::CButtonCommand       m_cW;
	input::CRelativeAxisCommand m_cXAxis;
	input::CRelativeAxisCommand m_cYAxis;
	input::CRelativeAxisCommand m_cZAxis;

	//данные для камеры
	math::PTargetCamera      m_spTargetCamera;      //контроллер камеры "нацеленная камера"	
};