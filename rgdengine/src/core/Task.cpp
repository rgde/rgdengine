#include "precompiled.h"

#include <rgde/core/task.h>
#include "../base/exception.h"

namespace core
{
	base_task::base_task(const application& app, int priority)
		: m_priority(priority), m_application(app)
	{
		m_is_started = true;
		m_is_paused = false;
	}

	base_task::~base_task()
	{
	}


	void base_task::start()
	{
		m_is_started = true;
		m_is_paused = false;
	}
	void base_task::stop()
	{
		m_is_started = false;
		m_is_paused = false;		
	}

	void base_task::pause()
	{
		if (m_is_started)
			m_is_paused = !m_is_paused;	
	}

	void base_task::execute()
	{
		//guard(base_task::execute())
		if (m_is_started && !m_is_paused)
			run();
		//unguard
	}
}