#include "precompiled.h"

#include <rgde/core/game_task.h>
#include <rgde/game/game.h>

namespace core
{
	GameTask::GameTask(const application& app,  unsigned int priority, const std::string& start_script) 
		: base_task(app, priority)
	{
		game::TheGame::Get().init(start_script);
	}

	GameTask::~GameTask ()
	{
		game::TheGame::Destroy();
	}

	void GameTask::run()
	{
		game::TheGame::Get().update();
	}

}