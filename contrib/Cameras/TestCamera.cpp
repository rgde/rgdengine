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
    //привязываем команды к контролам
    m_cEsc  .attachToControl(input::Keyboard, input::KeyEscape); //ESC - выход
    m_cR    .attachToControl(input::Keyboard, input::KeyR);
    m_cTab  .attachToControl(input::Keyboard, input::KeyTab);
    
    m_cW    .attachToControl(input::Keyboard, input::KeyW);
    m_cS    .attachToControl(input::Keyboard, input::KeyS);
    m_cE    .attachToControl(input::Keyboard, input::KeyE);
    m_cQ    .attachToControl(input::Keyboard, input::KeyQ);
    m_cF    .attachToControl(input::Keyboard, input::KeyF);
    m_cC    .attachToControl(input::Keyboard, input::KeyC);
    m_cA    .attachToControl(input::Keyboard, input::KeyA);
    m_cD    .attachToControl(input::Keyboard, input::KeyD);
    m_cXAxis.attachToControl(input::Mouse, input::AxisX);
    m_cYAxis.attachToControl(input::Mouse, input::AxisY);

    //задаем для команд функции-обработчики
    m_cEsc  .addHandler(this,&TestCamera::onEsc);
    m_cR    .addHandler(this,&TestCamera::onReset);
    m_cTab  .addHandler(this,&TestCamera::onChangeCamera);
    m_cYAxis.addHandler(this,&TestCamera::onYAxis);
    m_cXAxis.addHandler(this,&TestCamera::onXAxis);
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

void TestCamera::onEsc(const input::CButtonEvent &event)
{
    if (event.m_bPress)
        core::IApplication::Get()->close();
}

void TestCamera::onReset(const input::CButtonEvent &event)
{
    if (event.m_bPress)
    {
        m_nCameraType = 0; //Fly Camera

        initCamera();
    }
}

void TestCamera::onChangeCamera (const input::CButtonEvent &event)
{
    if (event.m_bPress)
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
}

void TestCamera::onXAxis(const input::CRelativeAxisEvent &event)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

    switch(m_nCameraType)
    {
    case 0: m_spFlyCamera->rotateLeft(-angle); break;
    case 1: m_spTargetCamera->rotateLeft(-angle); break;
    case 2: m_spFirstPersonCamera->rotateLeft(-angle); break;
    }
}

void TestCamera::onYAxis(const input::CRelativeAxisEvent &event)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

    switch(m_nCameraType)
    {
    case 0: m_spFlyCamera->rotateUp(angle); break;
    case 1: m_spTargetCamera->rotateUp(angle); break;
    case 2: m_spFirstPersonCamera->rotateUp(angle); break;
    }
}