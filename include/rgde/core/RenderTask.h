#pragma once

#include "core/Task.h"
#include "core/coreRenderDevice.h"

namespace core
{
	class RenderTask : public ITask
	{
	public:
		RenderTask(const IApplication& app, unsigned int priority = 0);

	protected:
		virtual void run();

	private:
		PRenderSystem m_spRenderDevice;
	};
}