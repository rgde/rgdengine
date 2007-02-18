#include "precompiled.h"
#include "core/InputTask.h"
#include "core/application.h"

#include "input/IL.h"                 //библиотека ввода
#include "input/ILSystemImplDx.h"     //имплементация ввода при помощи DInput
#include "input/ILSchemeImplSimple.h" //имплементация раскладки клавиша-контрол

namespace core
{
	CInputTask::CInputTask(const IApplication& app, unsigned int priority, bool exclusive) 
		: ITask(app, priority)
	{
		HWND hWnd = (HWND)IApplication::Get()->getWindowHandle();
		input::CSystem::Get().init( new input::CSystemImplDx(hWnd, exclusive) );
		input::CScheme::Get().init( new input::CSchemeImplSimple() );
	}

	CInputTask::~CInputTask ()
	{
		input::CScheme::Destroy();
		input::CSystem::Destroy();
	}

	void CInputTask::run()
	{
		input::CSystem::Update();
	}
}