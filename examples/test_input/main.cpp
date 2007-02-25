//main.cpp
#include "engine.h"
#include "TestInput.h"

int main()
{
    std::auto_ptr<core::IApplication> pApp(core::IApplication::Create(L"Test Input", 800, 600, 32, 85, false));
    pApp->addTask(core::PTask(new core::CInputTask(*pApp, 0, true)));
    pApp->addTask(core::PTask(new core::CGameTask(*pApp, 1)));
    pApp->addTask(core::PTask(new core::CRenderTask(*pApp, 2)));

    TestInput r;

    pApp->Run();

    return 0;
}