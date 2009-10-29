//main.cpp
#include <rgde/engine.h>
#include "TestInput.h"

int main()
{
    std::auto_ptr<core::application> pApp(core::application::create(L"Test Input", 800, 600, false));
    pApp->add(core::task_ptr(new core::input_task(*pApp, 0, true)));
    pApp->add(core::task_ptr(new core::game_task(*pApp, 1)));
    pApp->add(core::task_ptr(new core::render_task(*pApp, 2)));

    TestInput r;

    pApp->run();

    return 0;
}