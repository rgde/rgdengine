#include "precompiled.h"
#include "core/InputTask.h"
#include "core/application.h"

#include "input/input.h" //библиотека ввода

namespace core
{
    CInputTask::CInputTask(const IApplication& app, unsigned int priority, bool exclusive) 
        : ITask(app, priority)
    {
        HWND hWnd = (HWND)IApplication::Get()->getWindowHandle();

        input::CInputImpl *pImpl = new input::CInputImpl();
        pImpl->Init(hWnd,exclusive);

        input::CInput::Get().init( pImpl );
    }

    CInputTask::~CInputTask ()
    {
        input::CInput::Destroy();
    }

    void CInputTask::run()
    {
        input::CInput::Update();
    }
}