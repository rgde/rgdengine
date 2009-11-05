//main.cpp
#include <rgde/engine.h>
#include "TestInput.h"

int main()
{
    std::auto_ptr<core::application> app(core::application::create(L"Test Input", 800, 600, false));
    
	app->add<core::input_task>(0, true);
    app->add<core::game_task>(1);
    app->add<core::render_task>(2);

	{
		TestInput r;

		app->run();
	}

    return 0;
}