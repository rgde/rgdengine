//TestCamera.cpp
#include "engine.h"
#include "TestCamera.h"

TestCamera::TestCamera()
{
    m_nCameraType = 0; //Fly Camera

    m_spCamera = render::CRenderCamera::Create();
    m_spFlyCamera = math::CFlyCamera::Create(m_spCamera);
    m_spTargetCamera = math::CTargetCamera::Create(m_spCamera);
    m_spFirstPersonCamera = math::CFirstPersonCamera::Create(m_spCamera);

    render::TheCameraManager::Get().addCamera(m_spCamera);
    render::TheRenderManager::Get().enableVolumes(false);
    render::TheRenderManager::Get().enableLighting(true);

    m_spFont = render::IFont::Create(11, L"Arial", render::IFont::Heavy);

    initInput();
    initCamera();
    initLights();

    m_spModel = render::CModel::Create("test_scene");
}

TestCamera::~TestCamera()
{
}

void TestCamera::initInput()
{
    {
        using namespace input;

        //создадим команды
        CInput::addCommand(L"Quit");
        CInput::addCommand(L"Wireframe");
        CInput::addCommand(L"Reset");
        CInput::addCommand(L"Change");
        CInput::addCommand(L"Froward");
        CInput::addCommand(L"Backward");
        CInput::addCommand(L"CW");
        CInput::addCommand(L"CCW");
        CInput::addCommand(L"Up");
        CInput::addCommand(L"Down");
        CInput::addCommand(L"StarfeLeft");
        CInput::addCommand(L"StarfeRight");
        CInput::addCommand(L"Horz");
        CInput::addCommand(L"Vert");

        //связываем команды с контролами
        CInput::getDevice(Keyboard)->getControl(KeyEscape)->bind(L"Quit");
        CInput::getDevice(Keyboard)->getControl(KeySpace )->bind(L"Wireframe");
        CInput::getDevice(Keyboard)->getControl(KeyR     )->bind(L"Reset");
        CInput::getDevice(Keyboard)->getControl(KeyTab   )->bind(L"Change");
        CInput::getDevice(Keyboard)->getControl(KeyW     )->bind(L"Froward");
        CInput::getDevice(Keyboard)->getControl(KeyS     )->bind(L"Backward");
        CInput::getDevice(Keyboard)->getControl(KeyE     )->bind(L"CW");
        CInput::getDevice(Keyboard)->getControl(KeyQ     )->bind(L"CCW");
        CInput::getDevice(Keyboard)->getControl(KeyF     )->bind(L"Up");
        CInput::getDevice(Keyboard)->getControl(KeyC     )->bind(L"Down");
        CInput::getDevice(Keyboard)->getControl(KeyA     )->bind(L"StarfeLeft");
        CInput::getDevice(Keyboard)->getControl(KeyD     )->bind(L"StarfeRight");
        CInput::getDevice(Mouse   )->getControl(AxisX    )->bind(L"Horz");
        CInput::getDevice(Mouse   )->getControl(AxisY    )->bind(L"Vert");
    }

    //биндим хелперы с командами
    m_cEsc  .attach(L"Quit");
    m_cSpace.attach(L"Wireframe");
    m_cR    .attach(L"Reset");
    m_cTab  .attach(L"Change");
    m_cW    .attach(L"Froward");
    m_cS    .attach(L"Backward");
    m_cE    .attach(L"CW");
    m_cQ    .attach(L"CCW");
    m_cF    .attach(L"Up");
    m_cC    .attach(L"Down");
    m_cA    .attach(L"StarfeLeft");
    m_cD    .attach(L"StarfeRight");
    m_cXAxis.attach(L"Horz");
    m_cYAxis.attach(L"Vert");

    //задаем для команд функции-обработчики
    m_cEsc   += boost::bind(&TestCamera::onEsc,          this);
    m_cSpace += boost::bind(&TestCamera::onWireframe,    this);
    m_cR     += boost::bind(&TestCamera::onReset,        this);
    m_cTab   += boost::bind(&TestCamera::onChangeCamera, this);
    m_cYAxis += boost::bind(&TestCamera::onYAxis, this, _1);
    m_cXAxis += boost::bind(&TestCamera::onXAxis, this, _1);
}

void TestCamera::initCamera()
{
    math::Vec3f vUpVec    ( 0.0f,   0.0f, 1.0f );
    math::Vec3f vEyePt    ( 0.0f, 300.0f, 0.0f );
    math::Vec3f vLookatPt ( 0.0f,   0.0f, 0.0f );

    //m_spCamera->setProjection(math::Math::PI/4, 1.0f, 1.0f, 10000.0f);
    m_spCamera->setProjection(math::Math::PI/4, 4.f/3.f, 1.0f, 10000.0f);

    m_spFlyCamera->setPosition(vUpVec,vEyePt,vLookatPt);
    m_spTargetCamera->setPosition(vUpVec,vEyePt,vLookatPt);
    m_spFirstPersonCamera->setPosition(vUpVec,vEyePt,vLookatPt);

    m_spFlyCamera->activate();
}

void TestCamera::initLights()
{
    //render::TheLightManager::Get().setAmbientColor(math::Color(20, 20, 20, 255));

    //render::CPointLight *pPointLight = new render::CPointLight("point1");
    //scene::TheScene::Get().getRootFrame()->addChild(pPointLight);
    //pPointLight->setPosition(math::Vec3f(0,40,60));

    //pPointLight->setDiffuse(math::Color(180, 180, 180, 250));
    //pPointLight->setSpecular(math::Color(30, 30, 30, 250));
    //pPointLight->setAmbient(math::Color(0, 0, 0, 0));
    //pPointLight->setRange(5500);
    //pPointLight->setEnabled(true);
}

void TestCamera::update(float dt)
{
    //фпс
    render::TheDevice::Get().showFPS(m_spFont);

    //справка
    std::wstring text;
    switch (m_nCameraType)
    {
    case 0:
        text =
        L"ESC - Выход\n"
        L"SPACE - wireframe вкл/выкл"
        L"TAB - Переключить камеру\n"
        L"R - Сбросить программу в начальное состояние\n"
        L"\n"
        L"W - Вперед\n"
        L"S - Назад\n"
        L"A - Влево\n"
        L"D - Вправо\n"
        L"Q - Поворот против часовой стрелки\n"
        L"E - Поворот по часовой стрелке\n"
        L"F - Вверх\n"
        L"C - Вниз\n";
        break;
    case 1:
        text =
        L"ESC - Выход\n"
        L"SPACE - wireframe вкл/выкл"
        L"TAB - Переключить камеру\n"
        L"R - Сбросить программу в начальное состояние\n"
        L"\n"
        L"W - Приблизить\n"
        L"S - Удалить\n"
        L"\n"
        L"\n"
        L"Q - Поворот против часовой стрелки\n"
        L"E - Поворот по часовой стрелке\n"
        L"\n"
        L"\n";
        break;
    case 2:
        text =
        L"ESC - Выход\n"
        L"SPACE - wireframe вкл/выкл"
        L"TAB - Переключить камеру\n"
        L"R - Сбросить программу в начальное состояние\n"
        L"\n"
        L"W - Вперед\n"
        L"S - Назад\n"
        L"A - Влево\n"
        L"D - Вправо\n"
        L"Q - Поворот против часовой стрелки\n"
        L"E - Поворот по часовой стрелке\n"
        L"F - Вверх\n"
        L"C - Вниз\n";
        break;
    }
    m_spFont->render(text,math::Rect(1,416,400,400),0xFFFFFFFF,false);

    //текущая камера
    std::wstring camera;
    switch (m_nCameraType)
    {
    case 0: camera = L"Fly Camera"; break;
    case 1: camera = L"Target Camera"; break;
    case 2: camera = L"First Person Camera"; break;
    }
    m_spFont->render(camera,math::Rect(1,16,400,400),0xFFFF0000,false);

    //движение
    switch (m_nCameraType)
    {
    //Fly Camera
    case 0:
        {
            if(m_cE)
                m_spFlyCamera->rotateCW(dt*2.0f);
            if(m_cQ)
                m_spFlyCamera->rotateCCW(dt*2.0f);
            if(m_cW)
                m_spFlyCamera->goForward(dt*100.0f);
            if(m_cS)
                m_spFlyCamera->goBackward(dt*100.0f);
            if(m_cF)
                m_spFlyCamera->goUp(dt*100.0f);
            if(m_cC)
                m_spFlyCamera->goDown(dt*100.0f);
            if(m_cA)
                m_spFlyCamera->goLeft(dt*100.0f);
            if(m_cD)
                m_spFlyCamera->goRight(dt*100.0f);
            break;
        }
    //Target Camera
    case 1:
        {
            if(m_cE)
                m_spTargetCamera->rotateCW(dt*2.0f);
            if(m_cQ)
                m_spTargetCamera->rotateCCW(dt*2.0f);
            if(m_cW)
                m_spTargetCamera->goForward(dt*100.0f);
            if(m_cS)
                m_spTargetCamera->goBackward(dt*100.0f);
            break;
        }
    //First Person Camera
    case 2:
        {
            if(m_cW)
                m_spFirstPersonCamera->goForward(dt*100.0f);
            if(m_cS)
                m_spFirstPersonCamera->goBackward(dt*100.0f);
            if(m_cF)
                m_spFirstPersonCamera->goUp(dt*100.0f);
            if(m_cC)
                m_spFirstPersonCamera->goDown(dt*100.0f);
            if(m_cA)
                m_spFirstPersonCamera->goLeft(dt*100.0f);
            if(m_cD)
                m_spFirstPersonCamera->goRight(dt*100.0f);
            break;
        }
    }
}

void TestCamera::onEsc()
{
    core::IApplication::Get()->close();
}

void TestCamera::onWireframe()
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

void TestCamera::onReset()
{
    m_nCameraType = 0; //Fly Camera
    initCamera();
}

void TestCamera::onChangeCamera ()
{
    const int nCameras = 3; //Fly, Target, First Person
    m_nCameraType = (m_nCameraType+1) % nCameras;

    switch (m_nCameraType)
    {
    case 0: m_spFlyCamera->activate(); break;
    case 1: m_spTargetCamera->activate(); break;
    case 2: m_spFirstPersonCamera->activate(); break;
    }
}

void TestCamera::onXAxis(int dx)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = dx>accel ? dx*fast : dx*slow;

    switch(m_nCameraType)
    {
    case 0: m_spFlyCamera->rotateLeft(-angle); break;
    case 1: m_spTargetCamera->rotateLeft(-angle); break;
    case 2: m_spFirstPersonCamera->rotateLeft(-angle); break;
    }
}

void TestCamera::onYAxis(int dy)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = dy>accel ? dy*fast : dy*slow;

    switch(m_nCameraType)
    {
    case 0: m_spFlyCamera->rotateUp(angle); break;
    case 1: m_spTargetCamera->rotateUp(angle); break;
    case 2: m_spFirstPersonCamera->rotateUp(angle); break;
    }
}