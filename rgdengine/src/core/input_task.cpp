#include "precompiled.h"

#include <rgde/core/input_task.h>
#include <rgde/core/application.h>
#include <rgde/input/input.h> //библиотека ввода

#include <Windows.h>

#include "../input/inputimpl.h"

namespace core
{
    InputTask::InputTask(const application& app, unsigned int priority, bool exclusive) 
        : base_task(app, priority)
    {
        HWND hWnd = (HWND)application::Get()->getWindowHandle();

        input::InputImpl *pImpl = new input::InputImpl();
        pImpl->Init(hWnd,exclusive);

        input::Input::Get().init( pImpl );
    }

    InputTask::~InputTask ()
    {
        input::Input::Destroy();
    }

    void InputTask::run()
    {
        input::Input::Update();
    }
}