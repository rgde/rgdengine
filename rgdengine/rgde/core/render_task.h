#pragma once

#include <rgde/core/task.h>
#include <rgde/core/render_system.h>

namespace core
{
	class render_task : public base_task
	{
	public:
		render_task(const application& app, unsigned int priority = 0);

	protected:
		virtual void run();

	private:
		PRenderSystem m_spRenderDevice;
	};
}