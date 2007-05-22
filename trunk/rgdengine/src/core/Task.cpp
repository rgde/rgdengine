#include "precompiled.h"

#include <rgde/core/Task.h>
#include "../base/exception.h"

namespace core
{
	ITask::ITask(const IApplication& app, int nPriority)
		: m_nPriority(nPriority), m_application(app)
	{
		m_bIsStarted = true;
		m_bIsPaused = false;
	}

	ITask::~ITask()
	{
	}


	void ITask::start()
	{
		m_bIsStarted = true;
		m_bIsPaused = false;
	}
	void ITask::stop()
	{
		m_bIsStarted = false;
		m_bIsPaused = false;		
	}

	void ITask::pause()
	{
		if (m_bIsStarted)
			m_bIsPaused = !m_bIsPaused;	
	}

	void ITask::execute()
	{
		//guard(ITask::execute())
		if (m_bIsStarted && !m_bIsPaused)
			run();
		//unguard
	}
}