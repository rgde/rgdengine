#pragma once

#include <rgde/core/task.h>
#include <rgde/game/game.h>

namespace core
{
	class game_task: public core::base_task
	{
	public:
		game_task(const application& app, unsigned int priority = 0, 
			const std::string& start_script = "");
		virtual ~game_task();

	protected:
		virtual void run();

	private:
		game::game_system m_game;
	};
}