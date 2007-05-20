#include "precompiled.h"

#include "core/RenderTask.h"
#include "core/application.h"

namespace core
{
	RenderTask::RenderTask(const IApplication& app, unsigned int priority) 
		:	ITask(app, priority), 
			m_spRenderDevice(IRenderSystem::Create(app))
	{
	}

	void RenderTask::run()
	{
		m_spRenderDevice->update();
	}
}
