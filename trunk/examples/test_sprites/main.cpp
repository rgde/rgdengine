#include "sprites.h"

// The application's entry point
int __stdcall WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, int )
{
	typedef std::auto_ptr<core::application> application_ptr;

	application_ptr app(core::application::create(L"Test_Sprites Example", 640, 480, false));
	
	app->add<core::input_task>(0, false);
	app->add<core::game_task>(1);
	app->add<core::render_task>(2);

	{
		rgde::exapmles::sprites::sprite_example test;

		app->run();
	}
}