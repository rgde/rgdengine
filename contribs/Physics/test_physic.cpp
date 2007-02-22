#pragma warning(disable : 4244) // type conversion warning.
#pragma warning(disable : 4996) // '_function_' was declared deprecated

#include <windows.h>
#include "test_physic.h"
#include "strsafe.h"

#define FSIZE 11.0f

TestPhysic::TestPhysic() 
{
    m_spCamera = render::CRenderCamera::Create();
    m_spTargetCamera = math::CTargetCamera::Create(m_spCamera);
    m_spFirstPersonCamera = math::CFirstPersonCamera::Create(m_spCamera);
    render::TheCameraManager::Get().addCamera(m_spCamera);

    render::TheRenderManager::Get().enableVolumes( false );
    render::TheRenderManager::Get().enableLighting(false);

    m_spFont = render::IFont::Create(FSIZE, L"Arial", render::IFont::Heavy);

    initCamera();
    initInput();
    initLights();

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
    {
        using namespace input;

        //создадим команды
        CInput::addCommand(L"Up");
        CInput::addCommand(L"Down");
        CInput::addCommand(L"Left");
        CInput::addCommand(L"Right");
        CInput::addCommand(L"Escape");
        CInput::addCommand(L"Q");
        CInput::addCommand(L"L");
        CInput::addCommand(L"K");
        CInput::addCommand(L"B");
        CInput::addCommand(L"G");
        CInput::addCommand(L"R");
        CInput::addCommand(L"W");
        CInput::addCommand(L"A");
        CInput::addCommand(L"S");
        CInput::addCommand(L"D");
        CInput::addCommand(L"C");
        CInput::addCommand(L"T");
        CInput::addCommand(L"I");
        CInput::addCommand(L"F");
        CInput::addCommand(L"H");
        CInput::addCommand(L"E");
        CInput::addCommand(L"Space");
        CInput::addCommand(L"Minus");
        CInput::addCommand(L"Equals");
        CInput::addCommand(L"AxisX");
        CInput::addCommand(L"AxisY");
        CInput::addCommand(L"AxisWhell");

        //связываем команды с контролами
        CInput::getDevice(Keyboard)->getControl(KeyUp    )->bind(L"Up");
        CInput::getDevice(Keyboard)->getControl(KeyDown  )->bind(L"Down");
        CInput::getDevice(Keyboard)->getControl(KeyLeft  )->bind(L"Left");
        CInput::getDevice(Keyboard)->getControl(KeyRight )->bind(L"Right");
        CInput::getDevice(Keyboard)->getControl(KeyEscape)->bind(L"Escape");
        CInput::getDevice(Keyboard)->getControl(KeyO     )->bind(L"Q");
        CInput::getDevice(Keyboard)->getControl(KeyL     )->bind(L"L");
        CInput::getDevice(Keyboard)->getControl(KeyK     )->bind(L"K");
        CInput::getDevice(Keyboard)->getControl(KeyB     )->bind(L"B");
        CInput::getDevice(Keyboard)->getControl(KeyG     )->bind(L"G");
        CInput::getDevice(Keyboard)->getControl(KeyR     )->bind(L"R");
        CInput::getDevice(Keyboard)->getControl(KeyW     )->bind(L"W");
        CInput::getDevice(Keyboard)->getControl(KeyA     )->bind(L"A");
        CInput::getDevice(Keyboard)->getControl(KeyS     )->bind(L"S");
        CInput::getDevice(Keyboard)->getControl(KeyD     )->bind(L"D");
        CInput::getDevice(Keyboard)->getControl(KeyC     )->bind(L"C");
        CInput::getDevice(Keyboard)->getControl(KeyT     )->bind(L"T");
        CInput::getDevice(Keyboard)->getControl(KeyI     )->bind(L"I");
        CInput::getDevice(Keyboard)->getControl(KeyF     )->bind(L"F");
        CInput::getDevice(Keyboard)->getControl(KeyH     )->bind(L"H");
        CInput::getDevice(Keyboard)->getControl(KeyE     )->bind(L"E");
        CInput::getDevice(Keyboard)->getControl(KeySpace )->bind(L"Space");
        CInput::getDevice(Keyboard)->getControl(KeyMinus )->bind(L"Minus");
        CInput::getDevice(Keyboard)->getControl(KeyEquals)->bind(L"Equals");
        CInput::getDevice(Mouse   )->getControl(AxisX    )->bind(L"AxisX");
        CInput::getDevice(Mouse   )->getControl(AxisY    )->bind(L"AxisY");
        CInput::getDevice(Mouse   )->getControl(AxisWheel)->bind(L"AxisWhell");
    }

    m_cUp    .attach(L"Up");
    m_cDown  .attach(L"Down");
    m_cLeft  .attach(L"Left");
    m_cRight .attach(L"Right");
    m_cEsc   .attach(L"Escape");
    m_cO     .attach(L"Q");
    m_cL     .attach(L"L");
    m_cK     .attach(L"K");
    m_cB     .attach(L"B");
    m_cG     .attach(L"G");
    m_cR     .attach(L"R");
    m_cW     .attach(L"W");
    m_cA     .attach(L"A");
    m_cS     .attach(L"S");
    m_cD     .attach(L"D");
    m_cC     .attach(L"C");
    m_cT     .attach(L"T");
    m_cI     .attach(L"I");
    m_cF     .attach(L"F");
    m_cH     .attach(L"H");
    m_cE     .attach(L"E");
    m_cSpace .attach(L"Space");
    m_cMinus .attach(L"Minus");
    m_cEquals.attach(L"Equals");
    m_cXAxis .attach(L"AxisX");
    m_cYAxis .attach(L"AxisY");
    m_cZAxis .attach(L"AxisWhell");
    
    m_cEsc	  += boost::bind(&TestPhysic::onEsc,    this);
    m_cO	  += boost::bind(&TestPhysic::onO,      this);
    m_cL	  += boost::bind(&TestPhysic::onL,      this);
    m_cK	  += boost::bind(&TestPhysic::onK,      this);
    m_cB	  += boost::bind(&TestPhysic::onB,      this);
    m_cG	  += boost::bind(&TestPhysic::onG,      this);
    m_cR	  += boost::bind(&TestPhysic::onR,      this);
    m_cC	  += boost::bind(&TestPhysic::onC,      this);
    m_cT	  += boost::bind(&TestPhysic::onT,      this);
    m_cI	  += boost::bind(&TestPhysic::onI,      this);
    m_cF	  += boost::bind(&TestPhysic::onF,      this);
    m_cH	  += boost::bind(&TestPhysic::onH,      this);
    m_cE	  += boost::bind(&TestPhysic::onE,      this);
    m_cSpace  += boost::bind(&TestPhysic::onSpace,  this);
    m_cMinus  += boost::bind(&TestPhysic::onMinus,  this);
    m_cEquals += boost::bind(&TestPhysic::onEquals, this);

    m_cYAxis += boost::bind(&TestPhysic::onYAxis,     this, _1);
    m_cXAxis += boost::bind(&TestPhysic::onXAxis,     this, _1);
    m_cZAxis += boost::bind(&TestPhysic::onWheelAxis, this, _1);
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

void TestPhysic::initLights()
{
    //render::TheLightManager::Get().setAmbientColor(math::Color(20, 20, 20, 255));
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

void TestPhysic::update(float dt)
{
    if(dt>0.3f)
        dt = 0.3f;

    WCHAR szString[256];
    math::Vec3f pos, pos_, trg, up;

    // ul - upper left (1 -FPS, 2 -dbg tris,3 - skip), ll - lower left (1- first pos)
    int ul       = 3,
        ll       = 1,
        colWhite = 0xFFFFFFFF,
        colRed   = 0xFFFF0000,
        colGrey  = 0xEEEEEEEE,
        colGreen = 0xEE00FF00;

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

    pos = m_spCamera.get()->getGlobalPosition();
    StringCchPrintf(szString, 256,L"Camera pos 2: %3.1f:%3.1f:%3.1f",pos[0],pos[1],pos[2]);
    m_spFont->render(szString,math::Rect(1,600-(ll++)*FSIZE,400,400),colWhite,true);

    //update/approximate global tree
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
void TestPhysic::onEsc()
{
    core::IApplication::Get()->close();
}

//opening a new file
void TestPhysic::onO()
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
        createModel(m_spModel, std::string(w.begin(), w.end()));
    }		
}

void TestPhysic::onL()
{
    //render::TheLightManager::Get().setDebugRendering(!render::TheLightManager::Get().isDebugRendering());
}

void TestPhysic::onK()
{
    render::TheRenderManager::Get().enableLighting(!render::TheRenderManager::Get().isLightingEnabled());
}

void TestPhysic::onB()
{
    render::TheRenderManager::Get().enableVolumes(!render::TheRenderManager::Get().isVolumeDrawing());
}

void TestPhysic::onG()
{
    m_bRenderGrid = !m_bRenderGrid;
}

void TestPhysic::onR()
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

void TestPhysic::onSpace()
{
    m_bMove = !m_bMove;
}

void TestPhysic::onMinus()
{
    unsigned dtl = collision::TheCollider::Get().getDTL();
    if(dtl!=0)
        dtl--;
    collision::TheCollider::Get().setDTL(dtl);
}

void TestPhysic::onEquals()
{
    unsigned dtl = collision::TheCollider::Get().getDTL();
    if(++dtl>100)
        dtl = 100;
    collision::TheCollider::Get().setDTL(dtl);
}

void TestPhysic::onT()
{
    // testing purpose
    m_iRenderSpaceTree++;
    if(m_iRenderSpaceTree>=3)
        m_iRenderSpaceTree = 0;
};

void TestPhysic::onI()
{
    //Physic::Get().switchIdle();
};

void TestPhysic::onF()
{
    if (m_spModelPlane)
    {
        scene::TheScene::Get().getRootFrame()->removeChild(m_spModelPlane);
        m_spModelPlane = 0;
    }
    else
        createModel(m_spModelPlane, "plane");

    //Physic::Get().switchDebug();
};

void TestPhysic::onC()
{
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

void TestPhysic::onH()
{
    m_bShowHelp = !m_bShowHelp;
}

void TestPhysic::onE()
{
    m_iSpeed++;
    if(m_iSpeed>50)
        m_iSpeed = 1;
}

void TestPhysic::onXAxis(int dx)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = dx>accel ? dx*fast : dx*slow;

    if(m_bUseFPSCamera)
        m_spFirstPersonCamera->rotateLeft(-angle);
    else
        m_spTargetCamera->rotateLeft(-angle);
}

void TestPhysic::onYAxis(int dy)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = dy>accel ? dy*fast : dy*slow;
    if(m_bUseFPSCamera)
        m_spFirstPersonCamera->rotateUp(angle);
    else
        m_spTargetCamera->rotateUp(angle);
}

void TestPhysic::onWheelAxis(int dz)
{
    const float slow = .1f;
    if(m_bUseFPSCamera)
        m_spFirstPersonCamera->goForward(dz*slow);
    else
        m_spTargetCamera->goForward(dz*slow);
}