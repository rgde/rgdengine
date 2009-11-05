//RGDE
#include <rgde/engine.h>

//класс для теста рендера, камеры и ввода
class CTest : public game::IDynamicObject
{
	void setupGraphics()
	{
		m_pFont = render::IFont::Create(14, L"Tahoma", render::IFont::Bold);
		m_pModel = render::CModel::Create("test_teapot.xml");
		render::TheLightManager::Get().setAmbientColor(math::Color(120, 120, 120, 255));
	}

	void setupInput()
	{
		m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
		m_cQ.attachToControl(input::Keyboard, input::KeyQ);
		m_cE.attachToControl(input::Keyboard, input::KeyE);
		m_cA.attachToControl(input::Keyboard, input::KeyA);
		m_cD.attachToControl(input::Keyboard, input::KeyD);
		m_cW.attachToControl(input::Keyboard, input::KeyW);
		m_cS.attachToControl(input::Keyboard, input::KeyS);
		m_cF.attachToControl(input::Keyboard, input::KeyF);
		m_cC.attachToControl(input::Keyboard, input::KeyC);
		m_cSpace.attachToControl(input::Keyboard, input::KeySpace);
		m_cXAxis.attachToControl(input::Mouse, input::AxisX);
		m_cYAxis.attachToControl(input::Mouse, input::AxisY);
		m_cEsc.addHandler(this,&CTest::onEsc);
		m_cSpace.addHandler(this,&CTest::onSpace);
		m_cYAxis.addHandler(this,&CTest::onYAxis);
		m_cXAxis.addHandler(this,&CTest::onXAxis);
	}

	/////////////////////////////////////////
	// инициализация камер
	/////////////////////////////////////////
	void setupCameras()
	{
		// Создание главной камеры (без отдельных render-targets)
		m_pRenderCameraPrimary = render::CRenderCamera::Create();
		m_pRenderCameraPrimary->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);

		// Создание вторичной камеры (с отдельными render-targets)
		render::SViewPort			viewport(0, 0, 320, 240);
		render::SRenderTargetInfo	params;

		m_pRenderCameraSecondary = render::CRenderCamera::Create(0, viewport);
		m_pRenderCameraSecondary->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);

		// Создание таргетов для камеры
		params.format	= render::A8R8G8B8;
		params.size		= math::Vec2i(80, 60);
		m_renderTarget	= render::IRenderTexture::Create(params);
		m_pRenderCameraSecondary->setColorTarget(m_renderTarget, math::Color(0, 0, 0, 255));
        params.format	= render::D16;
		m_renderDepth	= render::IRenderTexture::Create(params);
		m_pRenderCameraSecondary->setDepthStencilTarget(m_renderDepth, 1.0f);
		
		// Добавление камер к рендеру
		render::TheCameraManager::Get().addCamera(m_pRenderCameraPrimary);
		render::TheCameraManager::Get().addCamera(m_pRenderCameraSecondary);

		// Создание спрайта для отображения вида вторичной камеры
		render::TheSpriteManager::Get().set_origin(math::Vec2f(0, 0));

		math::Rect	rect(0, 0, 1, 1);
		math::Vec2f pos	(0, 0);
		math::Vec2f size(800, 600);

		render::SSprite sprite(pos, size, 0xffffffff, m_renderTarget, 0, rect);
		m_vSprites.push_back(sprite);
		
		// Установка контроллеров главной камеры
		m_cFirstPersonCamera.setCamera(m_pRenderCameraPrimary);
		//m_cTargetCamera.setCamera(m_pRenderCameraSecondary);
		m_cTargetCamera.setCamera(m_pRenderCameraPrimary);

		// Настройка видовой матрицы контроллеров
		math::Vec3f vEyePt(100.0f, 80.0f, 20.0f);
		math::Vec3f vLookatPt(0.0f, 0.0f, 0.0f);
		math::Vec3f vUpVec(0.0f, 1.0f, 0.0f);

		m_cTargetCamera.setPosition		(vEyePt,vLookatPt,vUpVec);
		m_cFirstPersonCamera.setPosition(vEyePt,vLookatPt);
		
		// Установка текущего контроллера
		m_cFirstPersonCamera.activate();
		m_nCamera = 0;		
	}
public:
	CTest(): spApp(core::IApplication::Create())			 
	{
		spApp->addTask(core::PTask(new core::CRenderTask(2)));
		spApp->addTask(core::PTask(new core::CGameTask(1)));
		spApp->addTask(core::PTask(new core::CInputTask(0)));

        //инициализация рендера
		setupGraphics();

		//инициализация ввода
		setupInput();


		//инициализация камер
		setupCameras();

		spApp->Run();
	}


protected:

	//апдейт
	virtual void update(float dt)
	{
		render::TheDevice::Get().showFPS(m_pFont);
		render::TheDevice::Get().showWiredFloorGrid(200.0f);

		const float move = 6.5f*dt;
		const float rotate = 1.5f*dt;

		m_pFont->renderText(L"Press SPACE for change camera type", math::Rect(30,30, 400,400), 0xFFFFFFFF, true);
		switch(m_nCamera)
		{
			case 0: m_pFont->renderText(L"First Person Camera", math::Rect(30,60, 400,400), 0xFFFF0000, true); break;
			case 1: m_pFont->renderText(L"Target Camera", math::Rect(30,60, 400,400), 0xFFFF0000, true); break;		
		}

		math::Vec3f vEyePt;
		math::Vec3f vLookatPt;
		math::Vec3f vUpVec(0, 1, 0);

		switch(m_nCamera)
		{
		case 0:
			if (m_cW) m_cFirstPersonCamera.goForward(move);
			if (m_cS) m_cFirstPersonCamera.goBackward(move);
		    if (m_cA) m_cFirstPersonCamera.goLeft(move);
			if (m_cD) m_cFirstPersonCamera.goRight(move);
			//if (m_cF) m_cFirstPersonCamera.goUp(move);
			//if (m_cC) m_cFirstPersonCamera.goDown(move);
			m_cFirstPersonCamera.getPosition(vEyePt, vLookatPt);			
			break;
		case 1:
			if (m_cW) m_cTargetCamera.goForward(move);
			if (m_cS) m_cTargetCamera.goBackward(move);
			if (m_cA) m_cTargetCamera.rotateLeft(rotate);
			if (m_cD) m_cTargetCamera.rotateRight(rotate);
			if (m_cF) m_cTargetCamera.rotateDown(rotate);
			if (m_cC) m_cTargetCamera.rotate_up(rotate);
			if (m_cQ) m_cTargetCamera.rotateCCW(rotate);
			if (m_cE) m_cTargetCamera.rotateCW(rotate);
			m_cTargetCamera.getCameraPosition(vEyePt);
			m_cTargetCamera.getTargetPosition(vLookatPt);
			m_cTargetCamera.getUpVector(vUpVec);
			break;
		}
		
		m_pRenderCameraSecondary->lookAt(vEyePt, vLookatPt, vUpVec);
				
		render::TheSpriteManager::Get().addSprite(*m_vSprites.begin());
	}

	//выход из программы
	void onEsc(const input::CButtonEvent&)
	{
		core::IApplication::Get()->close();
	}

	//смена типа камеры
	void onSpace(const input::CButtonEvent &event)
	{
		if (event.m_bPress)
		{
			m_nCamera = (m_nCamera+1)%2;
			switch(m_nCamera)
			{
			case 0: m_cFirstPersonCamera.activate(); break;
			case 1: m_cTargetCamera.activate(); break;
			}
		}
	}

	//ось X
	void onXAxis(const input::CRelativeAxisEvent &event)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

		switch(m_nCamera)
		{
		case 0: m_cFirstPersonCamera.rotateLeft(angle); break;
		case 1: m_cTargetCamera.rotateLeft(-angle); break;
		}
	}

	//ось Y
	void onYAxis(const input::CRelativeAxisEvent &event)
	{
		const int accel = 5;
		const float slow = .01f;
		const float fast = 2*slow;
		float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

		switch(m_nCamera)
		{
		case 0: m_cFirstPersonCamera.rotate_up(angle); break;
		case 1: m_cTargetCamera.rotate_up(angle); break;
		}
	}
protected:
	std::auto_ptr<core::IApplication> spApp;

	//данные для рендера
	math::CFrame      m_frame;
	render::PFont     m_pFont;
	render::PModel    m_pModel;
	
	//данные для ввода
	input::Button       m_cEsc;
	input::Button       m_cQ;
	input::Button       m_cW;
	input::Button       m_cE;
	input::Button       m_cA;
	input::Button       m_cS;
	input::Button       m_cD;
	input::Button       m_cF;
	input::Button       m_cC;
	input::Button       m_cSpace;
	input::RelativeAxis m_cXAxis;
	input::RelativeAxis m_cYAxis;

	//данные для камеры
	int                      m_nCamera;				//номер текущего типа камеры (0-1)
	math::CTargetCamera      m_cTargetCamera;		//контроллер камеры "нацеленная камера"
	math::CFirstPersonCamera m_cFirstPersonCamera;	//контроллер камеры "вид из глаз"
	render::PCamera	 m_pRenderCameraSecondary;
	render::PCamera	 m_pRenderCameraPrimary;
	render::PRenderTexture	 m_renderTarget;
	render::PRenderTexture	 m_renderDepth;
	render::CSpriteManager::TSprites m_vSprites;
};

//здесь начинается работа программы
int __stdcall WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
	CTest test;
}