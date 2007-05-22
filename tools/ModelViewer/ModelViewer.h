#pragma once
#include <rgde/engine.h>
#include "utils.h"

class ModelViewer: public game::IDynamicObject
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
	
	void onEsc();	//выход из программы
	void onO();	//open model
	void onL();
	void onK();
	void onB();
	void onG();
	void onW();
	void onXAxis(int dx);
	void onYAxis(int dy);
	void onWheelAxis(int dz);

protected:
	render::PModel m_spModel;
	render::PFont  m_spFont;
	math::PCamera  m_camera; //указатель на камеру

	bool m_bRenderGrid;

	//данные для ввода
	input::KeyDown      m_cEsc;
	input::KeyDown      m_cO;
	input::KeyDown      m_cL;
	input::KeyDown      m_cK;
	input::KeyDown      m_cB;
	input::KeyDown      m_cG;
	input::KeyDown      m_cW;
    input::Button       m_cMouseLeft;
    input::Button       m_cMouseRight;
	input::RelativeAxis m_cXAxis;
	input::RelativeAxis m_cYAxis;
	input::RelativeAxis m_cZAxis;

	//данные для камеры
	math::PTargetCamera  m_spTargetCamera; //контроллер камеры "нацеленная камера"	
};