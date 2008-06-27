#include "precompiled.h"

#include <rgde/core/task.h>
#include "../base/exception.h"

namespace core
{
	base_task::base_task(const application& app, int nPriority)
		: m_nPriority(nPriority), m_application(app)
	{
		m_bIsStarted = true;
		m_bIsPaused = false;
	}

	base_task::~base_task()
	{
	}


	void base_task::start()
	{
		m_bIsStarted = true;
		m_bIsPaused = false;
	}
	void base_task::stop()
	{
		m_bIsStarted = false;
		m_bIsPaused = false;		
	}

	void base_task::pause()
	{
		if (m_bIsStarted)
			m_bIsPaused = !m_bIsPaused;	
	}

	void base_task::execute()
	{
		//guard(base_task::execute())
		if (m_bIsStarted && !m_bIsPaused)
			run();
		//unguard
	}
}