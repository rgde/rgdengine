//GameTask.h
#pragma once

#include "core/Task.h"

#include <string>


namespace core
{
	class CGameTask: public core::ITask
	{
	public:
		CGameTask(const IApplication& app, unsigned int priority = 0, const std::string& start_script = "");
		virtual ~CGameTask();

	protected:
		virtual void run();
	};

}