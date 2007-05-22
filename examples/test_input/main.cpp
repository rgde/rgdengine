//main.cpp
#include <rgde/engine.h>
#include "TestInput.h"

int main()
{
    std::auto_ptr<core::IApplication> pApp(core::IApplication::Create(L"Test Input", 800, 600, false));
    pApp->addTask(core::PTask(new core::InputTask(*pApp, 0, true)));
    pApp->addTask(core::PTask(new core::CGameTask(*pApp, 1)));
    pApp->addTask(core::PTask(new core::RenderTask(*pApp, 2)));

    TestInput r;

    pApp->Run();

    return 0;
}