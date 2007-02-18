#pragma once

#include "core/Task.h"
#include "core/coreRenderDevice.h"

namespace core
{
	class CRenderTask : public ITask
	{
	public:
		CRenderTask(const IApplication& app, unsigned int priority = 0);

	protected:
		virtual void run();

	private:
		PRenderSystem m_spRenderDevice;
	};
}