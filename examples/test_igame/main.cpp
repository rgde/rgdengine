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

		pApp->addTask(task_ptr(new render_task(*pApp, 0)));
		pApp->addTask(task_ptr(new input_task(*pApp, 1, false)));
        //->
		//pApp->addTask(task_ptr(new game_task(*pApp, 2, "gameconfig.xml")));
        pApp->addTask(task_ptr(new game_task(*pApp, 2)));

        game::TheGame::get().addLevel("intro", "menu");
        game::TheGame::get().addLevelTypeToCreate("intro", "AviLevelObject");

        game::TheGame::get().addLevel("menu", "play");
        game::TheGame::get().addLevelTypeToCreate("menu", "MenuLevelObject");

        game::TheGame::get().addLevel("play", "menu");
        game::TheGame::get().addLevelTypeToCreate("play", "PlayLevelObject");
        game::TheGame::get().addLevelTypeToCreate("play", "MapLevelObject");

        game::TheGame::get().setCurrentLevel("intro");
        //-<

		game::effects::SnowEffect background_effect;

		pApp->Run();
	}

	//_getch();

	return 0;
}