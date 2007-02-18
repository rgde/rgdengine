//TrackEditor.cpp
#include "engine.h"
#include "TrackEditor.h"
#include "boost/lexical_cast.hpp" //возможно стоит переделать на base::Lexical_cast [Dub]

TrackEditor::TrackEditor()
{
    m_spCamera = render::CRenderCamera::Create();
    m_spFlyCamera = math::CFlyCamera::Create(m_spCamera);
    m_spTrackCamera = math::CTrackCamera::Create(m_spCamera);

    render::TheCameraManager::Get().addCamera(m_spCamera);
    render::TheRenderManager::Get().enableVolumes(false);
    render::TheRenderManager::Get().enableLighting(true);

    m_spFont = render::IFont::Create(11, L"Arial", render::IFont::Heavy);

    initInput();
    initCamera();
    initLights();
    initEditor();

    m_spModel = render::CModel::Create("teapot");
}

TrackEditor::~TrackEditor()
{
}

void TrackEditor::initInput()
{
    //привязываем команды к контролам
    m_cEsc  .attachToControl(input::Keyboard, input::KeyEscape); //ESC - выход
    m_cTab  .attachToControl(input::Keyboard, input::KeyTab);
    m_c0    .attachToControl(input::Keyboard, input::Key0);
    m_cQ    .attachToControl(input::Keyboard, input::KeyQ);
    m_cW    .attachToControl(input::Keyboard, input::KeyW);
    m_cE    .attachToControl(input::Keyboard, input::KeyE);
    m_cA    .attachToControl(input::Keyboard, input::KeyA);
    m_cS    .attachToControl(input::Keyboard, input::KeyS);
    m_cD    .attachToControl(input::Keyboard, input::KeyD);
    m_cF    .attachToControl(input::Keyboard, input::KeyF);
    m_cC    .attachToControl(input::Keyboard, input::KeyC);
    m_cF1   .attachToControl(input::Keyboard, input::KeyF1);
    m_cF2   .attachToControl(input::Keyboard, input::KeyF2);
    m_cF3   .attachToControl(input::Keyboard, input::KeyF3);
    m_cF4   .attachToControl(input::Keyboard, input::KeyF4);
    m_cZ    .attachToControl(input::Keyboard, input::KeyZ);
    m_cX    .attachToControl(input::Keyboard, input::KeyX);
    m_cO    .attachToControl(input::Keyboard, input::KeyO);
    m_cP    .attachToControl(input::Keyboard, input::KeyP);
    m_cSpace.attachToControl(input::Keyboard, input::KeySpace);
    m_cDel  .attachToControl(input::Keyboard, input::KeyDelete);
    m_cXAxis.attachToControl(input::Mouse, input::AxisX);
    m_cYAxis.attachToControl(input::Mouse, input::AxisY);

    //задаем для команд функции-обработчики
    m_cEsc  .addHandler(this,&TrackEditor::onEsc);
    m_cTab  .addHandler(this,&TrackEditor::onInfo);
    m_c0    .addHandler(this,&TrackEditor::onReset);
    m_cF1   .addHandler(this,&TrackEditor::onLoadModel);
    m_cF2   .addHandler(this,&TrackEditor::onLoadTrack);
    m_cF3   .addHandler(this,&TrackEditor::onSaveTrack);
    m_cF4   .addHandler(this,&TrackEditor::onChangeMode);
    m_cZ    .addHandler(this,&TrackEditor::onPrevKey);
    m_cX    .addHandler(this,&TrackEditor::onNextKey);
    m_cO    .addHandler(this,&TrackEditor::onFaster);
    m_cP    .addHandler(this,&TrackEditor::onSlower);
    m_cSpace.addHandler(this,&TrackEditor::onAddKey);
    m_cDel  .addHandler(this,&TrackEditor::onDelKey);
    m_cYAxis.addHandler(this,&TrackEditor::onYAxis);
    m_cXAxis.addHandler(this,&TrackEditor::onXAxis);
}

void TrackEditor::initCamera()
{
    math::Vec3f vUpVec    ( 0.0f,   0.0f, 1.0f );
    math::Vec3f vEyePt    ( 0.0f, 300.0f, 0.0f );
    math::Vec3f vLookatPt ( 0.0f,   0.0f, 0.0f );

    m_spCamera->setProjection(math::Math::PI/4, 4.f/3.f, 1.0f, 10000.0f);
    m_spFlyCamera->setPosition(vUpVec,vEyePt,vLookatPt);
    m_spFlyCamera->activate();

    m_spTrackCamera->m_cTrack.m_values.clear();
    m_spTrackCamera->m_cTrack.apply();
}

void TrackEditor::initLights()
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

void TrackEditor::initEditor()
{
    m_bPlay = false;
    m_bInfo = true;
    i = m_spTrackCamera->m_cTrack.m_values.end();
}

void TrackEditor::update(float dt)
{
    //фпс
    render::TheDevice::Get().showFPS(m_spFont);

    //справка
    if (m_bInfo)
    {
        std::wstring help =
        std::wstring(L"ESC - Выход\n"
        L"TAB - Скрыть/показать справку\n"
        L"0(ноль) - Сбросить программу в начальное состояние\n"
        L"\n"
        L"F1 - Загрузить модель (disable)\n"
        L"F2 - Загрузить трек\n"
        L"F3 - Сохранить трек\nF4 - ") +
        //L"F4 - Смена режима редактирование/проигрывание трека\n"
        std::wstring(m_bPlay ? L"Перейти в режим редактирования трека" : L"Перейти в режим проигрывания трека")
        +
        std::wstring(L"\n"
        L"Space - Добавить ключ\n"
        L"Del - Удалить текущий ключ\n"
        L"Z - Перейти к предыдущему ключу\n"
        L"X - Перейти к следующему ключу\n"
        L"O - Увеличить скорость в текущем ключе\n"
        L"P - Уменьшить скорость в текущем ключе\n"
        L"\n"
        L"W - Вперед\n"
        L"S - Назад\n"
        L"A - Влево\n"
        L"D - Вправо\n"
        L"Q - Поворот против часовой стрелки\n"
        L"E - Поворот по часовой стрелке\n"
        L"F - Вверх\n"
        L"C - Вниз\n");

        m_spFont->render(help,math::Rect(9,259,400,400),0xFF000000,false);
        m_spFont->render(help,math::Rect(8,257,400,400),0xFFFFFFFF,false);
    }

    //в каком режиме находимся
    std::wstring mode = m_bPlay ? L"Проигрывание трека" : L"Редактирование трека";
    m_spFont->render(mode,math::Rect(9,18,400,400),0xFF000000,false);
    m_spFont->render(mode,math::Rect(8,16,400,400),0xFFFF0000,false);

    //в каждом режиме выводим свою информацию
    if (m_bPlay)
    {
        //режим проигрывания
        math::CTrack::Key key = m_spTrackCamera->m_cTrack.getKey(m_spTrackCamera->getPosition());

        //общая длительность трека
        //текущая позиция
        //текущая скорость
        std::wstring infoL =
        L"положение (общая длительность трека):\n"
        L"текущая позиция:\n"
        L"текущая скорость:";

        std::wstring position        = boost::lexical_cast<std::wstring>(m_spTrackCamera->getPosition()/100.f);
        std::wstring totalTime       = boost::lexical_cast<std::wstring>(m_spTrackCamera->m_cTrack.getTotalTime()/100.f);
        std::wstring currentPosition = L"(" +
                                       boost::lexical_cast<std::wstring>(key.m_vEyePt.mData[0]) + L",  " +
                                       boost::lexical_cast<std::wstring>(key.m_vEyePt.mData[1]) + L",  " +
                                       boost::lexical_cast<std::wstring>(key.m_vEyePt.mData[2]) + L")";
        std::wstring speed           = boost::lexical_cast<std::wstring>(key.m_fSpeed);

        std::wstring infoR =
        position + L"   (" +totalTime + L") сек\n" +
        currentPosition + L"\n" +
        speed;

        m_spFont->render(infoL,math::Rect(9,46,400,400),0xFF000000,false);
        m_spFont->render(infoL,math::Rect(8,44,400,400),0xFF88FF88,false);
        m_spFont->render(infoR,math::Rect(249,46,400,400),0xFF000000,false);
        m_spFont->render(infoR,math::Rect(248,44,400,400),0xFFFFFFFF,false);
    }
    else
    {
        //режим редактирования

        //общая длительность трека
        //номер текущего ключа (общее кол-во ключей)
        //скорость в текущем ключе
        std::wstring infoL =
        L"общая длительность трека:\n"
        L"номер текущего ключа (общее кол-во ключей):\n"
        L"скорость в текущем ключе:";

        std::wstring totalTime  = boost::lexical_cast<std::wstring>(m_spTrackCamera->m_cTrack.getTotalTime()/100.f);
        std::wstring currentNum = L"-";
        std::wstring size       = boost::lexical_cast<std::wstring>(m_spTrackCamera->m_cTrack.m_values.size());
        std::wstring speed      = L"-";

        if (i != m_spTrackCamera->m_cTrack.m_values.end())
        {
            int num = 1;
            math::CTrack::iterator it = m_spTrackCamera->m_cTrack.m_values.begin();
            while (it != m_spTrackCamera->m_cTrack.m_values.end())
            {
                if (i == it)
                    currentNum = boost::lexical_cast<std::wstring>(num);
                ++num;
                ++it;
            }

            speed = boost::lexical_cast<std::wstring>(i->m_fSpeed);
        }

        std::wstring infoR =
        totalTime + L" сек\n" +
        currentNum + L"   (" + size + L")\n" +
        speed;

        m_spFont->render(infoL,math::Rect(9,46,400,400),0xFF000000,false);
        m_spFont->render(infoL,math::Rect(8,44,400,400),0xFF88FF88,false);
        m_spFont->render(infoR,math::Rect(289,46,400,400),0xFF000000,false);
        m_spFont->render(infoR,math::Rect(288,44,400,400),0xFFFFFFFF,false);

        //кривулина
        //->
        //float t=0;
        //const float step = m_spTrackCamera->m_cTrack.getTotalTime()/100;
        //while (t<m_spTrackCamera->m_cTrack.getTotalTime())
        //{
        //    math::CTrack::Key key1 = m_spTrackCamera->m_cTrack.getKey(t);
        //    math::CTrack::Key key2 = m_spTrackCamera->m_cTrack.getKey(t+step);

        //    math::Vec3f v1 = key1.m_vEyePt;
        //    math::Vec3f v2 = key2.m_vEyePt;

        //    t += 2*step;
        //    render::Line3dManager::Get().addLine(v1,v2);
        //}
        float t=0;
        const float step = 1.f/10.f;
        while (t <= m_cSpline.length())
        {
            math::Vec3f v1 = m_cSpline.interpolate(t);
            math::Vec3f v2 = m_cSpline.interpolate(t+step);

            t += step;
            render::Line3dManager::Get().addLine(v1,v2);
        }
        //-<

        //текущий ключ
        if (i != m_spTrackCamera->m_cTrack.m_values.end())
        {
            math::Vec3f halfSize = math::Vec3f(1.5f,1.5f,1.5f);
            math::AABoxf box(i->m_vEyePt-halfSize, i->m_vEyePt+halfSize);
            render::Line3dManager::Get().addBox(math::MAT_IDENTITY44F, box, 0xFFFF0000);
        }
    }

    //движение
    if (!m_bPlay)
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
    }
    else
    {
        m_spTrackCamera->goForward(100.0f*dt);
        if (m_spTrackCamera->getPosition() >= m_spTrackCamera->m_cTrack.getTotalTime())
            m_spTrackCamera->goTo(0);
    }
}

void TrackEditor::onEsc(const input::CButtonEvent &event)
{
    if (event.m_bPress)
        core::IApplication::Get()->close();
}

void TrackEditor::onInfo(const input::CButtonEvent &event)
{
    if (event.m_bPress)
        m_bInfo = !m_bInfo;
}

void TrackEditor::onReset(const input::CButtonEvent &event)
{
    if (event.m_bPress)
    {
        initCamera();
        initEditor();
    }
}

void TrackEditor::onLoadModel(const input::CButtonEvent &event)
{/*
    if (!event.m_bPress)
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
    ofn.lpstrFilter       = L"Model files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
    ofn.lpstrCustomFilter = (LPWSTR) NULL;
    ofn.nMaxCustFilter    = 0L;
    ofn.nFilterIndex      = 1L;
    ofn.lpstrFile         = (wchar_t*)file.c_str();
    ofn.nMaxFile          = (DWORD)file.size();
    ofn.lpstrFileTitle    = (wchar_t*)fileTitle.c_str();
    ofn.nMaxFileTitle     = (DWORD)fileTitle.size();
    ofn.lpstrInitialDir   = NULL;
    ofn.lpstrTitle        = L"Open a Model File";
    ofn.nFileOffset       = 0;
    ofn.nFileExtension    = 0;
    ofn.lpstrDefExt       = L"*.xml";
    ofn.lCustData         = 0;

    ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        SetCurrentDirectory((wchar_t*)current_dir.c_str());
		std::wstring tt(ofn.lpstrFileTitle);
		std::string file_name = io::helpers::getFileNameWithoutExtension(std::string(tt.begin(),tt.end()));
        //todo: бага, старая модель не удалятся -> надо фиксить [Dub]
        m_spModel = render::CModel::Create(file_name);
    }       
*/}

void TrackEditor::onLoadTrack(const input::CButtonEvent &event)
{
    if (!event.m_bPress)
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
    ofn.lpstrFilter       = L"Track files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
    ofn.lpstrCustomFilter = (LPWSTR) NULL;
    ofn.nMaxCustFilter    = 0L;
    ofn.nFilterIndex      = 1L;
    ofn.lpstrFile         = (wchar_t*)file.c_str();
    ofn.nMaxFile          = (DWORD)file.size();
    ofn.lpstrFileTitle    = (wchar_t*)fileTitle.c_str();
    ofn.nMaxFileTitle     = (DWORD)fileTitle.size();
    ofn.lpstrInitialDir   = NULL;
    ofn.lpstrTitle        = L"Open a Track File";
    ofn.nFileOffset       = 0;
    ofn.nFileExtension    = 0;
    ofn.lpstrDefExt       = L"*.xml";
    ofn.lCustData         = 0;

    ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        SetCurrentDirectory((wchar_t*)current_dir.c_str());

		std::wstring temp(ofn.lpstrFileTitle);
		m_spTrackCamera->m_cTrack.load(std::string(temp.begin(), temp.end()));
        i = m_spTrackCamera->m_cTrack.m_values.begin();
        //->
        moveTrack2Spline();
        //-<
    }       
}

void TrackEditor::onSaveTrack(const input::CButtonEvent &event)
{
    if (!event.m_bPress)
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
    ofn.lpstrFilter       = L"Track files (*.xml)\0*.xml\0All Files (*.*)\0*.*\0";
    ofn.lpstrCustomFilter = (LPWSTR) NULL;
    ofn.nMaxCustFilter    = 0L;
    ofn.nFilterIndex      = 1L;
    ofn.lpstrFile         = (wchar_t*)file.c_str();
    ofn.nMaxFile          = (DWORD)file.size();
    ofn.lpstrFileTitle    = (wchar_t*)fileTitle.c_str();
    ofn.nMaxFileTitle     = (DWORD)fileTitle.size();
    ofn.lpstrInitialDir   = NULL;
    ofn.lpstrTitle        = L"Save a Track File";
    ofn.nFileOffset       = 0;
    ofn.nFileExtension    = 0;
    ofn.lpstrDefExt       = L"*.xml";
    ofn.lCustData         = 0;

    ofn.Flags = OFN_SHOWHELP | OFN_PATHMUSTEXIST;

    if (GetSaveFileName(&ofn))
    {
        SetCurrentDirectory((wchar_t*)current_dir.c_str());
		std::wstring temp(ofn.lpstrFileTitle);
		m_spTrackCamera->m_cTrack.save(std::string(temp.begin(), temp.end()));
    }       
}

void TrackEditor::onChangeMode(const input::CButtonEvent &event)
{
    if (event.m_bPress)
    {
        if (m_spTrackCamera->m_cTrack.getTotalTime() == 0)
            return;

        m_bPlay = !m_bPlay;
        if (m_bPlay)
            m_spTrackCamera->activate();
        else
        {
            math::CTrack::Key key = m_spTrackCamera->m_cTrack.getKey( m_spTrackCamera->getPosition() );
            m_spFlyCamera->setPosition(key.m_vUp, key.m_vEyePt, key.m_vLookatPt);
            m_spFlyCamera->activate();
        }
    }
}

void TrackEditor::onPrevKey(const input::CButtonEvent &event)
{
    if (event.m_bPress)
        if
        (
            i != m_spTrackCamera->m_cTrack.m_values.begin() &&
            i != m_spTrackCamera->m_cTrack.m_values.end()
        )
            --i;
}

void TrackEditor::onNextKey(const input::CButtonEvent &event)
{
    if (event.m_bPress)
        if (m_spTrackCamera->m_cTrack.m_values.size() > 0)
            if (i != --m_spTrackCamera->m_cTrack.m_values.end())
                ++i;
}

void TrackEditor::onFaster(const input::CButtonEvent &event)
{
    if (event.m_bPress)
        if (i != m_spTrackCamera->m_cTrack.m_values.end())
        {
            i->m_fSpeed += 0.1f;
            m_spTrackCamera->m_cTrack.apply();
        }
}

void TrackEditor::onSlower(const input::CButtonEvent &event)
{
    if (event.m_bPress)
        if (i != m_spTrackCamera->m_cTrack.m_values.end())
            if (i->m_fSpeed > 0.1f)
            {
                i->m_fSpeed -= 0.1f;
                m_spTrackCamera->m_cTrack.apply();
            }
}

void TrackEditor::onAddKey(const input::CButtonEvent &event)
{
    if (event.m_bPress)
    {
        math::CTrack::Key key;
        m_spFlyCamera->getPosition(key.m_vUp, key.m_vEyePt, key.m_vLookatPt);
        key.m_fSpeed = 1.f;

        if (i != m_spTrackCamera->m_cTrack.m_values.end())
            ++i;

        i = m_spTrackCamera->m_cTrack.m_values.insert(i, key);
        m_spTrackCamera->m_cTrack.apply();
        //->
        moveTrack2Spline();
        //-<
    }
}

void TrackEditor::onDelKey(const input::CButtonEvent &event)
{
    if (event.m_bPress)
        if (i != m_spTrackCamera->m_cTrack.m_values.end())
        {
            i = m_spTrackCamera->m_cTrack.m_values.erase(i);
            if
            (
                i == m_spTrackCamera->m_cTrack.m_values.end() &&
                i != m_spTrackCamera->m_cTrack.m_values.begin()
            )
                --i;
            m_spTrackCamera->m_cTrack.apply();
            //->
            moveTrack2Spline();
            //-<
        }
}

void TrackEditor::onXAxis(const input::CRelativeAxisEvent &event)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

    if (!m_bPlay)
        m_spFlyCamera->rotateLeft(-angle);
}

void TrackEditor::onYAxis(const input::CRelativeAxisEvent &event)
{
    const int accel = 5;
    const float slow = .01f;
    const float fast = 2*slow;
    float angle = event.m_nDelta>accel ? event.m_nDelta*fast : event.m_nDelta*slow;

    if (!m_bPlay)
        m_spFlyCamera->rotateUp(angle);
}

//->
void TrackEditor::moveTrack2Spline()
{
    m_cSpline.m_values.swap(math::Vec3fCatmulRomSpline::List());
    math::CTrack::iterator it = m_spTrackCamera->m_cTrack.m_values.begin();
    while (it != m_spTrackCamera->m_cTrack.m_values.end())
    {
        m_cSpline.m_values.push_back(it->m_vEyePt);
        ++it;
    }
}
//-<