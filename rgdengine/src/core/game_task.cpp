#include "precompiled.h"

#include <rgde/core/game_task.h>


namespace core
{
	game_task::game_task(const application& app,  unsigned int priority, const std::string& start_script) 
		: base_task(app, priority)
	{
		m_game.init(start_script);
	}

	game_task::~game_task ()
	{
	}

	void game_task::run()
	{
		m_game.update();
	}

}