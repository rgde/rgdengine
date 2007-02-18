#include "ModelViewer.h"

ModelViewer::ModelViewer() 
{
	m_camera = render::CRenderCamera::Create();
	render::TheCameraManager::Get().addCamera(m_camera);

	render::TheRenderManager::Get().enableVolumes( false );
	render::TheRenderManager::Get().enableLighting(false);

	m_spFont = render::IFont::Create(11, L"Arial", render::IFont::Heavy);

	std::string cmd_line = getCommandLineParams();
	if (cmd_line.size() > 0)
		createModel(cmd_line);
	//
	initCamera();
	////инициализация ввода
	initInput();
	createLights();

	m_bRenderGrid = false;
}


ModelViewer::~ModelViewer()
{
}

void ModelViewer::initInput()
{
	m_cEsc.attachToControl(input::Keyboard, input::KeyEscape);
	m_cO.attachToControl(input::Keyboard, input::KeyO);
	m_cL.attachToControl(input::Keyboard, input::KeyL);
	m_cK.attachToControl(input::Keyboard, input::KeyK);
	m_cB.attachToControl(input::Keyboard, input::KeyB);
	m_cG.attachToControl(input::Keyboard, input::KeyG);
	m_cW.attachToControl(input::Keyboard, input::KeyW);
	m_cXAxis.attachToControl(input::Mouse, input::AxisX);
	m_cYAxis.attachToControl(input::Mouse, input::AxisY);
	m_cZAxis.attachToControl(input::Mouse, input::AxisWheel);
	m_cEsc.addHandler(this,&ModelViewer::onEsc);
	m_cO.addHandler(this,&ModelViewer::onO);
	m_cL.addHandler(this,&ModelViewer::onL);
	m_cK.addHandler(this,&ModelViewer::onK);
	m_cB.addHandler(this,&ModelViewer::onB);
	m_cG.addHandler(this,&ModelViewer::onG);
	m_cW.addHandler(this,&ModelViewer::onW);
	m_cYAxis.addHandler(this,&ModelViewer::onYAxis);
	m_cXAxis.addHandler(this,&ModelViewer::onXAxis);
	m_cZAxis.addHandler(this,&ModelViewer::onWheelAxis);
}

void ModelViewer::initCamera()
{
	math::Vec3f vEyePt( 0.0f, 40, -300 );
	math::Vec3f vLookatPt( 0.0f, 0.0f, 0.0f );
	math::Vec3f vUpVec( 0.0f, 1.0f, 0.0f );
	
	m_camera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
    m_spTargetCamera = math::CTargetCamera::Create(m_camera);
	m_spTargetCamera->setPosition(vUpVec,vEyePt,vLookatPt);
	m_spTargetCamera->activate();
}

void ModelViewer::createModel(const std::string& strFileName)
{
	if (m_spModel)
	{
		scene::TheScene::Get().getRootFrame()->removeChild(m_spModel);
		m_spModel = 0;
		//return;
	}

	std::string file = io::helpers::getFileNameWithoutExtension(strFileName);
	m_spModel = render::CModel::Create(file);

	if(m_spModel)
	{
		scene::TheScene::Get().getRootFrame()->addChild(m_spModel);
		m_spModel->play();
		m_spModel->setLooped( true );
	}
}

void ModelViewer::createLights()
{
	//render::TheLightManager::Get().setAmbientColor(math::Color(255, 255, 255, 255));

	//render::CPointLight *pPointLight = new render::CPointLight("point1");
	//scene::TheScene::Get().getRootFrame()->addChild(pPointLight);
	//pPointLight->setPosition(math::Vec3f(0,40,60));

	//pPointLight->setDiffuse(math::Color(180, 180, 180, 250));
	//pPointLight->setSpecular(math::Color(30, 30, 30, 250));
	//pPointLight->setAmbient(math::Color(0, 0, 0, 0));
	//pPointLight->setRange(5500);
	//pPointLight->setEnabled(true);

	//
	//render::CSpotLight *pSpotLight = new render::CSpotLight("spot1");
	//
	//scene::TheScene::Get().getRootFrame()->addChild(pSpotLight);
	//pSpotLight->setPosition(math::Vec3f(0,-40,-100));

	//pSpotLight->setDiffuse(math::Color(180, 180, 180, 250));
	//pSpotLight->setSpecular(math::Color(30, 30, 30, 250));
	//pSpotLight->setAmbient(math::Color(0, 0, 0, 0));
	//pSpotLight->setRange(5500);
	//pSpotLight->setEnabled(true);
	//pSpotLight->setTheta(0);
	//pSpotLight->setPhi(45.0f/180.0f*3.14f);
	//pSpotLight->setFalloff(1);

	//render::CDirectionalLight *pDirectionalLight = new render::CDirectionalLight("directional1");

	//scene::TheScene::Get().getRootFrame()->addChild(pDirectionalLight);
	//pDirectionalLight->setPosition(math::Vec3f(0,-40,-100));

	//pDirectionalLight->setDiffuse(math::Color(180, 180, 180, 250));
	//pDirectionalLight->setSpecular(math::Color(30, 30, 30, 250));
	//pDirectionalLight->setAmbient(math::Color(0, 0, 0, 0));
	//pDirectionalLight->setEnabled(true);
	//pDirectionalLight->setDirection(math::Vec3f(0, 0, 1));
}


void ModelViewer::update(float dt)
{
	render::TheDevice::Get().showFPS(m_spFont);

	if(m_bRenderGrid){
		render::TheDevice::Get().showWiredFloorGrid(300.0f, 20, math::Color(150, 150, 150, 255));
		render::TheDevice::Get().showWiredFloorGrid(300.0f, 2, math::Color(60, 60, 60, 255));
	}

	const static std::wstring text =	L"Клавиши:\n"
										L"\"O\" - загрузка модели\n"
										L"\"L\" - вкл/выкл отображения источников света\n"
										L"\"K\" - вкл/выкл освещения\n"
										L"\"B\" - вкл/выкл отображения AABBox-ов\n"
										L"\"G\" - вкл/выкл отображения сетки\n"
										L"\"W\" - вкл/выкл wireframe режима\n"
										L"Мышь - вращение камерой";

	m_spFont->render(text,math::Rect(1,480,400,400),0xEEEEEEEE);//,true);
}

//выход из программы
void ModelViewer::onEsc(const input::CButtonEvent&)
{
	core::IApplication::Get()->close();
}

//opening a new file
void ModelViewer::onO(const input::CButtonEvent &e)
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
		createModel(std::string(w.begin(), w.end()));
	}		
}

void ModelViewer::onL(const input::CButtonEvent& event)
{
	//if(event.m_bPress)
	//	render::TheLightManager::Get().setDebugRendering(!render::TheLightManager::Get().isDebugRendering());
}

void ModelViewer::onK(const input::CButtonEvent& event)
{
	if(event.m_bPress)
		render::TheRenderManager::Get().enableLighting(!render::TheRenderManager::Get().isLightingEnabled());
}

void ModelViewer::onB(const input::CButtonEvent& event)
{
	if(event.m_bPress)
		render::TheRenderManager::Get().enableVolumes(!render::TheRenderManager::Get().isVolumeDrawing());
}

void ModelViewer::onG(const input::CButtonEvent& event)
{
	if(event.m_bPress)
		m_bRenderGrid = !m_bRenderGrid;
}

void ModelViewer::onW(const input::CButtonEvent& event)
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

//ось X
void ModelViewer::onXAxis(const input::CRelativeAxisEvent &event)
{
	const int accel = 5;
	const float slow = .01f;
	const float fast = 2*slow;
	float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

	if (input::CSystem::Get().GetControl(input::Mouse, input::ButtonLeft)->getPress())
	{
        math::Vec3f cam_up;
		math::Vec3f cam_pos;
		math::Vec3f cam_target;
		m_spTargetCamera->getPosition(cam_up,cam_pos,cam_target);

		math::Vec3f dir = cam_target - cam_pos;
		math::normalize(dir);
		math::normalize(cam_up);
		math::Vec3f cam_side = math::makeCross(cam_up, dir);
		cam_side *= -event.m_nDelta*0.1f;

		cam_pos += cam_side;
		cam_target += cam_side;
		m_spTargetCamera->setPosition(cam_up,cam_pos,cam_target);

	}
	else if (input::CSystem::Get().GetControl(input::Mouse, input::ButtonRight)->getPress() == false)
		return;
	else
	{
		m_spTargetCamera->rotateLeft(-angle);
	}
}

//ось Y
void ModelViewer::onYAxis(const input::CRelativeAxisEvent &event)
{
	if (input::CSystem::Get().GetControl(input::Mouse, input::ButtonLeft)->getPress())
	{
		math::Vec3f cam_up;
		math::Vec3f cam_pos;
		math::Vec3f cam_target;
		m_spTargetCamera->getPosition(cam_up,cam_pos,cam_target);

		math::Vec3f dir = cam_target - cam_pos;
		math::normalize(dir);
		dir *= event.m_nDelta;

		cam_pos += dir;
		cam_target += dir;
		m_spTargetCamera->setPosition(cam_up,cam_pos,cam_target);
		return;

	}
	else if (input::CSystem::Get().GetControl(input::Mouse, input::ButtonRight)->getPress() == false)
		return;

	const int accel = 5;
	const float slow = .01f;
	const float fast = 2*slow;
	float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

	m_spTargetCamera->rotateUp(angle);
}

/*onWheelAxis*/
void ModelViewer::onWheelAxis(const input::CRelativeAxisEvent &event)
{
	const float slow = .1f;
	m_spTargetCamera->goBackward(-event.m_nDelta*slow);
}