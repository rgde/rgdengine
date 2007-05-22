#pragma once

#include <rgde/core/Task.h>
#include <rgde/core/coreRenderDevice.h>

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