#include "precompiled.h"

#include <rgde/core/input_task.h>
#include <rgde/core/application.h>
#include <rgde/input/input.h> //библиотека ввода

#include <Windows.h>

#include "../input/inputimpl.h"

namespace core
{
    input_task::input_task(const application& app, unsigned int priority, bool exclusive) 
        : base_task(app, priority)
    {
        HWND hWnd = (HWND)application::get()->getWindowHandle();

        input::input_impl *pImpl = new input::input_impl();
        pImpl->Init(hWnd,exclusive);

        input::Input::get().init( pImpl );
    }

    input_task::~input_task ()
    {
        input::Input::Destroy();
    }

    void input_task::run()
    {
        input::Input::update();
    }
}