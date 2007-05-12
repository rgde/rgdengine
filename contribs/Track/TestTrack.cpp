//TestTrack.cpp
#include "engine.h"
#include "TestTrack.h"

TestTrack::TestTrack()
{
    m_spCamera = render::CRenderCamera::Create();
    m_spTargetCamera = math::CTargetCamera::Create(m_spCamera);

    render::TheCameraManager::Get().addCamera(m_spCamera);
    render::TheRenderManager::Get().enableVolumes(false);
    render::TheRenderManager::Get().enableLighting(true);

    m_spFont = render::IFont::Create(11, L"Arial", render::IFont::Heavy);

    initInput();
    initCamera();
    initLights();

    //создаем трек
    math::CTrack::Key key;

    key.m_fSpeed = 1.f;
    key.m_vEyePt = math::Vec3f(0.f,0.f,0.f);
    m_track.m_values.push_back(key);

    key.m_fSpeed = 10.f;
    key.m_vEyePt = math::Vec3f(20.f,0.f,0.f);
    m_track.m_values.push_back(key);

    key.m_fSpeed = 1.f;
    key.m_vEyePt = math::Vec3f(0.f,20.f,-20.f);
    m_track.m_values.push_back(key);

    m_track.apply();
}

TestTrack::~TestTrack()
{
}

void TestTrack::initInput()
{
    {
        using namespace input;

        //создадим команды
		//биндим хелперы с командами
		m_cEsc  .attach(L"Quit");
		m_cR    .attach(L"Reset");
		m_cW    .attach(L"Froward");
		m_cS    .attach(L"Backward");
		m_cE    .attach(L"CW");
		m_cQ    .attach(L"CCW");
		m_cXAxis.attach(L"Horz");
		m_cYAxis.attach(L"Vert");

        //связываем команды с контролами
		Input::getDevice(types::Keyboard)->getControl(types::KeyEscape)->bind(L"Quit");
        Input::getDevice(types::Keyboard)->getControl(types::KeyR     )->bind(L"Reset");
        Input::getDevice(types::Keyboard)->getControl(types::KeyW     )->bind(L"Froward");
        Input::getDevice(types::Keyboard)->getControl(types::KeyS     )->bind(L"Backward");
        Input::getDevice(types::Keyboard)->getControl(types::KeyE     )->bind(L"CW");
        Input::getDevice(types::Keyboard)->getControl(types::KeyQ     )->bind(L"CCW");
        Input::getDevice(types::Mouse   )->getControl(types::AxisX    )->bind(L"Horz");
        Input::getDevice(types::Mouse   )->getControl(types::AxisY    )->bind(L"Vert");
    }

    //задаем для команд функции-обработчики
    m_cEsc   += boost::bind(&TestTrack::onEsc,   this);
    m_cR     += boost::bind(&TestTrack::onReset, this);
    m_cYAxis += boost::bind(&TestTrack::onYAxis, this, _1);
    m_cXAxis += boost::bind(&TestTrack::onXAxis, this, _1);
}

void TestTrack::initCamera()
{
    math::Vec3f vUpVec    ( 0.0f,   0.0f, 1.0f );
    math::Vec3f vEyePt    ( 0.0f, 100.0f, 0.0f );
    math::Vec3f vLookatPt ( 0.0f,   0.0f, 0.0f );

    m_spCamera->setProjection(math::Math::PI/4, 4.f/3.f, 1.0f, 10000.0f);

    m_spTargetCamera->setPosition(vUpVec,vEyePt,vLookatPt);
    m_spTargetCamera->activate();
}

void TestTrack::initLights()
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

void TestTrack::update(float dt)
{
    //фпс
    render::TheDevice::Get().showFPS(m_spFont);

    //коментарий
    std::wstring comment = L"Не смейтесь - это трек. Просто он нарисован в статике.";
    m_spFont->render(comment,math::Rect(1,26,400,400),0xFF00FF00,false);

    //справка
    std::wstring text =
        L"ESC - Выход\n"
        L"R - Сбросить программу в начальное состояние\n"
        L"\n"
        L"W - Приблизить\n"
        L"S - Удалить\n"
        L"Q - Поворот против часовой стрелки\n"
        L"E - Поворот по часовой стрелке\n";
    m_spFont->render(text,math::Rect(1,416,400,400),0xFFFFFFFF,false);

    //трек
    float t=0;
    const float step = m_track.getTotalTime()/100;
    while (t<m_track.getTotalTime())
    {
        math::CTrack::Key key1 = m_track.getKey(t);
        math::CTrack::Key key2 = m_track.getKey(t+step);

        math::Vec3f v1 = key1.m_vEyePt;
        math::Vec3f v2 = key2.m_vEyePt;

        t += 2*step;
        render::Line3dManager::Get().addLine(v1,v2);
    }
 
    //бокс
    render::Line3dManager::Get().addBox(math::MAT_IDENTITY44F, math::Vec3f(20.f,20.f,20.f), 0xffff0000);

    //движение
    if(m_cE)
        m_spTargetCamera->rotateCW(dt*2.0f);
    if(m_cQ)
        m_spTargetCamera->rotateCCW(dt*2.0f);
    if(m_cW)
        m_spTargetCamera->goForward(dt*100.0f);
    if(m_cS)
        m_spTargetCamera->goBackward(dt*100.0f);
}

void TestTrack::onEsc()
{
    core::IApplication::Get()->close();
}

void TestTrack::onReset()
{
    initCamera();
}

void TestTrack::onXAxis(int dx)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = dx>accel ? dx*fast : dx*slow;

    m_spTargetCamera->rotateLeft(-angle);
}

void TestTrack::onYAxis(int dy)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = dy>accel ? dy*fast : dy*slow;

    m_spTargetCamera->rotateUp(angle);
}