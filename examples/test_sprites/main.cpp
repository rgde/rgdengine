#include "sprites.h"

// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	std::auto_ptr<core::application> pApp(core::application::create(L"Test_Sprites Example", 640, 480, false));
	pApp->add(core::task_ptr(new core::input_task(*pApp, 0, false)));
	pApp->add(core::task_ptr(new core::game_task(*pApp, 1)));
	pApp->add(core::task_ptr(new core::render_task(*pApp, 2)));

	{
		rgde::exapmles::sprites::sprite_example test;

		pApp->run();
	}
}