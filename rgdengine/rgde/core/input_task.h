#pragma once

#include "task.h"

namespace core
{
	class InputTask: public ITask
	{
	public:
		InputTask(const IApplication& app, unsigned int priority = 0, bool exclusive = true);
		virtual ~InputTask();

	protected:
		virtual void run();
	};
}