#pragma once

#include <rgde/core/task.h>

namespace core
{
	class input_task: public base_task
	{
	public:
		input_task(const application& app, unsigned int priority = 0, 
			bool exclusive = true);
		virtual ~input_task();

	protected:
		virtual void run();
	};
}