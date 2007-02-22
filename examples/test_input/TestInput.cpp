//TestInput.cpp
#include "engine.h"
#include "TestInput.h"

TestInput::TestInput()
{

    ///////
    // 1 //
    ///////
    {
        input::CInput::LoadFromFile("input.xml");
    }

    ///////
    // 2 //
    ///////
    {/*
        std::vector<char> data;

        io::CFileSystem &fs    = io::TheFileSystem::Get();
        io::PReadStream stream = fs.findFile("input.xml");
        io::StreamToVector<char>(data, stream);

        input::CInput::LoadFromString(std::string(data.begin(), data.end()));
    */}

    ///////
    // 3 //
    ///////
    {/*
        std::string sXml =
            "<?xml version=\"1.0\" encoding=\"utf-8\"?>"             \
            "<input>"                                                \
            "  <command name=\"Quit\">"                              \
            "    <control device=\"Keyboard\" name=\"KeyEscape\" />" \
            "  </command>"                                           \
            "  <command name=\"Left\">"                              \
            "    <control device=\"Keyboard\" name=\"KeyLeft\" />"   \
            "  </command>"                                           \
            "  <command name=\"Right\">"                             \
            "    <control device=\"Keyboard\" name=\"KeyRight\" />"  \
            "  </command>"                                           \
            "  <command name=\"Up\">"                                \
            "    <control device=\"Keyboard\" name=\"KeyUp\" />"     \
            "  </command>"                                           \
            "  <command name=\"Down\">"                              \
            "    <control device=\"Keyboard\" name=\"KeyDown\" />"   \
            "  </command>"                                           \
            "  <command name=\"SpeedUp\">"                           \
            "    <control device=\"Keyboard\" name=\"KeyQ\" />"      \
            "  </command>"                                           \
            "  <command name=\"SpeedDown\">"                         \
            "    <control device=\"Keyboard\" name=\"KeyA\" />"      \
            "  </command>"                                           \
            "  <command name=\"Pause\">"                             \
            "    <control device=\"Keyboard\" name=\"KeyP\" />"      \
            "    <control device=\"Keyboard\" name=\"KeySpace\" />"  \
            "    <control device=\"Mouse\" name=\"ButtonLeft\" />"   \
            "  </command>"                                           \
            "  <command name=\"Horz\">"                              \
            "    <control device=\"Mouse\" name=\"AxisX\" />"        \
            "  </command>"                                           \
            "  <command name=\"Vert\">"                              \
            "    <control device=\"Mouse\" name=\"AxisY\" />"        \
            "  </command>"                                           \
            "</input>";

        input::CInput::LoadFromString(sXml);
    */}

    ///////
    // 4 //
    ///////
    {/*
        using namespace input;

        //создадим команды
        CInput::addCommand(L"Quit");
        CInput::addCommand(L"Left");
        CInput::addCommand(L"Right");
        CInput::addCommand(L"Up");
        CInput::addCommand(L"Down");
        CInput::addCommand(L"SpeedUp");
        CInput::addCommand(L"SpeedDown");
        CInput::addCommand(L"Pause");
        CInput::addCommand(L"Horz");
        CInput::addCommand(L"Vert");

        //свяжем команды с контролами
        CInput::getDevice(Keyboard)->getControl(KeyEscape )->bind(L"Quit");
        CInput::getDevice(Keyboard)->getControl(KeyLeft   )->bind(L"Left");
        CInput::getDevice(Keyboard)->getControl(KeyRight  )->bind(L"Right");
        CInput::getDevice(Keyboard)->getControl(KeyUp     )->bind(L"Up");
        CInput::getDevice(Keyboard)->getControl(KeyDown   )->bind(L"Down");
        CInput::getDevice(Keyboard)->getControl(KeyQ      )->bind(L"SpeedUp");
        CInput::getDevice(Keyboard)->getControl(KeyA      )->bind(L"SpeedDown");
        CInput::getDevice(Keyboard)->getControl(KeyP      )->bind(L"Pause");
        CInput::getDevice(Keyboard)->getControl(KeySpace  )->bind(L"Pause");
        CInput::getDevice(Mouse   )->getControl(ButtonLeft)->bind(L"Pause");
        CInput::getDevice(Mouse   )->getControl(AxisX     )->bind(L"Horz");
        CInput::getDevice(Mouse   )->getControl(AxisY     )->bind(L"Vert");
    */}

    //настроим хелперы
    m_keyupQuit.   attach(L"Quit");
    m_btnLeft.     attach(L"Left");
    m_btnRight.    attach(L"Right");
    m_btnUp.       attach(L"Up");
    m_btnDown.     attach(L"Down");
    m_btnSpeedUp.  attach(L"SpeedUp");
    m_btnSpeedDown.attach(L"SpeedDown");
    m_trigPause.   attach(L"Pause");
    m_raxisHorz.   attach(L"Horz");
    m_raxisVert.   attach(L"Vert");

    //биндим функции для перехвата событий
    m_keyupQuit += boost::bind(&TestInput::onQuit, this);
    m_raxisHorz += boost::bind(&TestInput::onHorz, this, _1);
    m_raxisVert += boost::bind(&TestInput::onVert, this, _1);
}

void TestInput::update (float dt)
{
    //...
}

void TestInput::onQuit ()
{
    core::IApplication::Get()->close();
}

void TestInput::onHorz (int dx)
{
    if (!m_trigPause)
    {
        std::wcout << L"TestInput::onHorz(" << dx <<")\n";
    }
}

void TestInput::onVert (int dy)
{
    if (!m_trigPause)
    {
        std::wcout << L"TestInput::onVert(" << dy <<")\n";
    }
}
