#pragma once

#include <rgde/core/task.h>


namespace core
{
	class GameTask: public core::ITask
	{
	public:
		GameTask(const IApplication& app, unsigned int priority = 0, const std::string& start_script = "");
		virtual ~GameTask();

	protected:
		virtual void run();
	};

}