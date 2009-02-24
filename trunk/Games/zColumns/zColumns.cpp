#include "stdafx.h"
#include "zColumns.h"

#include "game.h"


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::auto_ptr<core::application> pApp(core::application::create(L"RGDE: zColumns Game", 400, 600, false));
	pApp->addTask(core::task_ptr(new core::input_task(*pApp, 0, false)));
	pApp->addTask(core::task_ptr(new core::game_task(*pApp, 1)));
	pApp->addTask(core::task_ptr(new core::render_task(*pApp, 2)));

	{
		zcollumns::Game game;
		pApp->Run();
	}

	return 0;
}