#include "precompiled.h"

#include <rgde/core/render_task.h>
#include <rgde/core/application.h>

namespace core
{
	render_task::render_task(const application& app, unsigned int priority) 
		:	base_task(app, priority), 
			m_spRenderDevice(render_system::create(app))
	{
	}

	void render_task::run()
	{
		m_spRenderDevice->update();
	}
}
