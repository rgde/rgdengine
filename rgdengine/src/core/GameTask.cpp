#include "precompiled.h"

#include <rgde/core/GameTask.h>
#include <rgde/game/game.h>

namespace core
{
	GameTask::GameTask(const IApplication& app,  unsigned int priority, const std::string& start_script) 
		: ITask(app, priority)
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