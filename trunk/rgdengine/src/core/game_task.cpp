#include "precompiled.h"

#include <rgde/core/game_task.h>
#include <rgde/game/game.h>

namespace core
{
	game_task::game_task(const application& app,  unsigned int priority, const std::string& start_script) 
		: base_task(app, priority)
	{
		game::TheGame::get().init(start_script);
	}

	game_task::~game_task ()
	{
		game::TheGame::destroy();
	}

	void game_task::run()
	{
		game::TheGame::get().update();
	}

}