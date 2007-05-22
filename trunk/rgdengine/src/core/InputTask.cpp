#include "precompiled.h"

#include <rgde/core/InputTask.h>
#include <rgde/core/application.h>

#include <rgde/input/input.h> //библиотека ввода
#include <rgde/input/inputimpl.h>

#include <Windows.h>

namespace core
{
    InputTask::InputTask(const IApplication& app, unsigned int priority, bool exclusive) 
        : ITask(app, priority)
    {
        HWND hWnd = (HWND)IApplication::Get()->getWindowHandle();

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