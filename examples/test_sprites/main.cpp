#include "sprites.h"

// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	std::auto_ptr<core::application> pApp(core::application::Create(L"Test_Sprites Example", 640, 480, false));
	pApp->addTask(core::task_ptr(new core::InputTask(*pApp, 0, false)));
	pApp->addTask(core::task_ptr(new core::GameTask(*pApp, 1)));
	pApp->addTask(core::task_ptr(new core::render_task(*pApp, 2)));

	{
		rgde::exapmles::sprites::sprite_example test;

		pApp->Run();
	}
}