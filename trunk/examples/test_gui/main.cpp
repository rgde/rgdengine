//RGDE
#include <rgde/engine.h>
#include <rgde/math/mathCamera.h>

// ui
#include "uiButton.h"
#include "uiManager.h"
#include "uiControl.h"

class VeryVerySimpleGame  : public game::IDynamicObject // :)
{
public:
	VeryVerySimpleGame() 
		//: m_spApp(core::IApplication::Create(L"GUI Example"))
		//: m_spApp(core::IApplication::Create(L"GUI Example", 640, 480, false))						   
	{
		m_app = AplicationPtr(core::IApplication::Create(L"GUI Example"));
		m_pCamera = new math::CBaseCamera;
		m_cTargetCamera = math::CTargetCamera::Create(m_pCamera);

		m_app->addTask(core::PTask(new core::InputTask(*m_app, 0)));
		m_app->addTask(core::PTask(new core::RenderTask(*m_app, 1)));
		m_app->addTask(core::PTask(new core::CGameTask(*m_app, 2)));

		ui::TheUIManager::Get().addControl(	new ui::uiButton );

		////input 
		//m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
		//m_cEsc.addHandler(this, &VeryVerySimpleGame::onEsc);

		math::Vec3f vEyePt( 0.0f, 10, -30 );
		math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
		math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );

		m_pCamera->lookAt(vEyePt, vLookatPt, vUpVec);
		m_pCamera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
		m_pCamera->activate();

		// Инициализация ввода
		//m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
		//m_cW.attachToControl(input::Keyboard, input::KeyW);
		//m_cS.attachToControl(input::Keyboard, input::KeyS);
		//m_cA.attachToControl(input::Keyboard, input::KeyA);
		//m_cD.attachToControl(input::Keyboard, input::KeyD);
		//m_cF.attachToControl(input::Keyboard, input::KeyF);
		//m_cC.attachToControl(input::Keyboard, input::KeyC);
		//m_cQ.attachToControl(input::Keyboard, input::KeyQ);
		//m_cE.attachToControl(input::Keyboard, input::KeyE);
		//m_cXAxis.attachToControl(input::Mouse, input::AxisX);
		//m_cYAxis.attachToControl(input::Mouse, input::AxisY);
		//m_cRightButton.attachToControl(input::Mouse, input::ButtonRight);
		//m_cEsc.addHandler(this,&VeryVerySimpleGame::onEsc);
		//m_cYAxis.addHandler(this,&VeryVerySimpleGame::onYAxis);
		//m_cXAxis.addHandler(this,&VeryVerySimpleGame::onXAxis);


		// Инициализация камеры
		//m_cTargetCamera.setCamera(m_pCamera);
		m_cTargetCamera->setPosition(vEyePt,vLookatPt,vUpVec);
		m_cTargetCamera->activate();

		m_app->Run();
	}


protected:
	virtual void update(float dt)
	{
		const float move = 40*dt;
		const float rotate = 3*dt;

		//if (m_cW) m_cTargetCamera.goForward(move);
		//if (m_cS) m_cTargetCamera.goBackward(move);
		//if (m_cA) m_cTargetCamera.rotateLeft(rotate);
		//if (m_cD) m_cTargetCamera.rotateRight(rotate);
		//if (m_cF) m_cTargetCamera.rotateDown(rotate);
		//if (m_cC) m_cTargetCamera.rotateUp(rotate);
		//if (m_cQ) m_cTargetCamera.rotateCCW(rotate);
		//if (m_cE) m_cTargetCamera.rotateCW(rotate);
	}

	//void onEsc (const input::CButtonEvent&)
	//{
	//	core::IApplication::Get()->close();
	//}

	////ось X
	//void onXAxis(const input::CRelativeAxisEvent &event)
	//{
	//	const int accel = 5;
	//	const float slow = .01f;
	//	const float fast = 2*slow;

	//	float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

	//	if (m_cRightButton)
	//		m_cTargetCamera.rotateLeft(-angle);
	//}

	////ось Y
	//void onYAxis(const input::CRelativeAxisEvent &event)
	//{
	//	const int accel = 5;
	//	const float slow = .01f;
	//	const float fast = 2*slow;
	//	float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

	//	if (m_cRightButton)
	//		m_cTargetCamera.rotateUp(angle);
	//}

protected:
	math::PCamera				m_pCamera;
	math::PTargetCamera			m_cTargetCamera;      //контроллер камеры "нацеленная камера"

	//input::Button       m_cEsc;
	//input::Button       m_cQ;
	//input::Button       m_cW;
	//input::Button       m_cE;
	//input::Button       m_cA;
	//input::Button       m_cS;
	//input::Button       m_cD;
	//input::Button       m_cF;
	//input::Button       m_cC;
	//input::Button		m_cRightButton;
	//input::RelativeAxis m_cXAxis;
	//input::RelativeAxis m_cYAxis;

	typedef boost::shared_ptr<core::IApplication> AplicationPtr;
	AplicationPtr m_app;
};


void  main()
{
	VeryVerySimpleGame r;
}