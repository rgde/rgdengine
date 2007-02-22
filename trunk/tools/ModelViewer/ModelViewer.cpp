#include <windows.h>
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

    {
        using namespace input;

        //создадим команды
        CInput::addCommand(L"Esc");
        CInput::addCommand(L"O");
        CInput::addCommand(L"L");
        CInput::addCommand(L"K");
        CInput::addCommand(L"B");
        CInput::addCommand(L"G");
        CInput::addCommand(L"W");
        CInput::addCommand(L"MouseLeft");
        CInput::addCommand(L"MouseRight");
        CInput::addCommand(L"XAxis");
        CInput::addCommand(L"YAxis");
        CInput::addCommand(L"ZAxis");

        //связываем команды с контролами
        CInput::getDevice(Keyboard)->getControl(KeyEscape  )->bind(L"Esc");
        CInput::getDevice(Keyboard)->getControl(KeyO       )->bind(L"O");
        CInput::getDevice(Keyboard)->getControl(KeyL       )->bind(L"L");
        CInput::getDevice(Keyboard)->getControl(KeyK       )->bind(L"K");
        CInput::getDevice(Keyboard)->getControl(KeyB       )->bind(L"B");
        CInput::getDevice(Keyboard)->getControl(KeyG       )->bind(L"G");
        CInput::getDevice(Keyboard)->getControl(KeyW       )->bind(L"W");
        CInput::getDevice(Mouse   )->getControl(ButtonLeft )->bind(L"MouseLeft");
        CInput::getDevice(Mouse   )->getControl(ButtonRight)->bind(L"MouseRight");
        CInput::getDevice(Mouse   )->getControl(AxisX      )->bind(L"XAxis");
        CInput::getDevice(Mouse   )->getControl(AxisY      )->bind(L"YAxis");
        CInput::getDevice(Mouse   )->getControl(AxisWheel  )->bind(L"ZAxis");
    }

    //биндим хелперы с командами
    m_cEsc       .attach(L"Esc");
    m_cO         .attach(L"O");
    m_cL         .attach(L"L");
    m_cK         .attach(L"K");
    m_cB         .attach(L"B");
    m_cG         .attach(L"G");
    m_cW         .attach(L"W");
    m_cMouseLeft .attach(L"MouseLeft");
    m_cMouseRight.attach(L"MouseRight");
    m_cXAxis     .attach(L"XAxis");
    m_cYAxis     .attach(L"YAxis");
    m_cZAxis     .attach(L"ZAxis");

    //задаем для команд функции-обработчики
    m_cEsc   += boost::bind(&ModelViewer::onEsc,       this);
    m_cO     += boost::bind(&ModelViewer::onO,         this);
    m_cL     += boost::bind(&ModelViewer::onL,         this);
    m_cK     += boost::bind(&ModelViewer::onK,         this);
    m_cB     += boost::bind(&ModelViewer::onB,         this);
    m_cG     += boost::bind(&ModelViewer::onG,         this);
    m_cW     += boost::bind(&ModelViewer::onW,         this);
    m_cXAxis += boost::bind(&ModelViewer::onXAxis,     this, _1);
    m_cYAxis += boost::bind(&ModelViewer::onYAxis,     this, _1);
    m_cZAxis += boost::bind(&ModelViewer::onWheelAxis, this, _1);
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
void ModelViewer::onEsc()
{
	core::IApplication::Get()->close();
}

//opening a new file
void ModelViewer::onO()
{
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

void ModelViewer::onL()
{
    //render::TheLightManager::Get().setDebugRendering(!render::TheLightManager::Get().isDebugRendering());
}

void ModelViewer::onK()
{
    render::TheRenderManager::Get().enableLighting(!render::TheRenderManager::Get().isLightingEnabled());
}

void ModelViewer::onB()
{
    render::TheRenderManager::Get().enableVolumes(!render::TheRenderManager::Get().isVolumeDrawing());
}

void ModelViewer::onG()
{
    m_bRenderGrid = !m_bRenderGrid;
}

void ModelViewer::onW()
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

//ось X
void ModelViewer::onXAxis(int dx)
{
	const int accel = 5;
	const float slow = .01f;
	const float fast = 2*slow;
	float angle = dx>accel ? dx*fast : dx*slow;

	if (m_cMouseLeft)
	{
        math::Vec3f cam_up;
		math::Vec3f cam_pos;
		math::Vec3f cam_target;
		m_spTargetCamera->getPosition(cam_up,cam_pos,cam_target);

		math::Vec3f dir = cam_target - cam_pos;
		math::normalize(dir);
		math::normalize(cam_up);
		math::Vec3f cam_side = math::makeCross(cam_up, dir);
		cam_side *= -dx*0.1f;

		cam_pos += cam_side;
		cam_target += cam_side;
		m_spTargetCamera->setPosition(cam_up,cam_pos,cam_target);

	}
	else if (!m_cMouseRight)
		return;
	else
	{
		m_spTargetCamera->rotateLeft(-angle);
	}
}

//ось Y
void ModelViewer::onYAxis(int dy)
{
	if (m_cMouseLeft)
	{
		math::Vec3f cam_up;
		math::Vec3f cam_pos;
		math::Vec3f cam_target;
		m_spTargetCamera->getPosition(cam_up,cam_pos,cam_target);

		math::Vec3f dir = cam_target - cam_pos;
		math::normalize(dir);
		dir *= dy;

		cam_pos += dir;
		cam_target += dir;
		m_spTargetCamera->setPosition(cam_up,cam_pos,cam_target);
		return;

	}
	else if (!m_cMouseRight)
		return;

	const int accel = 5;
	const float slow = .01f;
	const float fast = 2*slow;
	float angle = dy>accel ? dy*fast : dy*slow;

	m_spTargetCamera->rotateUp(angle);
}

//колесико мыши
void ModelViewer::onWheelAxis(int dz)
{
	const float slow = .1f;
	m_spTargetCamera->goBackward(-dz*slow);
}