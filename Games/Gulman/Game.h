#pragma once

#include <engine.h>

namespace gulman
{
	class Game :  public game::IDynamicObject
	{
	public:
		Game();	
		~Game();

	protected:
		void initInput();
		void initCamera();

		void createModel();

		virtual void update(float dt);
		
		void onEsc(const input::CButtonEvent&);	//выход из программы
		void onXAxis(const input::CRelativeAxisEvent &event);
		void onYAxis(const input::CRelativeAxisEvent &event);
		void onWheelAxis(const input::CRelativeAxisEvent &event);

	protected:
		render::PModel			m_spModel;
		render::PFont			m_spFont;
		math::PCamera			m_camera;            //указатель на камеру

		//данные для ввода
		input::CButtonCommand       m_cEsc;
		input::CRelativeAxisCommand m_cXAxis;
		input::CRelativeAxisCommand m_cYAxis;
		input::CRelativeAxisCommand m_cZAxis;

		//данные для камеры
		math::PTargetCamera      m_spTargetCamera;      //контроллер камеры "нацеленная камера"	
	};
}