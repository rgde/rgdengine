#include "precompiled.h"

#include "core/RenderTask.h"
#include "core/application.h"

namespace core
{
	CRenderTask::CRenderTask(const IApplication& app, unsigned int priority) 
		:	ITask(app, priority), 
			m_spRenderDevice(IRenderSystem::Create(app))
	{
	}

	void CRenderTask::run()
	{
		m_spRenderDevice->update();
	}
}