#pragma warning(disable : 4244) // type conversion warning.
#pragma warning(disable : 4996) // '_function_' was declared deprecated

#include "test_physic.h"
#include "strsafe.h"

#define FSIZE				11.0f

TestPhysic::TestPhysic() 
{
	m_spCamera = render::CRenderCamera::Create();
    m_spTargetCamera = math::CTargetCamera::Create(m_spCamera);
    m_spFirstPersonCamera = math::CFirstPersonCamera::Create(m_spCamera);
	render::TheCameraManager::Get().addCamera(m_spCamera);

	render::TheRenderManager::Get().enableVolumes( false );
	render::TheRenderManager::Get().enableLighting(false);

	m_spFont = render::IFont::Create(FSIZE, L"Arial", render::IFont::Heavy);

	//	std::string cmd_line = getCommandLineParams();
	//	if (cmd_line.size() > 0)
	//		createModel(cmd_line);

	//
	initCamera();
	////инициализация ввода
	initInput();
	createLights();

	m_bShowHelp = false;
	m_bRenderGrid = false;//true;
	m_bMove = false;

	m_bCollideCamera = false;

	m_iSpeed = 10;

	m_iRenderSpaceTree = 0;//2;
	collision::TheCollider::Get().setDTL(2);//(3);
}


TestPhysic::~TestPhysic()
{
}

void TestPhysic::initInput()
{
	m_cUp		.attachToControl(input::Keyboard,	input::KeyUp);
	m_cDown		.attachToControl(input::Keyboard,	input::KeyDown);
	m_cLeft		.attachToControl(input::Keyboard,	input::KeyLeft);
	m_cRight	.attachToControl(input::Keyboard,	input::KeyRight);

	m_cEsc		.attachToControl(input::Keyboard,	input::KeyEscape);
	m_cO		.attachToControl(input::Keyboard,	input::KeyO);
	m_cL		.attachToControl(input::Keyboard,	input::KeyL);
	m_cK		.attachToControl(input::Keyboard,	input::KeyK);
	m_cB		.attachToControl(input::Keyboard,	input::KeyB);
	m_cG		.attachToControl(input::Keyboard,	input::KeyG);
	m_cR		.attachToControl(input::Keyboard,	input::KeyR);
	m_cW		.attachToControl(input::Keyboard,	input::KeyW);
	m_cA		.attachToControl(input::Keyboard,	input::KeyA);
	m_cS		.attachToControl(input::Keyboard,	input::KeyS);
	m_cD		.attachToControl(input::Keyboard,	input::KeyD);
	m_cC		.attachToControl(input::Keyboard,	input::KeyC);
	m_cT		.attachToControl(input::Keyboard,	input::KeyT);
	m_cI		.attachToControl(input::Keyboard,	input::KeyI);
	m_cF		.attachToControl(input::Keyboard,	input::KeyF);
	m_cH		.attachToControl(input::Keyboard,	input::KeyH);
	m_cE		.attachToControl(input::Keyboard,	input::KeyE);
	m_cSpace	.attachToControl(input::Keyboard,	input::KeySpace);
	m_cMinus	.attachToControl(input::Keyboard,	input::KeyMinus);
	m_cEquals	.attachToControl(input::Keyboard,	input::KeyEquals);

	m_cXAxis	.attachToControl(input::Mouse,		input::AxisX);
	m_cYAxis	.attachToControl(input::Mouse,		input::AxisY);
	m_cZAxis	.attachToControl(input::Mouse,		input::AxisWheel);

	m_cEsc		.addHandler(this,	&TestPhysic::onEsc);
	m_cO		.addHandler(this,	&TestPhysic::onO);
	m_cL		.addHandler(this,	&TestPhysic::onL);
	m_cK		.addHandler(this,	&TestPhysic::onK);
	m_cB		.addHandler(this,	&TestPhysic::onB);
	m_cG		.addHandler(this,	&TestPhysic::onG);
	m_cR		.addHandler(this,	&TestPhysic::onR);
	m_cC		.addHandler(this,	&TestPhysic::onC);
	m_cT		.addHandler(this,	&TestPhysic::onT);
	m_cI		.addHandler(this,	&TestPhysic::onI);
	m_cF		.addHandler(this,	&TestPhysic::onF);
	m_cH		.addHandler(this,	&TestPhysic::onH);
	m_cE		.addHandler(this,	&TestPhysic::onE);
	m_cSpace	.addHandler(this,	&TestPhysic::onSpace);
	m_cMinus	.addHandler(this,	&TestPhysic::onMinus);
	m_cEquals	.addHandler(this,	&TestPhysic::onEquals);

	m_cYAxis	.addHandler(this,	&TestPhysic::onYAxis);
	m_cXAxis	.addHandler(this,	&TestPhysic::onXAxis);
	m_cZAxis	.addHandler(this,	&TestPhysic::onWheelAxis);
}

void TestPhysic::initCamera()
{
	math::Vec3f vEyePt( 10.0f, -50.0f, 10.0f );
	math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
	math::Vec3f vUpVec( 0.0f, 0.0f, 1.0f );

    m_spCamera->setProjection(math::Math::PI/4, 4.f/3.f, 1.0f, 10000.0f);

    m_spFirstPersonCamera->setPosition(vUpVec,vEyePt,vLookatPt);
    m_spTargetCamera->setPosition(vUpVec,vEyePt,vLookatPt);

	m_spTargetCamera->activate();
}

void TestPhysic::createModel(render::PModel &pModel, const std::string& strFileName)
{
	if (pModel)
	{
		scene::TheScene::Get().getRootFrame()->removeChild(pModel);
		pModel = 0;
	}

	std::string file = io::helpers::getFileNameWithoutExtension(strFileName);
	pModel = render::CModel::Create(file);

	if(pModel)
	{
		scene::TheScene::Get().getRootFrame()->addChild(pModel);
		pModel->play();
		pModel->setLooped( true );
	}
}

void TestPhysic::createLights()
{
	//render::TheLightManager::Get().setAmbientColor(math::Color(20, 20, 20, 255));
}


void TestPhysic::update(float dt)
{
	if(dt>0.3f)
		dt = 0.3f;

	WCHAR szString[256];
	math::Vec3f pos, pos_, trg, up;

	// ul - upper left (1 -FPS, 2 -dbg tris,3 - skip), ll - lower left (1- first pos)
	int ul = 3, ll = 1,
		colWhite	= 0xFFFFFFFF,
		colRed		= 0xFFFF0000,
		colGrey		= 0xEEEEEEEE,
		colGreen	= 0xEE00FF00;

	//TODO
	//	Physic::Get().drawDebug();

	//testing move
	if(m_bMove)
	{
		int iTest = 0;
		//		if(m_spModelPlane)
		//			m_spModelPlane->update( dt );

		//		if(m_spModel)
		//			m_spModel->update( dt );
	}

	const float movecam = 10.0f*dt*m_iSpeed;

//	pos = m_camera.get()->getPos();
//	StringCchPrintf(szString, 256,L"Camera pos 1: %3.1f:%3.1f:%3.1f",pos[0],pos[1],pos[2]);
//	m_spFont->renderText(szString,math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);
//	const float movecam = 10.0f*dt*m_iSpeed;

	if(m_bUseFPSCamera)
	{
		// move cursor to center
		int x = GetSystemMetrics(SM_CXSCREEN) / 2;
		int y = GetSystemMetrics(SM_CYSCREEN) / 2;
		SetCursorPos(x,y);

		m_spFirstPersonCamera->getPosition(up,pos_,trg);

		// process cmds
		if(m_cW)	m_spFirstPersonCamera->goForward(movecam);
		if(m_cS)	m_spFirstPersonCamera->goBackward(movecam);
		if(m_cA)	m_spFirstPersonCamera->goLeft(movecam);
		if(m_cD)	m_spFirstPersonCamera->goRight(movecam);

		if(m_cUp)	m_spFirstPersonCamera->rotateUp(dt);
		if(m_cDown)	m_spFirstPersonCamera->rotateDown(dt);
		if(m_cLeft)	m_spFirstPersonCamera->rotateLeft(dt);
		if(m_cRight)m_spFirstPersonCamera->rotateRight(dt);
	}
	else
	{
		m_spTargetCamera->getPosition(up,pos_,trg);

		if(m_cW)	m_spTargetCamera->goForward(movecam);
		if(m_cS)	m_spTargetCamera->goBackward(movecam);
		if(m_cA)	m_spTargetCamera->rotateLeft(dt);
		if(m_cD)	m_spTargetCamera->rotateRight(dt);

		if(m_cUp)	m_spTargetCamera->rotateUp(dt);
		if(m_cDown)	m_spTargetCamera->rotateDown(dt);
		if(m_cLeft)	m_spTargetCamera->rotateLeft(dt);
		if(m_cRight)m_spTargetCamera->rotateRight(dt);
	}

//	pos = m_camera.get()->getGlobalPosition();
//	StringCchPrintf(szString, 256,L"Camera pos 2: %3.1f:%3.1f:%3.1f",pos[0],pos[1],pos[2]);
//	m_spFont->renderText(szString,math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);
	pos = m_spCamera.get()->getGlobalPosition();
	StringCchPrintf(szString, 256,L"Camera pos 2: %3.1f:%3.1f:%3.1f",pos[0],pos[1],pos[2]);
	m_spFont->render(szString,math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite,true);

	//
	// update/approximate global tree
	scene::TheScene::Get().approximate();

	m_bCollideCamera = collision::TheCollider::Get().CollideGlobal(*m_spCamera.get());

	if(m_bRenderGrid){
		render::TheDevice::Get().showWiredFloorGrid(300.0f, 20, math::Color(150, 150, 150, 255));
		render::TheDevice::Get().showWiredFloorGrid(300.0f, 2, math::Color(60, 60, 60, 255));
	}

	// debug draw
	if(m_iRenderSpaceTree==1)
	{
		scene::TheScene::Get().draw();
	}

	if(m_bCollideCamera)
	{
		if(m_bUseFPSCamera)
		{
			m_spFirstPersonCamera->setPosition(up,pos_,trg);
		}
		else
		{
			m_spTargetCamera->setPosition(up,pos_,trg);
		}
		m_spFont->render(L"Collide",math::Rect(1,600-(ll++)*FSIZE,400,400),colRed);
	}
	else
	{
		m_spFont->render(L"Free",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);
	}

	pos = m_spCamera.get()->getPos();
	trg = m_spCamera.get()->getExt();
	StringCchPrintf(szString, 256,L"Camera pos 3: %3.1f:%3.1f:%3.1f",pos[0],pos[1],pos[2]);
	m_spFont->render(szString,math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite,true);
	StringCchPrintf(szString, 256,L"Camera trg: %3.1f:%3.1f:%3.1f",trg[0],trg[1],trg[2]);
	m_spFont->render(szString,math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite,true);

	if(m_bShowHelp)
	{
		render::TheDevice::Get().showFPS(m_spFont);

		if(m_bUseFPSCamera)
			m_spFirstPersonCamera->getPosition(up,pos,trg);
		else
			m_spTargetCamera->getPosition(up,pos,trg);
		StringCchPrintf(szString, 256, L"Camera pos: %3.1f:%3.1f:%3.1f",pos[0],pos[1],pos[2]);
		m_spFont->render(szString,math::Rect(1,(ul++)*FSIZE,400,400),colWhite);
		StringCchPrintf(szString, 256, L"Camera trg: %3.1f:%3.1f:%3.1f",trg[0],trg[1],trg[2]);
		m_spFont->render(szString,math::Rect(1,(ul++)*FSIZE,400,400),colWhite);
		m_spFont->render(m_bUseFPSCamera?L"Camera FPS":L"Camera target",
			math::Rect(1,(ul++)*FSIZE,400,400),colWhite);
		m_spFont->render(szString,math::Rect(1,(ul++)*FSIZE,400,400),colWhite);
		m_spFont->render(m_bUseFPSCamera?L"Camera FPS":L"Camera target",
			math::Rect(1,(ul++)*FSIZE,400,400),colWhite,true);

		m_spFont->render(L"\"H\" - выкл помощь",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);

		if(render::TheRenderManager::Get().getFillMode()==render::WireFrame)
			m_spFont->render(L"\"R\" - выкл wireframe режима",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);
		else
			m_spFont->render(L"\"R\" - вкл wireframe режима",math::Rect(1,600-(ll++)*FSIZE,400,400),colGrey);

		if(m_bRenderGrid)
			m_spFont->render(L"\"G\" - выкл отображения сетки",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);
		else
			m_spFont->render(L"\"G\" - вкл отображения сетки",math::Rect(1,600-(ll++)*FSIZE,400,400),colGrey);

		//		if(render::TheLightManager::Get().isDebugRendering())
		//			m_spFont->renderText(L"\"L\" - выкл отображения источников света",math::Rect(1,600-(ll++)*FSIZE,800,400),colGreen);
		//		else
		//			m_spFont->renderText(L"\"L\" - вкл отображения источников света",math::Rect(1,600-(ll++)*FSIZE,800,400),colGrey);

		if(render::TheRenderManager::Get().isLightingEnabled())
			m_spFont->render(L"\"K\" - выкл освещения",math::Rect(1,600-(ll++)*FSIZE,800,400),colGreen);
		else
			m_spFont->render(L"\"K\" - вкл освещения",math::Rect(1,600-(ll++)*FSIZE,800,400),colGrey);

		if(render::TheRenderManager::Get().isVolumeDrawing())
			m_spFont->render(L"\"B\" - выкл отображения AABBox-ов",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);
		else
			m_spFont->render(L"\"B\" - вкл отображения AABBox-ов",math::Rect(1,600-(ll++)*FSIZE,400,400),colGrey);

		m_spFont->render(L"\"C\" - смена камеры",math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);
		m_spFont->render(L"Мышь - вращение камерой",math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);

		m_spFont->render(L"-= Остальное =-",math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);


		//if(Physic::Get().getIdle())
		//	m_spFont->renderText(L"\"I\" - выкл процесс",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);
		//else
		//	m_spFont->renderText(L"\"I\" - вкл процесс",math::Rect(1,600-(ll++)*FSIZE,400,400),colGrey);

		//			if(Physic::Get().getDebug()==IDebugDraw::DBG_DrawAabb)
		//				m_spFont->renderText(L"\"F\" - показать контактные точки",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);
		//			else
		//			if(Physic::Get().getDebug()==IDebugDraw::DBG_DrawContactPoints)
		//				m_spFont->renderText(L"\"F\" - выключить отладку физики",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);
		//			else
		//				m_spFont->renderText(L"\"F\" - показать AABB физических объектов",math::Rect(1,600-(ll++)*FSIZE,400,400),colGrey);

		if(m_spModelPlane)
			m_spFont->render(L"\"F\" - Выгрузить модель пола",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);
		else
			m_spFont->render(L"\"F\" - Загрузить модель пола",math::Rect(1,600-(ll++)*FSIZE,400,400),colGrey);


		//m_spFont->renderText(L"\"Space\" - сброс",math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);
		if(m_bMove)
			m_spFont->render(L"\"Space\" - выкл движ",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);
		else
			m_spFont->render(L"\"Space\" - вкл движ",math::Rect(1,600-(ll++)*FSIZE,400,400),colGrey);

		m_spFont->render(L"-= Вспомогательные функции =-",math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);

		switch(m_iRenderSpaceTree) {
			case 0:
				m_spFont->render(L"\"T\" - показать аппрокс. объекты",math::Rect(1,600-(ll++)*FSIZE,400,400),colGrey);
				break;

			case 1:
				m_spFont->render(L"\"T\" - Показать финал. объекты",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);
				break;

			case 2:
				m_spFont->render(L"\"T\" - скрыть дерево",math::Rect(1,600-(ll++)*FSIZE,400,400),colGreen);
				break;
		}

		m_spFont->render(L"\"E\" - для изменения",math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);
		StringCchPrintf(szString, 256,L"Скорость: %3d",m_iSpeed*10);
		m_spFont->render(szString,math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);

		StringCchPrintf(szString, 256,L"Уровень теста DTL: %3d",collision::TheCollider::Get().getDTL());
		m_spFont->render(szString,math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);
		m_spFont->render(L"\"+\"/\"-\" - для изменения теста DTL",math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);
		StringCchPrintf(szString, 256,L"Уровень теста DTL(): %3d",collision::TheCollider::Get().getDTL());
		m_spFont->render(szString,math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);
		m_spFont->render(L"\"+\"/\"-\" - для изменения",math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);

		m_spFont->render(L"-= Пространственное дерево =-",math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);

		m_spFont->render(L"Клавиши:",math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite);

		//		static float sfMove = 0;
		//		static bool sbMoveDown = 1;
		//		if(sfMove < 0)
		//			sbMoveDown = true;
		//		else if(sfMove > 600-FSIZE)
		//			sbMoveDown = false;
		//		sfMove += ((sbMoveDown)?1:-1)*40*dt;

		//		m_spFont->renderText(L"Хелп",math::Rect(755,sfMove,400,400),colWhite);
	}
	else
	{
		m_spFont->render(L"\"H\" - вкл помощь",math::Rect(1,600-(ll++)*FSIZE,400,400),colGrey);
	}

	//	const static std::wstring text =	L"Клавиши:\n"
	//										L"\"O\" - загрузка модели\n"
	//										L"\"L\" - вкл/выкл отображения источников света\n"
	//										L"\"K\" - вкл/выкл освещения\n"
	//										L"\"B\" - вкл/выкл отображения AABBox-ов\n"
	//										L"\"G\" - вкл/выкл отображения сетки\n"
	//										L"\"W\" - вкл/выкл wireframe режима\n"
	//										L"Мышь - вращение камерой";
	//	m_spFont->renderText(text,math::Rect(1,480,400,400),0xEEEEEEEE);

	//
	// update/finalize global tree
	scene::TheScene::Get().finalize();

	// debug draw
	if(m_iRenderSpaceTree==2)
	{
		scene::TheScene::Get().draw();
	}
}

//выход из программы
void TestPhysic::onEsc(const input::CButtonEvent&)
{
	core::IApplication::Get()->close();
}

//opening a new file
void TestPhysic::onO(const input::CButtonEvent &e)
{
	if(!e.m_bPress)
		return;

	std::wstring current_dir;
	current_dir.resize(256);
	GetCurrentDirectory( 256, (wchar_t*)current_dir.c_str() );
	std::wstring file; file.resize(256);
	std::wstring fileTitle; fileTitle.resize(256);

	// Set up the OPENFILENAME structure
	OPENFILENAME ofn;
	ofn.lStructSize       = sizeof(OPENFILENAME);
	ofn.hwndOwner         = 0;
	ofn.hInstance         = (HINSTANCE)GetModuleHandle( 0 );
	ofn.lpstrFilter       = L"Model files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";//(wchar_t*)filter.c_str();
	ofn.lpstrCustomFilter = (LPWSTR) NULL;
	ofn.nMaxCustFilter    = 0L;
	ofn.nFilterIndex      = 1L;
	ofn.lpstrFile         = (wchar_t*)file.c_str();
	ofn.nMaxFile          = (DWORD)file.size();
	ofn.lpstrFileTitle    = (wchar_t*)fileTitle.c_str();
	ofn.nMaxFileTitle     = (DWORD)fileTitle.size();
	ofn.lpstrInitialDir   = NULL;
	ofn.lpstrTitle        = L"Open a File";
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = L"*.xml";
	ofn.lCustData         = 0;

	ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{			
		SetCurrentDirectory( (wchar_t*)current_dir.c_str() );
		std::wstring w(ofn.lpstrFileTitle);
		createModel(m_spModel, std::string(w.begin(), w.end()));
	}		
}

void TestPhysic::onL(const input::CButtonEvent& event)
{
	//if(event.m_bPress)
	//	render::TheLightManager::Get().setDebugRendering(!render::TheLightManager::Get().isDebugRendering());
}

void TestPhysic::onK(const input::CButtonEvent& event)
{
	if(event.m_bPress)
		render::TheRenderManager::Get().enableLighting(!render::TheRenderManager::Get().isLightingEnabled());
}

void TestPhysic::onB(const input::CButtonEvent& event)
{
	if(event.m_bPress)
		render::TheRenderManager::Get().enableVolumes(!render::TheRenderManager::Get().isVolumeDrawing());
}

void TestPhysic::onG(const input::CButtonEvent& event)
{
	if(event.m_bPress)
		m_bRenderGrid = !m_bRenderGrid;
}

void TestPhysic::onR(const input::CButtonEvent& event)
{
	if(event.m_bPress)
	{
		int nFillMode = render::TheRenderManager::Get().getFillMode();

		switch(nFillMode)
		{
		case render::Solid:
			render::TheRenderManager::Get().setFillMode(render::WireFrame);
			break;

		case render::WireFrame:
			render::TheRenderManager::Get().setFillMode(render::Solid);
			break;
		};
	}
}

void TestPhysic::onSpace(const input::CButtonEvent& event)
{
	if(event.m_bPress)
		m_bMove = !m_bMove;
}

void TestPhysic::onMinus(const input::CButtonEvent& event)
{
	if(event.m_bPress)
	{
		unsigned dtl = collision::TheCollider::Get().getDTL();
		if(dtl!=0)
			dtl--;
		collision::TheCollider::Get().setDTL(dtl);
	}
}

void TestPhysic::onEquals(const input::CButtonEvent& event)
{
	if(event.m_bPress)
	{
		unsigned dtl = collision::TheCollider::Get().getDTL();
		if(++dtl>100)
			dtl = 100;
		collision::TheCollider::Get().setDTL(dtl);
	}
}

void TestPhysic::onT(const input::CButtonEvent& event)
{
	// testing purpose
	if(event.m_bPress)
	{
		m_iRenderSpaceTree++;
		if(m_iRenderSpaceTree>=3)
			m_iRenderSpaceTree = 0;
	}
};

void TestPhysic::onI(const input::CButtonEvent& event)
{
	//		if(event.m_bPress)
	//			Physic::Get().switchIdle();
};

void TestPhysic::onF(const input::CButtonEvent& event)
{
	if(event.m_bPress)
	{
		if (m_spModelPlane)
		{
			scene::TheScene::Get().getRootFrame()->removeChild(m_spModelPlane);
			m_spModelPlane = 0;
		}
		else
			createModel(m_spModelPlane, "plane");
	}

	//		if(event.m_bPress)
	//			Physic::Get().switchDebug();
};

void TestPhysic::onC(const input::CButtonEvent& event)
{
	if(event.m_bPress)
		m_bUseFPSCamera = !m_bUseFPSCamera;

	math::Vec3f vEyePt		( 0.0f, 0.0f, 0.0f );
	math::Vec3f vLookatPt	( 0.0f, 0.0f, 0.0f );
	math::Vec3f vUpVec		( 0.0f, 0.0f, 0.0f );
	math::Vec3f vZero		( 0.0f, 0.0f, 0.0f );

	if(m_bUseFPSCamera)
	{
		m_spTargetCamera->getPosition(vUpVec,vEyePt,vLookatPt);
        vUpVec.mData[0] = 0.0f;
        vUpVec.mData[1] = 0.0f;
        vUpVec.mData[2] = 1.0f;
		m_spFirstPersonCamera->setPosition(vUpVec,vEyePt,vLookatPt);
		m_spFirstPersonCamera->activate();
	}
	else
	{
		m_spFirstPersonCamera->getPosition(vUpVec,vEyePt,vLookatPt);
		m_spTargetCamera->setPosition(vUpVec,vEyePt,vZero);
		m_spTargetCamera->activate();
	}
}

void TestPhysic::onH(const input::CButtonEvent& event)
{
	if(event.m_bPress)
		m_bShowHelp = !m_bShowHelp;
}

void TestPhysic::onE(const input::CButtonEvent& event)
{
	if(event.m_bPress)
	{
		m_iSpeed++;
		if(m_iSpeed>50)
			m_iSpeed = 1;
	}
}

void TestPhysic::onXAxis(const input::CRelativeAxisEvent &event)
{
	//	if (input::CSystem::Get().GetControl(input::Mouse, input::ButtonRight)->getPress() == false)
	//		return;

	const int accel = 5;
	const float slow = .01f;
	const float fast = 2*slow;
	float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

	if(m_bUseFPSCamera)
		m_spFirstPersonCamera->rotateLeft(-angle);
	else
		m_spTargetCamera->rotateLeft(-angle);
}

void TestPhysic::onYAxis(const input::CRelativeAxisEvent &event)
{
	//	if (input::CSystem::Get().GetControl(input::Mouse, input::ButtonRight)->getPress() == false)
	//		return;

	const int accel = 5;
	const float slow = .01f;
	const float fast = 2*slow;
	float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;
	if(m_bUseFPSCamera)
		m_spFirstPersonCamera->rotateUp(angle);
	else
		m_spTargetCamera->rotateUp(angle);
}

void TestPhysic::onWheelAxis(const input::CRelativeAxisEvent &event)
{
	const float slow = .1f;
	if(m_bUseFPSCamera)
		m_spFirstPersonCamera->goForward(event.m_nDelta*slow);
	else
		m_spTargetCamera->goForward(event.m_nDelta*slow);;
}