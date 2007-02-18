#pragma once

#include "Task.h"

namespace core
{
	class CInputTask: public ITask
	{
	public:
		CInputTask(const IApplication& app, unsigned int priority = 0, bool exclusive = true);
		virtual ~CInputTask();

	protected:
		virtual void run();
	};
}