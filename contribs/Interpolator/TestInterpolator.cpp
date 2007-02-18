//TestInterpolator.cpp
#include "engine.h"
#include "TestInterpolator.h"

TestInterpolator::TestInterpolator()
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

    //m_spModel = render::CModel::Create("test_scene");

    m_spline.m_values.push_back(math::Vec3f(  0,  0,  0));
    m_spline.m_values.push_back(math::Vec3f( 20,  0,  0));
    m_spline.m_values.push_back(math::Vec3f(  0, 20,-20));
    m_spline.m_values.push_back(math::Vec3f(-20,  0, 20));
    m_spline.m_values.push_back(math::Vec3f(  0,-20,  0));
    m_spline.m_values.push_back(math::Vec3f(-20,  0,  0));
    m_spline.apply();
}

TestInterpolator::~TestInterpolator()
{
}

void TestInterpolator::initInput()
{
    //привязываем команды к контролам
    m_cEsc  .attachToControl(input::Keyboard, input::KeyEscape); //ESC - выход
    m_cR    .attachToControl(input::Keyboard, input::KeyR);
    
    m_cW    .attachToControl(input::Keyboard, input::KeyW);
    m_cS    .attachToControl(input::Keyboard, input::KeyS);
    m_cE    .attachToControl(input::Keyboard, input::KeyE);
    m_cQ    .attachToControl(input::Keyboard, input::KeyQ);
    m_cXAxis.attachToControl(input::Mouse, input::AxisX);
    m_cYAxis.attachToControl(input::Mouse, input::AxisY);

    //задаем для команд функции-обработчики
    m_cEsc  .addHandler(this,&TestInterpolator::onEsc);
    m_cR    .addHandler(this,&TestInterpolator::onReset);
    m_cYAxis.addHandler(this,&TestInterpolator::onYAxis);
    m_cXAxis.addHandler(this,&TestInterpolator::onXAxis);
}

void TestInterpolator::initCamera()
{
    math::Vec3f vUpVec    ( 0.0f,   0.0f, 1.0f );
    math::Vec3f vEyePt    ( 0.0f, 100.0f, 0.0f );
    math::Vec3f vLookatPt ( 0.0f,   0.0f, 0.0f );

    m_spCamera->setProjection(math::Math::PI/4, 4.f/3.f, 1.0f, 10000.0f);

    m_spTargetCamera->setPosition(vUpVec,vEyePt,vLookatPt);
    m_spTargetCamera->activate();
}

void TestInterpolator::initLights()
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

void TestInterpolator::update(float dt)
{
    //фпс
    render::TheDevice::Get().showFPS(m_spFont);

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

    //сплайн
    float t=0;
    const float step = m_spline.length()/100;
    while (t<m_spline.length())
    {
        math::Vec3f v1 = m_spline(t);
        math::Vec3f v2 = m_spline(t+step);
        t += 2*step;
        render::Line3dManager::Get().addLine(v1,v2);
    }

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

void TestInterpolator::onEsc(const input::CButtonEvent &event)
{
    if (event.m_bPress)
        core::IApplication::Get()->close();
}

void TestInterpolator::onReset(const input::CButtonEvent &event)
{
    if (event.m_bPress)
        initCamera();
}

void TestInterpolator::onXAxis(const input::CRelativeAxisEvent &event)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

    m_spTargetCamera->rotateLeft(-angle);
}

void TestInterpolator::onYAxis(const input::CRelativeAxisEvent &event)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

    m_spTargetCamera->rotateUp(angle);
}