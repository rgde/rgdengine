//main.cpp
#include <rgde/engine.h>
#include "TestTrack.h"

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    math::CTrack t; 

    std::auto_ptr<core::IApplication> pApp(core::IApplication::Create(L"Track", 800, 600, false, false));
    pApp->addTask(core::PTask(new core::InputTask(*pApp, 0, false)));
    pApp->addTask(core::PTask(new core::CGameTask(*pApp, 1)));
    pApp->addTask(core::PTask(new core::RenderTask(*pApp, 2)));

    TestTrack r;

    pApp->Run();

    return 0;
}