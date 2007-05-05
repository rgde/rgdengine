//main.cpp
#include "engine.h"
#include "TrackEditor.h"

int __stdcall WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int)
{
    std::auto_ptr<core::IApplication> pApp(core::IApplication::Create(L"Track Editor", 800, 600, false, false));
    pApp->addTask(core::PTask(new core::CInputTask(*pApp, 0, false)));
    pApp->addTask(core::PTask(new core::CGameTask(*pApp, 1)));
    pApp->addTask(core::PTask(new core::CRenderTask(*pApp, 2)));

    TrackEditor r;

    pApp->Run();

    return 0;
}