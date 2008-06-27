#pragma once

#include "task.h"

namespace core
{
	class InputTask: public base_task
	{
	public:
		InputTask(const application& app, unsigned int priority = 0, bool exclusive = true);
		virtual ~InputTask();

	protected:
		virtual void run();
	};
}