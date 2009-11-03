#include <rgde/engine.h>
#include "conio.h"

#include "SnowEffect.h"

using namespace core;
using namespace game;

typedef std::auto_ptr<application> PApplication;

int main()
{
	{
		PApplication pApp(application::create(L"RGDE: Game Test", 640, 480, false));

		pApp->add(task_ptr(new render_task(*pApp, 0)));
		pApp->add(task_ptr(new input_task(*pApp, 1, false)));
        //->
		//pApp->add(task_ptr(new game_task(*pApp, 2, "gameconfig.xml")));
        pApp->add(task_ptr(new game_task(*pApp, 2)));

        game::game_system::get().addLevel("intro", "menu");
        game::game_system::get().addLevelTypeToCreate("intro", "AviLevelObject");

        game::game_system::get().addLevel("menu", "play");
        game::game_system::get().addLevelTypeToCreate("menu", "MenuLevelObject");

        game::game_system::get().addLevel("play", "menu");
        game::game_system::get().addLevelTypeToCreate("play", "PlayLevelObject");
        game::game_system::get().addLevelTypeToCreate("play", "MapLevelObject");

        game::game_system::get().setCurrentLevel("intro");
        //-<

		game::effects::SnowEffect background_effect;

		pApp->run();
	}

	//_getch();

	return 0;
}