#include "precompiled.h"

#include <rgde/core/GameTask.h>
#include <rgde/game/game.h>

namespace core
{
	CGameTask::CGameTask(const IApplication& app,  unsigned int priority, const std::string& start_script) 
		: ITask(app, priority)
	{
		game::TheGame::Get().init(start_script);
	}

	CGameTask::~CGameTask ()
	{
		game::TheGame::Destroy();
	}

	void CGameTask::run()
	{
		game::TheGame::Get().update();
	}

}