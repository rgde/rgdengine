//main.cpp
#include <rgde/engine.h>
#include "TestInput.h"

int main()
{
    std::auto_ptr<core::application> pApp(core::application::create(L"Test Input", 800, 600, false));
    pApp->addTask(core::task_ptr(new core::input_task(*pApp, 0, true)));
    pApp->addTask(core::task_ptr(new core::game_task(*pApp, 1)));
    pApp->addTask(core::task_ptr(new core::render_task(*pApp, 2)));

    TestInput r;

    pApp->Run();

    return 0;
}