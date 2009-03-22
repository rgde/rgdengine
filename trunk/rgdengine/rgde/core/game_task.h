#pragma once

#include <rgde/core/task.h>

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
	};
}